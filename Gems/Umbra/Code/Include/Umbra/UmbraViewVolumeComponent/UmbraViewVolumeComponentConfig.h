/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/RTTI.h>
#include <Umbra/UmbraSceneAsset/UmbraSceneSettings.h>

namespace AZ
{
    class ReflectContext;
}

namespace Umbra
{
    //! UmbraViewVolumeComponent persistent configuration with settings for overriding same computation values within the volume. 
    class UmbraViewVolumeComponentConfig final : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(UmbraViewVolumeComponentConfig, "{845FC090-884E-4EFF-B8F3-2D038BC49424}", AZ::ComponentConfig);
        AZ_CLASS_ALLOCATOR(UmbraViewVolumeComponentConfig, AZ::SystemAllocator);
        static void Reflect(AZ::ReflectContext* context);

        bool m_overrideSceneSettings = false;
        UmbraSceneSettings m_sceneSettings;
    };
} // namespace Umbra
