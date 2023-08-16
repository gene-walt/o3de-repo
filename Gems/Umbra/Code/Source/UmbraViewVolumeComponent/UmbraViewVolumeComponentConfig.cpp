/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Umbra/UmbraViewVolumeComponent/UmbraViewVolumeComponentConfig.h>

namespace Umbra
{
    void UmbraViewVolumeComponentConfig::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraViewVolumeComponentConfig>()
                ->Version(0)
                ->Field("overrideSceneSettings", &UmbraViewVolumeComponentConfig::m_overrideSceneSettings)
                ->Field("sceneSettings", &UmbraViewVolumeComponentConfig::m_sceneSettings)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<UmbraViewVolumeComponentConfig>("UmbraViewVolumeComponentConfig", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Show)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraViewVolumeComponentConfig::m_overrideSceneSettings, "Override Scene Settings", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraViewVolumeComponentConfig::m_sceneSettings, "Scene Settings", "")
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<UmbraViewVolumeComponentConfig>("UmbraViewVolumeComponentConfig")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Constructor()
                ->Constructor<const UmbraViewVolumeComponentConfig&>()
                ->Property("overrideSceneSettings", BehaviorValueProperty(&UmbraViewVolumeComponentConfig::m_overrideSceneSettings))
                ->Property("sceneSettings", BehaviorValueProperty(&UmbraViewVolumeComponentConfig::m_sceneSettings))
                ;
        }
    }
} // namespace Umbra
