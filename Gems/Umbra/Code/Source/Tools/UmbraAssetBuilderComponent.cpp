/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * its licensors.
 */

#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Tools/UmbraAssetBuilderComponent.h>
#include <Tools/UmbraAssetBuilder.h>

namespace Umbra
{
    void UmbraAssetBuilderComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraAssetBuilderComponent, AZ::Component>()
                ->Version(0)
                ->Attribute(AZ::Edit::Attributes::SystemComponentTags, AZStd::vector<AZ::Crc32>({ AssetBuilderSDK::ComponentTags::AssetBuilder }))
                ;
        }
    }

    UmbraAssetBuilderComponent::UmbraAssetBuilderComponent()
    {
    }

    UmbraAssetBuilderComponent::~UmbraAssetBuilderComponent()
    {
    }

    void UmbraAssetBuilderComponent::Activate()
    {
        // Register asset workers
        m_assetWorkers.emplace_back(MakeAssetBuilder<UmbraAssetBuilder>());
    }

    void UmbraAssetBuilderComponent::Deactivate()
    {
        m_assetWorkers.clear();
    }
} // namespace Umbra
