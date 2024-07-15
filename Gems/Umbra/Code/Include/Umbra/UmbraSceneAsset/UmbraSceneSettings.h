/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/RTTI.h>

namespace AZ
{
    class ReflectContext;
}

namespace Umbra
{
    //! UmbraSceneSettings provides details about an entity that has been added to the umbra scene. The same entity can be added with
    //! multiple parts, like multiple meshes found in a model.
    struct UmbraSceneSettings final
    {
    public:
        AZ_RTTI(UmbraSceneSettings, "{B7B21D65-1036-43BF-870A-0B60A0829765}");
        AZ_CLASS_ALLOCATOR(UmbraSceneSettings, AZ::SystemAllocator);
        static void Reflect(AZ::ReflectContext* context);

        float m_collisionRadius = 0.1f;
        float m_smallestHole = 0.1f;
        float m_smallestOccluder = 0.5f;
    };
} // namespace Umbra
