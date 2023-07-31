/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <UmbraLevelComponent/UmbraLevelComponentController.h>

namespace Umbra
{
    void UmbraLevelComponentController::Reflect(AZ::ReflectContext* context)
    {
        UmbraLevelComponentConfig::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraLevelComponentController>()
                ->Version(0)
                ->Field("Configuration", &UmbraLevelComponentController::m_configuration)
                ;
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<UmbraLevelComponentRequestBus>("UmbraLevelComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ;
        }
    }

    void UmbraLevelComponentController::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("UmbraLevelService"));
    }

    void UmbraLevelComponentController::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("UmbraLevelService"));
    }

    void UmbraLevelComponentController::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    UmbraLevelComponentController::UmbraLevelComponentController(const UmbraLevelComponentConfig& config)
        : m_configuration(config)
    {
    }

    void UmbraLevelComponentController::Activate(AZ::EntityId entityId)
    {
        m_entityId = entityId;
        UmbraLevelComponentRequestBus::Handler::BusConnect(m_entityId);
    }

    void UmbraLevelComponentController::Deactivate()
    {
        UmbraLevelComponentRequestBus::Handler::BusDisconnect();
        m_entityId = AZ::EntityId(AZ::EntityId::InvalidEntityId);
    }

    void UmbraLevelComponentController::SetConfiguration(const UmbraLevelComponentConfig& config)
    {
        m_configuration = config;
    }

    const UmbraLevelComponentConfig& UmbraLevelComponentController::GetConfiguration() const
    {
        return m_configuration;
    }

    void UmbraLevelComponentController::OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
    }

    void UmbraLevelComponentController::OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
    }

    void UmbraLevelComponentController::OnAssetError(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
    }

    void UmbraLevelComponentController::OnAssetReloadError(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
    }
} // namespace Umbra