/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/RTTI.h>
#include <Umbra/UmbraSceneAsset/UmbraSceneAsset.h>

namespace AZ
{
    class ReflectContext;
}

namespace Umbra
{
    //! UmbraSceneComponent persistent configuration with settings for computing visibility data for the current scene. 
    class UmbraSceneComponentConfig final : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(UmbraSceneComponentConfig, "{C6384EFC-3691-4BF0-AD0F-E59C33BCE548}", AZ::ComponentConfig);
        AZ_CLASS_ALLOCATOR(UmbraSceneComponentConfig, AZ::SystemAllocator);

        static void Reflect(AZ::ReflectContext* context);

        float m_collisionRadius = 0.125f;
        float m_smallestHole = 0.5f;
        float m_smallestOccluder = 3.f;
        AZ::Data::Asset<UmbraSceneAsset> m_sceneAsset;
    };
} // namespace Umbra
