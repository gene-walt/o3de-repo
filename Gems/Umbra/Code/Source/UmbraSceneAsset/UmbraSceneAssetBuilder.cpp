/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AssetBuilderSDK/SerializationDependencies.h>
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Serialization/Json/JsonUtils.h>
#include <AzCore/Serialization/Utils.h>
#include <AzCore/std/algorithm.h>
#include <AzCore/std/smart_ptr/make_shared.h>
#include <AzCore/Utils/Utils.h>
#include <AzFramework/IO/LocalFileIO.h>
#include <AzFramework/StringFunc/StringFunc.h>
#include <AzToolsFramework/API/EditorAssetSystemAPI.h>
#include <Umbra/UmbraSceneAsset/UmbraSceneAsset.h>
#include <UmbraSceneAsset/UmbraSceneAssetBuilder.h>
#include <UmbraSceneAsset/UmbraSceneDescriptor.h>

#include <optimizer/umbraLocalComputation.hpp>
#include <optimizer/umbraScene.hpp>
#include <runtime/umbraQuery.hpp>
#include <runtime/umbraTome.hpp>
#include <umbraInfo.hpp>

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
        builderDescriptor.m_version = 1;

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

        AZ_Trace("AssetBuilder", "Load umbra scene data succeeded\n");

        // Set up tome computation parameters from scene descriptor.
        Umbra::ComputationParams params;
        params.setParam(Umbra::ComputationParams::COLLISION_RADIUS, sceneDescriptor.m_sceneSettings.m_collisionRadius);
        params.setParam(Umbra::ComputationParams::SMALLEST_HOLE, sceneDescriptor.m_sceneSettings.m_smallestHole);
        params.setParam(Umbra::ComputationParams::SMALLEST_OCCLUDER, sceneDescriptor.m_sceneSettings.m_smallestOccluder);

        // Update computation parameters with settings overridden by each view
        for (const auto& sceneSettingsByViewPair : sceneDescriptor.m_sceneSettingsByView)
        {
            params.setVolumeParam(
                sceneSettingsByViewPair.first,
                Umbra::ComputationParams::COLLISION_RADIUS,
                sceneSettingsByViewPair.second.m_collisionRadius);

            params.setVolumeParam(
                sceneSettingsByViewPair.first,
                Umbra::ComputationParams::SMALLEST_HOLE,
                sceneSettingsByViewPair.second.m_smallestHole);

            params.setVolumeParam(
                sceneSettingsByViewPair.first,
                Umbra::ComputationParams::SMALLEST_OCCLUDER,
                sceneSettingsByViewPair.second.m_smallestOccluder);
        }

        // Initiate processing the umbra scene.
        Umbra::LocalComputation::Params localParams;
        localParams.computationParams = &params;
        localParams.scene = scene;
        localParams.cacheSizeMegs = 100;
        localParams.numThreads = -1;

        const AZStd::string tempPath = (AZ::IO::FixedMaxPath(AZ::Utils::GetProjectUserPath()) / "umbracache").String();
        localParams.tempPath = tempPath.c_str();

        // Attempt to read the license key from an environment variable instead of relying on license files in the working directory or bin
        // folder.
        char licenseEnvBuffer[256]{};
        if (auto licenseEnvOutcome = AZ::Utils::GetEnv(licenseEnvBuffer, "UMBRA_LICENSE_KEY"); licenseEnvOutcome)
        {
            localParams.licenseKey = licenseEnvBuffer;
        }

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

        AZ_Trace("AssetBuilder", "Process umbra scene data succeeded\n");

        // Embed tome buffer directly into scene asset 
        UmbraSceneAsset sceneAsset;
        sceneAsset.m_objectDescriptors = sceneDescriptor.m_objectDescriptors;
        sceneAsset.m_tomeBuffer.assign(reinterpret_cast<uint8_t*>(result.tome), reinterpret_cast<uint8_t*>(result.tome) + result.tomeSize);
        computation->release();

        // Get file name from source file path, then replace the extension to generate product file name
        AZStd::string sceneAssetFileName;
        AzFramework::StringFunc::Path::GetFullFileName(request.m_fullPath.c_str(), sceneAssetFileName);
        AzFramework::StringFunc::Path::ReplaceExtension(sceneAssetFileName, UmbraSceneAsset::Extension);

        // Construct product full path
        AZStd::string sceneAssetPath;
        AzFramework::StringFunc::Path::ConstructFull(request.m_tempDirPath.c_str(), sceneAssetFileName.c_str(), sceneAssetPath, true);

        // Save the asset to binary format for production
        if (AZ::Utils::SaveObjectToFile(sceneAssetPath, AZ::DataStream::ST_BINARY, &sceneAsset, sceneAsset.GetType(), nullptr))
        {
            AssetBuilderSDK::JobProduct jobProduct;
            jobProduct.m_dependenciesHandled = true; // This builder has no dependencies to output.
            jobProduct.m_productFileName = sceneAssetPath;
            jobProduct.m_productAssetType = sceneAsset.GetType();
            jobProduct.m_productSubID = 0;
            response.m_outputProducts.emplace_back(AZStd::move(jobProduct));
            AZ_Trace("AssetBuilder", "Saved data to file %s \n", sceneAssetPath.c_str());
        }
        else
        {
            AZ_Error("UmbraSceneAssetBuilder", false, "Failed to save asset to cache");
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        // Save raw dumb file for other systems and debugging
        AZStd::string tomePath = sceneAssetPath;
        AzFramework::StringFunc::Path::ReplaceExtension(tomePath, "tome");
        if (AZ::Utils::SaveStreamToFile(tomePath, sceneAsset.m_tomeBuffer))
        {
            AssetBuilderSDK::JobProduct jobProduct;
            jobProduct.m_dependenciesHandled = true; // This builder has no dependencies to output.
            jobProduct.m_productFileName = tomePath;
            // We need a unique asset type identifier for the tone file since it is not directly registered or used by any other system
            jobProduct.m_productAssetType = AZ::Uuid::CreateString("{956C25FF-AEF1-4E3D-A1A5-85675731A782}");
            jobProduct.m_productSubID = 1;
            response.m_outputProducts.emplace_back(AZStd::move(jobProduct));
            AZ_Trace("AssetBuilder", "Saved raw tome to file %s \n", tomePath.c_str());
        }
        else
        {
            AZ_Error("UmbraSceneAssetBuilder", false, "Failed to save raw tome to cache");
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Success;
    }

    void UmbraSceneAssetBuilder::ShutDown()
    {
        m_isShuttingDown = true;
    }

} // namespace Umbra
