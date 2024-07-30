/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <UmbraSceneAsset/UmbraSceneDescriptor.h>

namespace Umbra
{
    const char* UmbraSceneDescriptor::Extension = "umbrascenedesc";

    void UmbraSceneDescriptor::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->RegisterGenericType<AZStd::unordered_map<uint32_t, UmbraSceneSettings>>();
            serializeContext->RegisterGenericType<AZStd::vector<UmbraObjectDescriptor>>();

            serializeContext->Class<UmbraSceneDescriptor>()
                ->Version(0)
                ->Field("sceneSettings", &UmbraSceneDescriptor::m_sceneSettings)
                ->Field("sceneSettingsByView", &UmbraSceneDescriptor::m_sceneSettingsByView)
                ->Field("objectDescriptors", &UmbraSceneDescriptor::m_objectDescriptors)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<UmbraSceneDescriptor>("UmbraSceneDescriptor", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Show)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneDescriptor::m_sceneSettings, "Scene Settings", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneDescriptor::m_sceneSettingsByView, "Scene Settings By View", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneDescriptor::m_objectDescriptors, "Object Descriptors", "")
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<UmbraSceneDescriptor>("UmbraSceneDescriptor")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "Umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Constructor()
                ->Constructor<const UmbraSceneDescriptor&>()
                ->Property("sceneSettings", BehaviorValueProperty(&UmbraSceneDescriptor::m_sceneSettings))
                ->Property("sceneSettingsByView", BehaviorValueProperty(&UmbraSceneDescriptor::m_sceneSettingsByView))
                ->Property("objectDescriptors", BehaviorValueProperty(&UmbraSceneDescriptor::m_objectDescriptors))
                ;
        }
    }
} // namespace Umbra
