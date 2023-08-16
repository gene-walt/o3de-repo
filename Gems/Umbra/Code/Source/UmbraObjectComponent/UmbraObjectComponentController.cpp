/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
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
            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<UmbraObjectComponentController>("UmbraObjectComponentController", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraObjectComponentController::m_configuration, "Configuration", "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<UmbraObjectComponentRequestBus>("UmbraObjectComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Event("SetCanOcclude", &UmbraObjectComponentRequestBus::Events::SetCanOcclude)
                ->Event("GetCanOcclude", &UmbraObjectComponentRequestBus::Events::GetCanOcclude)
                ->Event("SetCanBeOccluded", &UmbraObjectComponentRequestBus::Events::SetCanBeOccluded)
                ->Event("GetCanBeOccluded", &UmbraObjectComponentRequestBus::Events::GetCanBeOccluded)
                ;
        }
    }

    void UmbraObjectComponentController::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC_CE("UmbraObjectService"));
    }

    void UmbraObjectComponentController::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC_CE("UmbraObjectService"));
        services.push_back(AZ_CRC_CE("UmbraSceneService"));
        services.push_back(AZ_CRC_CE("UmbraViewVolumeService"));
    }

    void UmbraObjectComponentController::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& services)
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

    void UmbraObjectComponentController::SetCanOcclude(bool canOcclude)
    {
        m_configuration.m_canOcclude = canOcclude;
    }

    bool UmbraObjectComponentController::GetCanOcclude() const
    {
        return m_configuration.m_canOcclude;
    }

    void UmbraObjectComponentController::SetCanBeOccluded(bool canBeOccluded)
    {
        m_configuration.m_canBeOccluded = canBeOccluded;
    }

    bool UmbraObjectComponentController::GetCanBeOccluded() const
    {
        return m_configuration.m_canBeOccluded;
    }
} // namespace Umbra