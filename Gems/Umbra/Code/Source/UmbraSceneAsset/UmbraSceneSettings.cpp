/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Umbra/UmbraSceneAsset/UmbraSceneSettings.h>

namespace Umbra
{
    void UmbraSceneSettings::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraSceneSettings>()
                ->Version(0)
                ->Field("collisionRadius", &UmbraSceneSettings::m_collisionRadius)
                ->Field("smallestHole", &UmbraSceneSettings::m_smallestHole)
                ->Field("smallestOccluder", &UmbraSceneSettings::m_smallestOccluder)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<UmbraSceneSettings>("UmbraSceneSettings", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Show)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &UmbraSceneSettings::m_collisionRadius, "Collision Radius", "The minimum distance between camera and occlusion geometry for computing visibility data.")
                    ->Attribute(AZ::Edit::Attributes::Suffix, "m")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->Attribute(AZ::Edit::Attributes::Max, 1000.0f)
                    ->Attribute(AZ::Edit::Attributes::SoftMin, 0.0f)
                    ->Attribute(AZ::Edit::Attributes::SoftMax, 10.0f)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &UmbraSceneSettings::m_smallestHole, "Smallest Hole", "The minimum size of a gap or voxel that the system sees through when computing visibility data. Smaller values result in higher resolution, more accurate visibility data that will also take longer to process.")
                    ->Attribute(AZ::Edit::Attributes::Suffix, "m")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->Attribute(AZ::Edit::Attributes::Max, 1000.0f)
                    ->Attribute(AZ::Edit::Attributes::SoftMin, 0.0f)
                    ->Attribute(AZ::Edit::Attributes::SoftMax, 10.0f)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &UmbraSceneSettings::m_smallestOccluder, "Smallest Occluder", "The minimum size of a feature or geometry recognized as an occluder when computing visibility data. Smaller values result in higher resolution, more accurate visibility data that will also take longer to process.")
                    ->Attribute(AZ::Edit::Attributes::Suffix, "m")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->Attribute(AZ::Edit::Attributes::Max, 1000.0f)
                    ->Attribute(AZ::Edit::Attributes::SoftMin, 0.0f)
                    ->Attribute(AZ::Edit::Attributes::SoftMax, 10.0f)
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<UmbraSceneSettings>("UmbraSceneSettings")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "Umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Constructor()
                ->Constructor<const UmbraSceneSettings&>()
                ->Property("collisionRadius", BehaviorValueProperty(&UmbraSceneSettings::m_collisionRadius))
                ->Property("smallestHole", BehaviorValueProperty(&UmbraSceneSettings::m_smallestHole))
                ->Property("smallestOccluder", BehaviorValueProperty(&UmbraSceneSettings::m_smallestOccluder))
                ;
        }
    }
} // namespace Umbra
