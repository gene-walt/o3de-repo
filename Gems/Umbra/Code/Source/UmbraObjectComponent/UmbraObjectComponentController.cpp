/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <UmbraObjectComponent/UmbraObjectComponentController.h>

namespace Umbra
{
    void UmbraObjectComponentController::Reflect(AZ::ReflectContext* context)
    {
        UmbraObjectComponentConfig::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraObjectComponentController>()
                ->Version(0)
                ->Field("Configuration", &UmbraObjectComponentController::m_configuration)
                ;
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<UmbraObjectComponentRequestBus>("UmbraObjectComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ;
        }
    }

    void UmbraObjectComponentController::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("UmbraObjectService"));
    }

    void UmbraObjectComponentController::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("UmbraObjectService"));
    }

    void UmbraObjectComponentController::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    UmbraObjectComponentController::UmbraObjectComponentController(const UmbraObjectComponentConfig& config)
        : m_configuration(config)
    {
    }

    void UmbraObjectComponentController::Activate(AZ::EntityId entityId)
    {
        m_entityId = entityId;
        UmbraObjectComponentRequestBus::Handler::BusConnect(m_entityId);
    }

    void UmbraObjectComponentController::Deactivate()
    {
        UmbraObjectComponentRequestBus::Handler::BusDisconnect();
        m_entityId = AZ::EntityId(AZ::EntityId::InvalidEntityId);
    }

    void UmbraObjectComponentController::SetConfiguration(const UmbraObjectComponentConfig& config)
    {
        m_configuration = config;
    }

    const UmbraObjectComponentConfig& UmbraObjectComponentController::GetConfiguration() const
    {
        return m_configuration;
    }
} // namespace Umbra