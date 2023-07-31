/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <UmbraViewVolumeComponent/EditorUmbraViewVolumeComponent.h>
#include <UmbraViewVolumeComponent/EditorUmbraViewVolumeComponentUtil.h>

namespace Umbra
{
    void EditorUmbraViewVolumeComponent::Reflect(AZ::ReflectContext* context)
    {
        BaseClass::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<EditorUmbraViewVolumeComponent, BaseClass>()
                ->Version(0)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<EditorUmbraViewVolumeComponent>(
                    "UmbraViewVolumeComponent", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::Category, "Umbra")
                        ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                        ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Icons/Components/Viewport/Component_Placeholder.svg")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->Attribute(AZ::Edit::Attributes::HelpPageURL, "")
                        ->Attribute(AZ::Edit::Attributes::PrimaryAssetType, AZ::AzTypeInfo<Umbra::UmbraTomeAsset>::Uuid())
                    ;

                editContext->Class<UmbraViewVolumeComponentConfig>(
                    "UmbraViewVolumeComponentConfig", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Hide)
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->ConstantProperty("EditorUmbraViewVolumeComponentTypeId", BehaviorConstant(AZ::Uuid(EditorUmbraViewVolumeComponentTypeId)))
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation)
                ;
        }
    }

    EditorUmbraViewVolumeComponent::EditorUmbraViewVolumeComponent(const UmbraViewVolumeComponentConfig& config)
        : BaseClass(config)
    {
    }

    void EditorUmbraViewVolumeComponent::Activate()
    {
        BaseClass::Activate();
        UmbraViewVolumeComponentNotificationBus::Handler::BusConnect(GetEntityId());
    }

    void EditorUmbraViewVolumeComponent::Deactivate()
    {
        UmbraViewVolumeComponentNotificationBus::Handler::BusDisconnect();
        BaseClass::Deactivate();
    }
} // namespace Umbra
