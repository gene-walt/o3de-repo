/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Clients/UmbraComponentController.h>

namespace Umbra
{
    void UmbraComponentConfig::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraComponentConfig>()
                ->Version(0)
                ;
        }
    }

    void UmbraComponentController::Reflect(AZ::ReflectContext* context)
    {
        UmbraComponentConfig::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraComponentController>()
                ->Version(0)
                ->Field("Configuration", &UmbraComponentController::m_configuration)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<UmbraComponentController>("UmbraComponentController", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraComponentController::m_configuration, "Configuration", "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ;
            }
        }
    }

    void UmbraComponentController::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        dependent.push_back(AZ_CRC_CE("TransformService"));
    }

    void UmbraComponentController::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("UmbraService"));
    }

    void UmbraComponentController::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("UmbraService"));
    }

    void UmbraComponentController::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("TransformService"));
    }

    UmbraComponentController::UmbraComponentController(const UmbraComponentConfig& config)
        : m_configuration(config)
    {
    }

    void UmbraComponentController::Activate(AZ::EntityId entityId)
    {
        m_entityId = entityId;
    }

    void UmbraComponentController::Deactivate()
    {
    }

    void UmbraComponentController::SetConfiguration(const UmbraComponentConfig& config)
    {
        m_configuration = config;
    }

    const UmbraComponentConfig& UmbraComponentController::GetConfiguration() const
    {
        return m_configuration;
    }
} // namespace Umbra
