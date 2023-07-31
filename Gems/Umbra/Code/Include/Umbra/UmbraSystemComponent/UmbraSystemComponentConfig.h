/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/Component.h>

namespace Umbra
{
    //! Common configuration for UmbraSystemComponent
    class UmbraSystemComponentConfig final : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(UmbraSystemComponentConfig, "{A518CBF4-ACC9-4B9E-879E-E62CC83C6DF3}", AZ::ComponentConfig);
        AZ_CLASS_ALLOCATOR(UmbraSystemComponentConfig, AZ::SystemAllocator);

        static void Reflect(AZ::ReflectContext* context);
    };
} // namespace Umbra
