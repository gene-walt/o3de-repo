/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <UmbraSceneAsset/UmbraSceneAssetBuilder.h>
#include <UmbraSceneAsset/UmbraSceneAssetBuilderComponent.h>

namespace Umbra
{
    void UmbraSceneAssetBuilderComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraSceneAssetBuilderComponent, AZ::Component>()
                ->Version(0)
                ->Attribute(AZ::Edit::Attributes::SystemComponentTags, AZStd::vector<AZ::Crc32>({ AssetBuilderSDK::ComponentTags::AssetBuilder }))
                ;
        }
    }

    UmbraSceneAssetBuilderComponent::UmbraSceneAssetBuilderComponent()
    {
    }

    UmbraSceneAssetBuilderComponent::~UmbraSceneAssetBuilderComponent()
    {
    }

    void UmbraSceneAssetBuilderComponent::Activate()
    {
        // Register asset workers
        m_assetWorkers.emplace_back(MakeAssetBuilder<UmbraSceneAssetBuilder>());
    }

    void UmbraSceneAssetBuilderComponent::Deactivate()
    {
        m_assetWorkers.clear();
    }
} // namespace Umbra
