/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <UmbraSystemComponent/EditorUmbraSystemComponent.h>
#include <UmbraSystemComponent/EditorUmbraSystemComponentUtil.h>
#include <UmbraSceneAsset/UmbraSceneDescriptor.h>

namespace Umbra
{
    void EditorUmbraSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        BaseClass::Reflect(context);
        UmbraSceneDescriptor::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<EditorUmbraSystemComponent, BaseClass>()
                ->Version(0)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<EditorUmbraSystemComponent>(
                    "Umbra System", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::Category, "Umbra")
                        ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                        ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Icons/Components/Viewport/Component_Placeholder.svg")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("System"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->Attribute(AZ::Edit::Attributes::HelpPageURL, "")
                        ->Attribute(AZ::Edit::Attributes::PrimaryAssetType, AZ::AzTypeInfo<Umbra::UmbraSceneAsset>::Uuid())
                    ;

                editContext->Class<UmbraSystemComponentController>(
                    "Umbra System Component Controller", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSystemComponentController::m_configuration, "Configuration", "")
                        ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ;

                editContext->Class<UmbraSystemComponentConfig>(
                    "Umbra System Component Config", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Hide)
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->ConstantProperty("EditorUmbraSystemComponentTypeId", BehaviorConstant(AZ::Uuid(EditorUmbraSystemComponentTypeId)))
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation)
                ;
        }
    }

    EditorUmbraSystemComponent::EditorUmbraSystemComponent(const UmbraSystemComponentConfig& config)
        : BaseClass(config)
    {
    }

    void EditorUmbraSystemComponent::Activate()
    {
        BaseClass::Activate();
        UmbraSystemComponentNotificationBus::Handler::BusConnect(GetEntityId());
    }

    void EditorUmbraSystemComponent::Deactivate()
    {
        UmbraSystemComponentNotificationBus::Handler::BusDisconnect();
        BaseClass::Deactivate();
    }
} // namespace Umbra
