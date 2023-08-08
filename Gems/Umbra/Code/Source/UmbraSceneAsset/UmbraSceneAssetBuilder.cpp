/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * its licensors.
 */

#include <AssetBuilderSDK/SerializationDependencies.h>
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Serialization/Json/JsonUtils.h>
#include <AzCore/Serialization/Utils.h>
#include <AzCore/std/algorithm.h>
#include <AzCore/std/smart_ptr/make_shared.h>
#include <AzFramework/IO/LocalFileIO.h>
#include <AzFramework/StringFunc/StringFunc.h>
#include <AzToolsFramework/API/EditorAssetSystemAPI.h>
#include <Umbra/UmbraSceneAsset/UmbraSceneAsset.h>
#include <UmbraSceneAsset/UmbraSceneAssetBuilder.h>
#include <UmbraSceneAsset/UmbraSceneDescriptor.h>

#include <umbra/optimizer/umbraLocalComputation.hpp>
#include <umbra/optimizer/umbraScene.hpp>
#include <umbra/runtime/umbraQuery.hpp>
#include <umbra/runtime/umbraTome.hpp>
#include <umbra/umbraInfo.hpp>

namespace Umbra
{
    namespace
    {
        [[maybe_unused]] const char* UmbraSceneAssetBuilderName = "UmbraSceneAssetBuilder";
        [[maybe_unused]] const char* UmbraSceneAssetBuilderJobKey = "Umbra Scene Asset Builder";
        [[maybe_unused]] const char* UmbraSceneAssetBuilderPattern = "*.umbrascene"; // File represents umbra scene data exported from editor.
    } // namespace

    void UmbraSceneAssetBuilder::RegisterBuilder()
    {
        // setup builder descriptor
        AssetBuilderSDK::AssetBuilderDesc builderDescriptor;
        builderDescriptor.m_name = UmbraSceneAssetBuilderJobKey;
        builderDescriptor.m_patterns.push_back(
            AssetBuilderSDK::AssetBuilderPattern(UmbraSceneAssetBuilderPattern, AssetBuilderSDK::AssetBuilderPattern::PatternType::Wildcard));
        builderDescriptor.m_busId = azrtti_typeid<UmbraSceneAssetBuilder>();
        builderDescriptor.m_createJobFunction =
            AZStd::bind(&UmbraSceneAssetBuilder::CreateJobs, this, AZStd::placeholders::_1, AZStd::placeholders::_2);
        builderDescriptor.m_processJobFunction =
            AZStd::bind(&UmbraSceneAssetBuilder::ProcessJob, this, AZStd::placeholders::_1, AZStd::placeholders::_2);
        builderDescriptor.m_version = 0;

        BusConnect(builderDescriptor.m_busId);

        AssetBuilderSDK::AssetBuilderBus::Broadcast(&AssetBuilderSDK::AssetBuilderBusTraits::RegisterBuilderInformation, builderDescriptor);
    }

    UmbraSceneAssetBuilder::~UmbraSceneAssetBuilder()
    {
        BusDisconnect();
    }

    void UmbraSceneAssetBuilder::CreateJobs(const AssetBuilderSDK::CreateJobsRequest& request, AssetBuilderSDK::CreateJobsResponse& response)
    {
        if (m_isShuttingDown)
        {
            response.m_result = AssetBuilderSDK::CreateJobsResultCode::ShuttingDown;
            return;
        }

        for (const AssetBuilderSDK::PlatformInfo& platformInfo : request.m_enabledPlatforms)
        {
            AssetBuilderSDK::JobDescriptor descriptor;
            descriptor.m_jobKey = UmbraSceneAssetBuilderJobKey;
            descriptor.SetPlatformIdentifier(platformInfo.m_identifier.c_str());
            descriptor.m_critical = true;

            response.m_createJobOutputs.push_back(descriptor);
        }

        response.m_result = AssetBuilderSDK::CreateJobsResultCode::Success;
    }

    void UmbraSceneAssetBuilder::ProcessJob(const AssetBuilderSDK::ProcessJobRequest& request, AssetBuilderSDK::ProcessJobResponse& response)
    {
        AssetBuilderSDK::JobCancelListener jobCancelListener(request.m_jobId);

        if (jobCancelListener.IsCancelled())
        {
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Cancelled;
            return;
        }

        if (m_isShuttingDown)
        {
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Cancelled;
            return;
        }

        // Attempt to load the umbra scene from the specified source file path.
        AZStd::string scenePath = request.m_fullPath;

        Umbra::Scene* scene = Umbra::Scene::create(scenePath.c_str());
        if (!scene)
        {
            AZ_Error("UmbraSceneAssetBuilder", false, "Failed to load umbra scene: %s", scenePath.c_str());
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        // Replace the extension on the source file path to attempt to load the scene descriptor.
        AZStd::string sceneDescPath = request.m_fullPath;
        AZ::StringFunc::Path::ReplaceExtension(sceneDescPath, UmbraSceneDescriptor::Extension);

        // The scene descriptor is an optional sidecar file with additional parameters to configure how the scene is processed. It's treated
        // as a sidecar file instead of bundled with the umbra scene data so the umbra scene can still be loaded by the umbra debugger. If
        // there is no scene descriptor file then its default configuration will be used.
        UmbraSceneDescriptor sceneDescriptor;
        if (!AZ::JsonSerializationUtils::LoadObjectFromFile(sceneDescriptor, sceneDescPath))
        {
            AZ_Warning(
                "UmbraSceneAssetBuilder",
                false,
                "Failed to locate umbra scene descriptor. Using default descriptor values for computing tome data.: %s.",
                sceneDescPath.c_str());
        }

        AZ_TracePrintf("AssetBuilder", "Load umbra scene data succeeded\n");

        // Set up tome computation parameters from scene descriptor.
        Umbra::ComputationParams params;
        params.setParam(Umbra::ComputationParams::COLLISION_RADIUS, sceneDescriptor.m_collisionRadius);
        params.setParam(Umbra::ComputationParams::SMALLEST_HOLE, sceneDescriptor.m_smallestHole);
        params.setParam(Umbra::ComputationParams::SMALLEST_OCCLUDER, sceneDescriptor.m_smallestOccluder);

        // Initiate processing the umbra scene.
        Umbra::LocalComputation::Params localParams;
        localParams.computationParams = &params;
        localParams.scene = scene;
        localParams.cacheSizeMegs = 100;

        Umbra::Computation* computation = LocalComputation::create(localParams);
        Umbra::Computation::Result result = computation->waitForResult();

        if (result.error != Umbra::Computation::ERROR_OK)
        {
            AZ_Error("UmbraSceneAssetBuilder", false, "Failed to compile scene data: %s", result.errorStr);
            computation->release();
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        if (!result.tomeSize || result.tome == NULL)
        {
            AZ_Error("UmbraSceneAssetBuilder", false, "Failed to generate tome data from scene data");
            computation->release();
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        AZ_TracePrintf("AssetBuilder", "Process umbra scene data succeeded\n");

        // Embed tome buffer directly into scene asset 
        UmbraSceneAsset sceneAsset;
        sceneAsset.m_objectDescriptors = sceneDescriptor.m_objectDescriptors;
        sceneAsset.m_tomeBuffer.assign(reinterpret_cast<uint8_t*>(result.tome), reinterpret_cast<uint8_t*>(result.tome) + result.tomeSize);
        computation->release();

        // Get file name from source file path, then replace the extension to generate product file name
        AZStd::string destFileName;
        AzFramework::StringFunc::Path::GetFullFileName(request.m_fullPath.c_str(), destFileName);
        AzFramework::StringFunc::Path::ReplaceExtension(destFileName, UmbraSceneAsset::Extension);

        // Construct product full path
        AZStd::string destPath;
        AzFramework::StringFunc::Path::ConstructFull(request.m_tempDirPath.c_str(), destFileName.c_str(), destPath, true);

        // Save the asset to binary format for production
        if (!AZ::Utils::SaveObjectToFile(destPath, AZ::DataStream::ST_BINARY, &sceneAsset, sceneAsset.GetType(), nullptr))
        {
            AZ_Error("UmbraSceneAssetBuilder", false, "Failed to save asset to cache");
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        AZ_TracePrintf("AssetBuilder", "Saved data to file %s \n", destPath.c_str());

        // Success. Save output product(s) to response
        AssetBuilderSDK::JobProduct jobProduct(destPath, sceneAsset.GetType(), 0);
        jobProduct.m_dependenciesHandled = true; // This builder has no dependencies to output.
        response.m_outputProducts.push_back(jobProduct);
        response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Success;
    }

    void UmbraSceneAssetBuilder::ShutDown()
    {
        m_isShuttingDown = true;
    }

} // namespace Umbra
