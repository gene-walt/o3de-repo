/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Tools/EditorUmbraComponent.h>

namespace Umbra
{
    void EditorUmbraComponent::Reflect(AZ::ReflectContext* context)
    {
        BaseClass::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<EditorUmbraComponent, BaseClass>()
                ->Version(0)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<EditorUmbraComponent>(
                    "Umbra", "The Umbra component")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Graphics")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                    ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Icons/Components/Viewport/Component_Placeholder.svg")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::HelpPageURL, "")
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->ConstantProperty(UmbraEditorSystemComponentTypeId, BehaviorConstant(AZ::Uuid(UmbraEditorSystemComponentTypeId)))
                ->Attribute(AZ::Script::Attributes::Module, "render")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation)
                ;
        }
    }

    EditorUmbraComponent::EditorUmbraComponent()
    {
    }

    EditorUmbraComponent::EditorUmbraComponent(const UmbraComponentConfig& config)
        : BaseClass(config)
    {
    }

    void EditorUmbraComponent::Activate()
    {
        BaseClass::Activate();
    }

    void EditorUmbraComponent::Deactivate()
    {
        BaseClass::Deactivate();
    }
} // namespace Umbra
