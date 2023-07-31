/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * its licensors.
 */

#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <UmbraTomeAsset/UmbraTomeAssetBuilder.h>
#include <UmbraTomeAsset/UmbraTomeAssetBuilderComponent.h>

namespace Umbra
{
    void UmbraTomeAssetBuilderComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraTomeAssetBuilderComponent, AZ::Component>()
                ->Version(0)
                ->Attribute(AZ::Edit::Attributes::SystemComponentTags, AZStd::vector<AZ::Crc32>({ AssetBuilderSDK::ComponentTags::AssetBuilder }))
                ;
        }
    }

    UmbraTomeAssetBuilderComponent::UmbraTomeAssetBuilderComponent()
    {
    }

    UmbraTomeAssetBuilderComponent::~UmbraTomeAssetBuilderComponent()
    {
    }

    void UmbraTomeAssetBuilderComponent::Activate()
    {
        // Register asset workers
        m_assetWorkers.emplace_back(MakeAssetBuilder<UmbraTomeAssetBuilder>());
    }

    void UmbraTomeAssetBuilderComponent::Deactivate()
    {
        m_assetWorkers.clear();
    }
} // namespace Umbra
