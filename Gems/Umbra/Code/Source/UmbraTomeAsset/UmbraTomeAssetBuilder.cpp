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
#include <Umbra/UmbraTomeAsset/UmbraTomeAsset.h>
#include <UmbraTomeAsset/UmbraTomeAssetBuilder.h>

#include <umbra/optimizer/umbraLocalComputation.hpp>
#include <umbra/optimizer/umbraScene.hpp>
#include <umbra/runtime/umbraQuery.hpp>
#include <umbra/runtime/umbraTome.hpp>
#include <umbra/umbraInfo.hpp>

namespace Umbra
{
    namespace
    {
        [[maybe_unused]] const char* UmbraTomeAssetBuilderName = "UmbraTomeAssetBuilder";
        [[maybe_unused]] const char* UmbraTomeAssetBuilderJobKey = "Umbra Asset Builder";
        [[maybe_unused]] const char* UmbraTomeAssetBuilderPattern = "*.umbrascene"; // File represents umbra scene data exported from editor.
    } // namespace

    void UmbraTomeAssetBuilder::RegisterBuilder()
    {
        // setup builder descriptor
        AssetBuilderSDK::AssetBuilderDesc builderDescriptor;
        builderDescriptor.m_name = UmbraTomeAssetBuilderJobKey;
        builderDescriptor.m_patterns.push_back(
            AssetBuilderSDK::AssetBuilderPattern(UmbraTomeAssetBuilderPattern, AssetBuilderSDK::AssetBuilderPattern::PatternType::Wildcard));
        builderDescriptor.m_busId = azrtti_typeid<UmbraTomeAssetBuilder>();
        builderDescriptor.m_createJobFunction =
            AZStd::bind(&UmbraTomeAssetBuilder::CreateJobs, this, AZStd::placeholders::_1, AZStd::placeholders::_2);
        builderDescriptor.m_processJobFunction =
            AZStd::bind(&UmbraTomeAssetBuilder::ProcessJob, this, AZStd::placeholders::_1, AZStd::placeholders::_2);
        builderDescriptor.m_version = 0;

        BusConnect(builderDescriptor.m_busId);

        AssetBuilderSDK::AssetBuilderBus::Broadcast(&AssetBuilderSDK::AssetBuilderBusTraits::RegisterBuilderInformation, builderDescriptor);
    }

    UmbraTomeAssetBuilder::~UmbraTomeAssetBuilder()
    {
        BusDisconnect();
    }

    void UmbraTomeAssetBuilder::CreateJobs(const AssetBuilderSDK::CreateJobsRequest& request, AssetBuilderSDK::CreateJobsResponse& response)
    {
        if (m_isShuttingDown)
        {
            response.m_result = AssetBuilderSDK::CreateJobsResultCode::ShuttingDown;
            return;
        }

        for (const AssetBuilderSDK::PlatformInfo& platformInfo : request.m_enabledPlatforms)
        {
            AssetBuilderSDK::JobDescriptor descriptor;
            descriptor.m_jobKey = UmbraTomeAssetBuilderJobKey;
            descriptor.SetPlatformIdentifier(platformInfo.m_identifier.c_str());
            descriptor.m_critical = true;

            response.m_createJobOutputs.push_back(descriptor);
        }

        response.m_result = AssetBuilderSDK::CreateJobsResultCode::Success;
    }

    void UmbraTomeAssetBuilder::ProcessJob(const AssetBuilderSDK::ProcessJobRequest& request, AssetBuilderSDK::ProcessJobResponse& response)
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

        // TODO: Load umbra scene data from source file
        Umbra::Scene* scene = Umbra::Scene::create(request.m_fullPath.c_str());
        if (!scene)
        {
            AZ_Error("UmbraTomeAssetBuilder", false, "Failed to load umbra scene data source file %s", request.m_fullPath.c_str());
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        AZ_TracePrintf("AssetBuilder", "Load umbra scene data succeeded\n");

        // TODO: Process scene data and convert into tome asset
        // TODO: Make these settings configurable and include them with the source data
        static const float COLLISION_RADIUS = 0.125f;
        static const float SMALLEST_HOLE = 0.5f;
        static const float SMALLEST_OCCLUDER = 3.f;

        float collisionRadius = COLLISION_RADIUS;
        float smallestHole = SMALLEST_HOLE;
        float smallestOccluder = SMALLEST_OCCLUDER;

        Umbra::ComputationParams params;
        params.setParam(Umbra::ComputationParams::COLLISION_RADIUS, collisionRadius);
        params.setParam(Umbra::ComputationParams::SMALLEST_HOLE, smallestHole);
        params.setParam(Umbra::ComputationParams::SMALLEST_OCCLUDER, smallestOccluder);

        Umbra::LocalComputation::Params localParams;
        localParams.computationParams = &params;
        localParams.scene = scene;
        localParams.cacheSizeMegs = 100;

        Umbra::Computation* computation = LocalComputation::create(localParams);
        Umbra::Computation::Result result = computation->waitForResult();

        if (result.error != Umbra::Computation::ERROR_OK)
        {
            AZ_Error("UmbraTomeAssetBuilder", false, "Failed to compile scene data: %s", result.errorStr);
            computation->release();
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        if (!result.tomeSize || result.tome == NULL)
        {
            AZ_Error("UmbraTomeAssetBuilder", false, "Failed to get tome data from scene data");
            computation->release();
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        AZ_TracePrintf("AssetBuilder", "Process umbra scene data succeeded\n");

        // TODO: Store visibility data tome buffer into umbra asset
        UmbraTomeAsset tomeAsset;
        tomeAsset.m_tomeBuffer.assign(reinterpret_cast<uint8_t*>(result.tome), reinterpret_cast<uint8_t*>(result.tome) + result.tomeSize);
        computation->release();

        // Get file name from source file path, then replace the extension to generate product file name
        AZStd::string destFileName;
        AzFramework::StringFunc::Path::GetFullFileName(request.m_fullPath.c_str(), destFileName);
        AzFramework::StringFunc::Path::ReplaceExtension(destFileName, UmbraTomeAsset::Extension);

        // Construct product full path
        AZStd::string destPath;
        AzFramework::StringFunc::Path::ConstructFull(request.m_tempDirPath.c_str(), destFileName.c_str(), destPath, true);

        // Save the asset to binary format for production
        if (!AZ::Utils::SaveObjectToFile(destPath, AZ::DataStream::ST_BINARY, &tomeAsset, tomeAsset.GetType(), nullptr))
        {
            AZ_Error("UmbraTomeAssetBuilder", false, "Failed to save asset to cache") response.m_resultCode =
                AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        AZ_TracePrintf("AssetBuilder", "Saved data to file %s \n", destPath.c_str());

        // Success. Save output product(s) to response
        AssetBuilderSDK::JobProduct jobProduct(destPath, tomeAsset.GetType(), 0);
        jobProduct.m_dependenciesHandled = true; // This builder has no dependencies to output.
        response.m_outputProducts.push_back(jobProduct);
        response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Success;
    }

    void UmbraTomeAssetBuilder::ShutDown()
    {
        m_isShuttingDown = true;
    }

} // namespace Umbra
