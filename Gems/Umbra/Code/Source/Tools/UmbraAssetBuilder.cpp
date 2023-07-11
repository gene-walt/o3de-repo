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
#include <Clients/UmbraAsset.h>
#include <Tools/UmbraAssetBuilder.h>

namespace Umbra
{
    namespace
    {
        [[maybe_unused]] const char* UmbraAssetBuilderName = "UmbraAssetBuilder";
        [[maybe_unused]] const char* UmbraAssetBuilderJobKey = "Umbra Asset Builder";
        [[maybe_unused]] const char* UmbraAssetBuilderPattern = "*.umbrascene"; // File represents umbra scene data exported from editor.
    } // namespace

    void UmbraAssetBuilder::RegisterBuilder()
    {
        // setup builder descriptor
        AssetBuilderSDK::AssetBuilderDesc builderDescriptor;
        builderDescriptor.m_name = UmbraAssetBuilderJobKey;
        builderDescriptor.m_patterns.push_back(
            AssetBuilderSDK::AssetBuilderPattern(UmbraAssetBuilderPattern, AssetBuilderSDK::AssetBuilderPattern::PatternType::Wildcard));
        builderDescriptor.m_busId = azrtti_typeid<UmbraAssetBuilder>();
        builderDescriptor.m_createJobFunction =
            AZStd::bind(&UmbraAssetBuilder::CreateJobs, this, AZStd::placeholders::_1, AZStd::placeholders::_2);
        builderDescriptor.m_processJobFunction =
            AZStd::bind(&UmbraAssetBuilder::ProcessJob, this, AZStd::placeholders::_1, AZStd::placeholders::_2);
        builderDescriptor.m_version = 0;

        BusConnect(builderDescriptor.m_busId);

        AssetBuilderSDK::AssetBuilderBus::Broadcast(&AssetBuilderSDK::AssetBuilderBusTraits::RegisterBuilderInformation, builderDescriptor);
    }

    UmbraAssetBuilder::~UmbraAssetBuilder()
    {
        BusDisconnect();
    }

    void UmbraAssetBuilder::CreateJobs(const AssetBuilderSDK::CreateJobsRequest& request, AssetBuilderSDK::CreateJobsResponse& response)
    {
        if (m_isShuttingDown)
        {
            response.m_result = AssetBuilderSDK::CreateJobsResultCode::ShuttingDown;
            return;
        }

        for (const AssetBuilderSDK::PlatformInfo& platformInfo : request.m_enabledPlatforms)
        {
            AssetBuilderSDK::JobDescriptor descriptor;
            descriptor.m_jobKey = UmbraAssetBuilderJobKey;
            descriptor.SetPlatformIdentifier(platformInfo.m_identifier.c_str());
            descriptor.m_critical = true;

            response.m_createJobOutputs.push_back(descriptor);
        }

        response.m_result = AssetBuilderSDK::CreateJobsResultCode::Success;
    }

    void UmbraAssetBuilder::ProcessJob(const AssetBuilderSDK::ProcessJobRequest& request, AssetBuilderSDK::ProcessJobResponse& response)
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
        bool loaded = false;
        if (!loaded)
        {
            AZ_Error("UmbraAssetBuilder", false, "Failed to load umbra scene data source file %s", request.m_fullPath.c_str());
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        AZ_TracePrintf("AssetBuilder", "Load umbra scene data succeeded\n");

        // TODO: Compute visibility data from scene
        bool processed = false;
        if (!processed)
        {
            AZ_Error("UmbraAssetBuilder", false, "Failed to process umbra scene data %s", request.m_fullPath.c_str());
            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        AZ_TracePrintf("AssetBuilder", "Process umbra scene data succeeded\n");

        // TODO: Store visibility data tome buffer into umbra asset
        UmbraAsset umbraAsset;

        // Get file name from source file path, then replace the extension to generate product file name
        AZStd::string destFileName;
        AzFramework::StringFunc::Path::GetFullFileName(request.m_fullPath.c_str(), destFileName);
        AzFramework::StringFunc::Path::ReplaceExtension(destFileName, UmbraAsset::Extension);

        // Construct product full path
        AZStd::string destPath;
        AzFramework::StringFunc::Path::ConstructFull(request.m_tempDirPath.c_str(), destFileName.c_str(), destPath, true);

        // Save the asset to binary format for production
        bool result = AZ::Utils::SaveObjectToFile(destPath, AZ::DataStream::ST_BINARY, &umbraAsset, umbraAsset.GetType(), nullptr);
        if (!result)
        {
            AZ_Error("UmbraAssetBuilder", false, "Failed to save asset to cache") response.m_resultCode =
                AssetBuilderSDK::ProcessJobResult_Failed;
            return;
        }

        AZ_TracePrintf("AssetBuilder", "Saved data to file %s \n", destPath.c_str());

        // Success. Save output product(s) to response
        AssetBuilderSDK::JobProduct jobProduct(destPath, umbraAsset.GetType(), 0);
        jobProduct.m_dependenciesHandled = true; // This builder has no dependencies to output.
        response.m_outputProducts.push_back(jobProduct);
        response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Success;
    }

    void UmbraAssetBuilder::ShutDown()
    {
        m_isShuttingDown = true;
    }

} // namespace Umbra
