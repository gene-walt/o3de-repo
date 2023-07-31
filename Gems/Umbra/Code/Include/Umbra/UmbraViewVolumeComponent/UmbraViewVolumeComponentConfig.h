/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/Component.h>

namespace Umbra
{
    //! Common configuration for UmbraViewVolumeComponent
    class UmbraViewVolumeComponentConfig final : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(UmbraViewVolumeComponentConfig, "{845FC090-884E-4EFF-B8F3-2D038BC49424}", AZ::ComponentConfig);
        AZ_CLASS_ALLOCATOR(UmbraViewVolumeComponentConfig, AZ::SystemAllocator);

        static void Reflect(AZ::ReflectContext* context);
    };
} // namespace Umbra
