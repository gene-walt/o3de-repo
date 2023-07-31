/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Asset/AssetManager.h>
#include <AzCore/Component/Component.h>
#include <Umbra/UmbraSystemComponent/UmbraSystemComponentBus.h>
#include <Umbra/UmbraSystemComponent/UmbraSystemComponentConfig.h>
#include <Umbra/UmbraTomeAsset/UmbraTomeAsset.h>

#include <umbra/optimizer/umbraLocalComputation.hpp>
#include <umbra/optimizer/umbraScene.hpp>
#include <umbra/runtime/umbraQuery.hpp>
#include <umbra/runtime/umbraTome.hpp>
#include <umbra/umbraInfo.hpp>

namespace Umbra
{
    //!
    class UmbraSystemComponentController final
        : UmbraSystemComponentRequestBus::Handler
        , AZ::Data::AssetBus::Handler
    {
    public:
        friend class EditorUmbraSystemComponent;

        AZ_CLASS_ALLOCATOR(UmbraSystemComponentController, AZ::SystemAllocator);
        AZ_RTTI(UmbraSystemComponentController, "{E1DC4DD8-73E3-44A9-8F29-FC6731E108F4}");

        static void Reflect(AZ::ReflectContext* context);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        UmbraSystemComponentController() = default;
        UmbraSystemComponentController(const UmbraSystemComponentConfig& config);

        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const UmbraSystemComponentConfig& config);
        const UmbraSystemComponentConfig& GetConfiguration() const;

        //! UmbraSystemComponentRequestBus overrides...

    private:
        AZ_DISABLE_COPY(UmbraSystemComponentController);

        //! AZ::Data::AssetBus overrides...
        void OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetError(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloadError(AZ::Data::Asset<AZ::Data::AssetData> asset) override;

        AZ::EntityId m_entityId;
        UmbraSystemComponentConfig m_configuration;

        void QueueTome();
        void CreateTome();
        void ReleaseTome();

        AZStd::unique_ptr<UmbraTomeAssetHandler> m_tomeAssetHandler;
        AZ::Data::Asset<UmbraTomeAsset> m_tomeAsset;
        const Umbra::Tome* m_tome = nullptr;
    };
} // namespace Umbra
