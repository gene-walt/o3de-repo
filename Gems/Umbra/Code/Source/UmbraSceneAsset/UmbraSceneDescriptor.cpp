/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <UmbraSceneAsset/UmbraSceneDescriptor.h>

namespace Umbra
{
    const char* UmbraSceneDescriptor::Extension = "umbrascenedesc";

    void UmbraSceneDescriptor::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraSceneDescriptor>()
                ->Version(0)
                ->Field("sceneFilename", &UmbraSceneDescriptor::m_sceneFilename)
                ->Field("collisionRadius", &UmbraSceneDescriptor::m_collisionRadius)
                ->Field("smallestHole", &UmbraSceneDescriptor::m_smallestHole)
                ->Field("smallestOccluder", &UmbraSceneDescriptor::m_smallestOccluder)
                ->Field("objectDescriptors", &UmbraSceneDescriptor::m_objectDescriptors)
                ;
        }
    }
} // namespace Umbra
