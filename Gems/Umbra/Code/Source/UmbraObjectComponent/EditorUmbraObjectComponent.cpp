/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <UmbraObjectComponent/EditorUmbraObjectComponent.h>
#include <UmbraObjectComponent/EditorUmbraObjectComponentUtil.h>

namespace Umbra
{
    void EditorUmbraObjectComponent::Reflect(AZ::ReflectContext* context)
    {
        BaseClass::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<EditorUmbraObjectComponent, BaseClass>()
                ->Version(0)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<EditorUmbraObjectComponent>(
                    "UmbraObjectComponent", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::Category, "Umbra")
                        ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                        ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Icons/Components/Viewport/Component_Placeholder.svg")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->Attribute(AZ::Edit::Attributes::HelpPageURL, "")
                        ->Attribute(AZ::Edit::Attributes::PrimaryAssetType, AZ::AzTypeInfo<Umbra::UmbraTomeAsset>::Uuid())
                    ;

                editContext->Class<UmbraObjectComponentConfig>(
                    "UmbraObjectComponentConfig", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Hide)
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->ConstantProperty("EditorUmbraObjectComponentTypeId", BehaviorConstant(AZ::Uuid(EditorUmbraObjectComponentTypeId)))
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation)
                ;
        }
    }

    EditorUmbraObjectComponent::EditorUmbraObjectComponent(const UmbraObjectComponentConfig& config)
        : BaseClass(config)
    {
    }

    void EditorUmbraObjectComponent::Activate()
    {
        BaseClass::Activate();
        UmbraObjectComponentNotificationBus::Handler::BusConnect(GetEntityId());
    }

    void EditorUmbraObjectComponent::Deactivate()
    {
        UmbraObjectComponentNotificationBus::Handler::BusDisconnect();
        BaseClass::Deactivate();
    }
} // namespace Umbra
