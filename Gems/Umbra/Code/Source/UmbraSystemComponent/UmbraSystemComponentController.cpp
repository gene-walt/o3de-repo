/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/Asset/AssetManager.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <UmbraSystemComponent/UmbraSystemComponentController.h>

namespace Umbra
{
    void UmbraSystemComponentController::Reflect(AZ::ReflectContext* context)
    {
        UmbraSystemComponentConfig::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraSystemComponentController>()
                ->Version(0)
                ->Field("Configuration", &UmbraSystemComponentController::m_configuration)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<UmbraSystemComponentController>("UmbraSystemComponentController", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSystemComponentController::m_configuration, "Configuration", "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<UmbraSystemComponentRequestBus>("UmbraSystemComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "Umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ;
        }
    }

    void UmbraSystemComponentController::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC_CE("UmbraSystemService"));
    }

    void UmbraSystemComponentController::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC_CE("UmbraSystemService"));
    }

    void UmbraSystemComponentController::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC("AssetDatabaseService"));
        services.push_back(AZ_CRC("AssetCatalogService"));
    }

    UmbraSystemComponentController::UmbraSystemComponentController(const UmbraSystemComponentConfig& config)
        : m_configuration(config)
    {
    }

    void UmbraSystemComponentController::Activate(AZ::EntityId entityId)
    {
        m_entityId = entityId;
        m_sceneAssetHandler.reset(aznew UmbraSceneAssetHandler(UmbraSceneAsset::DisplayName, UmbraSceneAsset::Group, UmbraSceneAsset::Extension));
        m_sceneAssetHandler->Register();

        UmbraSystemComponentRequestBus::Handler::BusConnect(m_entityId);
    }

    void UmbraSystemComponentController::Deactivate()
    {
        UmbraSystemComponentRequestBus::Handler::BusDisconnect();
        m_sceneAssetHandler->Unregister();
        m_sceneAssetHandler.reset();
        m_entityId = AZ::EntityId(AZ::EntityId::InvalidEntityId);
    }

    void UmbraSystemComponentController::SetConfiguration(const UmbraSystemComponentConfig& config)
    {
        m_configuration = config;
    }

    const UmbraSystemComponentConfig& UmbraSystemComponentController::GetConfiguration() const
    {
        return m_configuration;
    }
} // namespace Umbra