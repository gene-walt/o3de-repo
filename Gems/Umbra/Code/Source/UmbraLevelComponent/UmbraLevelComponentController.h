/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/Component.h>
#include <Umbra/UmbraLevelComponent/UmbraLevelComponentBus.h>
#include <Umbra/UmbraLevelComponent/UmbraLevelComponentConfig.h>

namespace Umbra
{
    //!
    class UmbraLevelComponentController final
        : UmbraLevelComponentRequestBus::Handler
        , AZ::Data::AssetBus::Handler
    {
    public:
        friend class EditorUmbraLevelComponent;

        AZ_CLASS_ALLOCATOR(UmbraLevelComponentController, AZ::SystemAllocator);
        AZ_RTTI(UmbraLevelComponentController, "{F90A5E6D-49F7-4B59-BF3C-4D66D5B7B257}");

        static void Reflect(AZ::ReflectContext* context);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        UmbraLevelComponentController() = default;
        UmbraLevelComponentController(const UmbraLevelComponentConfig& config);

        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const UmbraLevelComponentConfig& config);
        const UmbraLevelComponentConfig& GetConfiguration() const;

        //! UmbraLevelComponentRequestBus overrides...

    private:
        AZ_DISABLE_COPY(UmbraLevelComponentController);

        //! AZ::Data::AssetBus overrides...
        void OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetError(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloadError(AZ::Data::Asset<AZ::Data::AssetData> asset) override;

        AZ::EntityId m_entityId;
        UmbraLevelComponentConfig m_configuration;
    };
} // namespace Umbra
