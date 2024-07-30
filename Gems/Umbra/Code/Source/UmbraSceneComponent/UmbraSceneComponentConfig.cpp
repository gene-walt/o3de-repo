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
                ->Field("sceneAsset", &UmbraSceneComponentConfig::m_sceneAsset)
                ->Field("exportStaticObjectsOnly", &UmbraSceneComponentConfig::m_exportStaticObjectsOnly)
                ->Field("additionalMemoryPerQuery", &UmbraSceneComponentConfig::m_additionalMemoryPerQuery)
                ->Field("sceneSettings", &UmbraSceneComponentConfig::m_sceneSettings)
                ->Field("pauseDebugInfo", &UmbraSceneComponentConfig::m_pauseDebugInfo)
                ->Field("renderDebugInfo", &UmbraSceneComponentConfig::m_renderDebugInfo)
                ->Field("renderDebugBounds", &UmbraSceneComponentConfig::m_renderDebugBounds)
                ->Field("renderDebugBuffers", &UmbraSceneComponentConfig::m_renderDebugBuffers)
                ->Field("renderDebugFrustums", &UmbraSceneComponentConfig::m_renderDebugFrustums)
                ->Field("renderDebugVolumes", &UmbraSceneComponentConfig::m_renderDebugVolumes)
                ->Field("renderDebugStats", &UmbraSceneComponentConfig::m_renderDebugStats)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<UmbraSceneComponentConfig>("UmbraSceneComponentConfig", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Show)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_sceneAsset, "Scene Asset", "The scene asset containing precomputed visibility data for occlusion queries")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_sceneSettings, "Scene Settings", "Settings to configure how steam data is processed by the builder")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_exportStaticObjectsOnly, "Export Static Objects Only", "If enabled, only objects with static transforms will be exported with the umbra scene")
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &UmbraSceneComponentConfig::m_additionalMemoryPerQuery, "Additional Memory (bytes) Per Query", "Additional working memory (in bytes) reserved for performing extended occlusion queries.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0)
                        ->Attribute(AZ::Edit::Attributes::Max, 4000000)
                    ->ClassElement(AZ::Edit::ClassElements::Group, "Debug Settings")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, false)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_pauseDebugInfo, "Pause Debug Info", "Freezes the state of debug rendering and statistics")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_renderDebugInfo, "Render Debug Info", "Gather and display debug lines and statistics from occlusion queries")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_renderDebugBounds, "Render Debug Bounds", "When debug rendering is enabled, display bounding boxes for all occlusion objects")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_renderDebugBuffers, "Render Debug Buffers", "When debug rendering is enabled, display a 3D visualization of the visibility buffer")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_renderDebugFrustums, "Render Debug Frustums", "When debug rendering is enabled, display lines for the view frustum")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_renderDebugVolumes, "Render Debug Volumes", "When debug rendering is enabled, display lines for volumes")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_renderDebugStats, "Render Debug Stats", "When debug rendering is enabled, display stats collected from occlusion queries")
                    ->ClassElement(AZ::Edit::ClassElements::Group, "")
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
                ->Property("pauseDebugInfo", BehaviorValueProperty(&UmbraSceneComponentConfig::m_pauseDebugInfo))
                ->Property("renderDebugInfo", BehaviorValueProperty(&UmbraSceneComponentConfig::m_renderDebugInfo))
                ->Property("renderDebugBounds", BehaviorValueProperty(&UmbraSceneComponentConfig::m_renderDebugBounds))
                ->Property("renderDebugBuffers", BehaviorValueProperty(&UmbraSceneComponentConfig::m_renderDebugBuffers))
                ->Property("renderDebugFrustums", BehaviorValueProperty(&UmbraSceneComponentConfig::m_renderDebugFrustums))
                ->Property("renderDebugVolumes", BehaviorValueProperty(&UmbraSceneComponentConfig::m_renderDebugVolumes))
                ->Property("renderDebugStats", BehaviorValueProperty(&UmbraSceneComponentConfig::m_renderDebugStats))
                ->Property("sceneSettings", BehaviorValueProperty(&UmbraSceneComponentConfig::m_sceneSettings))
                ->Property("sceneAsset", BehaviorValueProperty(&UmbraSceneComponentConfig::m_sceneAsset))
                ->Property("exportStaticObjectsOnly", BehaviorValueProperty(&UmbraSceneComponentConfig::m_exportStaticObjectsOnly))
                ->Property("additionalMemoryPerQuery", BehaviorValueProperty(&UmbraSceneComponentConfig::m_additionalMemoryPerQuery))
                ;
        }
    }
} // namespace Umbra
