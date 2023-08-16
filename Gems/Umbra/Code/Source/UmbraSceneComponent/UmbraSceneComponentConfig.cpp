/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/Asset/AssetSerializer.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Umbra/UmbraSceneComponent/UmbraSceneComponentConfig.h>

namespace Umbra
{
    void UmbraSceneComponentConfig::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraSceneComponentConfig, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("sceneSettings", &UmbraSceneComponentConfig::m_sceneSettings)
                ->Field("sceneAsset", &UmbraSceneComponentConfig::m_sceneAsset)
                ->Field("onlyStaticObjects", &UmbraSceneComponentConfig::m_onlyStaticObjects)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<UmbraSceneComponentConfig>("UmbraSceneComponentConfig", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Show)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_sceneSettings, "Scene Settings", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_sceneAsset, "Scene Asset", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_onlyStaticObjects, "Only Static Objects", "")
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<UmbraSceneComponentConfig>("UmbraSceneComponentConfig")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Constructor()
                ->Constructor<const UmbraSceneComponentConfig&>()
                ->Property("sceneSettings", BehaviorValueProperty(&UmbraSceneComponentConfig::m_sceneSettings))
                ->Property("sceneAsset", BehaviorValueProperty(&UmbraSceneComponentConfig::m_sceneAsset))
                ->Property("onlyStaticObjects", BehaviorValueProperty(&UmbraSceneComponentConfig::m_onlyStaticObjects))
                ;
        }
    }
} // namespace Umbra
