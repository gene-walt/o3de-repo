/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <UmbraViewVolumeComponent/UmbraViewVolumeComponentController.h>

namespace Umbra
{
    void UmbraViewVolumeComponentController::Reflect(AZ::ReflectContext* context)
    {
        UmbraViewVolumeComponentConfig::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraViewVolumeComponentController>()
                ->Version(0)
                ->Field("Configuration", &UmbraViewVolumeComponentController::m_configuration)
                ;
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<UmbraViewVolumeComponentRequestBus>("UmbraViewVolumeComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ;
        }
    }

    void UmbraViewVolumeComponentController::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("UmbraViewVolumeService"));
    }

    void UmbraViewVolumeComponentController::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("UmbraViewVolumeService"));
    }

    void UmbraViewVolumeComponentController::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    UmbraViewVolumeComponentController::UmbraViewVolumeComponentController(const UmbraViewVolumeComponentConfig& config)
        : m_configuration(config)
    {
    }

    void UmbraViewVolumeComponentController::Activate(AZ::EntityId entityId)
    {
        m_entityId = entityId;
        UmbraViewVolumeComponentRequestBus::Handler::BusConnect(m_entityId);
    }

    void UmbraViewVolumeComponentController::Deactivate()
    {
        UmbraViewVolumeComponentRequestBus::Handler::BusDisconnect();
        m_entityId = AZ::EntityId(AZ::EntityId::InvalidEntityId);
    }

    void UmbraViewVolumeComponentController::SetConfiguration(const UmbraViewVolumeComponentConfig& config)
    {
        m_configuration = config;
    }

    const UmbraViewVolumeComponentConfig& UmbraViewVolumeComponentController::GetConfiguration() const
    {
        return m_configuration;
    }
} // namespace Umbra