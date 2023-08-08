/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>
#include <Umbra/UmbraSceneAsset/UmbraObjectDescriptor.h>

namespace AZ
{
    class ReflectContext;
}

namespace Umbra
{
    //! UmbraSceneDescriptor provides the asset builder with settings related to computing baked visibility data
    struct UmbraSceneDescriptor final
    {
    public:
        AZ_RTTI(UmbraSceneDescriptor, "{7CB5B24B-4B04-4B80-9255-1E3D9E596979}");
        AZ_CLASS_ALLOCATOR(UmbraSceneDescriptor, AZ::SystemAllocator);
        static void Reflect(AZ::ReflectContext* context);

        static const char* Extension;

        AZStd::string m_sceneFilename;
        float m_collisionRadius = 0.125f;
        float m_smallestHole = 0.5f;
        float m_smallestOccluder = 3.f;
        AZStd::vector<UmbraObjectDescriptor> m_objectDescriptors;
    };
} // namespace Umbra
