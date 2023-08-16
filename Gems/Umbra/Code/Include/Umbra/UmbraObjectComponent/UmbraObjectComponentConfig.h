/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/Component.h>

namespace Umbra
{
    //! Common configuration for UmbraObjectComponent
    class UmbraObjectComponentConfig final : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(UmbraObjectComponentConfig, "{DED86709-F4F6-4B73-A181-2C2176E6B4D6}", AZ::ComponentConfig);
        AZ_CLASS_ALLOCATOR(UmbraObjectComponentConfig, AZ::SystemAllocator);

        static void Reflect(AZ::ReflectContext* context);

        bool m_canOcclude = true;
        bool m_canBeOccluded = true;
    };
} // namespace Umbra
