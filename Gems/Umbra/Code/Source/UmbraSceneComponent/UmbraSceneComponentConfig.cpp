/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/Asset/AssetSerializer.h>
#include <AzCore/RTTI/BehaviorContext.h>
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
                ->Field("collisionRadius", &UmbraSceneComponentConfig::m_collisionRadius)
                ->Field("smallestHole", &UmbraSceneComponentConfig::m_smallestHole)
                ->Field("smallestOccluder", &UmbraSceneComponentConfig::m_smallestOccluder)
                ->Field("sceneAsset", &UmbraSceneComponentConfig::m_sceneAsset)
                ;
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<UmbraSceneComponentConfig>("UmbraSceneComponentConfig")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Constructor()
                ->Constructor<const UmbraSceneComponentConfig&>()
                ;
        }
    }
} // namespace Umbra
