/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/EntityId.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/RTTI.h>

namespace AZ
{
    class ReflectContext;
}

namespace Umbra
{
    //! UmbraObjectDescriptor provides details about an entity that has been added to the umbra scene. The same entity can be added with
    //! multiple parts, like multiple meshes found in a model.
    struct UmbraObjectDescriptor final
    {
    public:
        AZ_RTTI(UmbraObjectDescriptor, "{B82FCA9E-01D2-48C6-A598-21CD61707D29}");
        AZ_CLASS_ALLOCATOR(UmbraObjectDescriptor, AZ::SystemAllocator);
        static void Reflect(AZ::ReflectContext* context);

        AZ::EntityId m_entityId;
        uint32_t m_partId = 0;
    };
} // namespace Umbra
