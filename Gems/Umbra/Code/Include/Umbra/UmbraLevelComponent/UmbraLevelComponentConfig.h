/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/Component.h>

namespace Umbra
{
    //! Common configuration for UmbraLevelComponent
    class UmbraLevelComponentConfig final : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(UmbraLevelComponentConfig, "{C6384EFC-3691-4BF0-AD0F-E59C33BCE548}", AZ::ComponentConfig);
        AZ_CLASS_ALLOCATOR(UmbraLevelComponentConfig, AZ::SystemAllocator);

        static void Reflect(AZ::ReflectContext* context);
    };
} // namespace Umbra
