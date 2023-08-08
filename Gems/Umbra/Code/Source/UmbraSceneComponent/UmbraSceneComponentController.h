/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Asset/AssetManager.h>
#include <AzCore/Component/Component.h>
#include <Umbra/UmbraSceneComponent/UmbraSceneComponentBus.h>
#include <Umbra/UmbraSceneComponent/UmbraSceneComponentConfig.h>

#include <umbra/runtime/umbraQuery.hpp>
#include <umbra/runtime/umbraTome.hpp>
#include <umbra/umbraInfo.hpp>

namespace Umbra
{
    //! Loads and manages an umbra scene asset and processes occlusion queries
    class UmbraSceneComponentController final
        : UmbraSceneComponentRequestBus::Handler
        , AZ::Data::AssetBus::Handler
    {
    public:
        friend class EditorUmbraSceneComponent;

        AZ_CLASS_ALLOCATOR(UmbraSceneComponentController, AZ::SystemAllocator);
        AZ_RTTI(UmbraSceneComponentController, "{F90A5E6D-49F7-4B59-BF3C-4D66D5B7B257}");

        static void Reflect(AZ::ReflectContext* context);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& services);

        UmbraSceneComponentController() = default;
        UmbraSceneComponentController(const UmbraSceneComponentConfig& config);

        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const UmbraSceneComponentConfig& config);
        const UmbraSceneComponentConfig& GetConfiguration() const;

        //! UmbraSceneComponentRequestBus overrides...
        void SetSceneAsset(AZ::Data::Asset<UmbraSceneAsset> asset) override;
        void SetSceneAssetId(const AZ::Data::AssetId& assetId) override;
        void SetSceneAssetPath(const AZStd::string& path) override;
        AZ::Data::Asset<const UmbraSceneAsset> GetSceneAsset() const override;
        AZ::Data::AssetId GetSceneAssetId() const override;
        bool UpdateVisibility(const AZ::Matrix4x4& cameraWorldToClip, const AZ::Vector3& cameraWorldPos) override;
        bool IsEntityVisible(const AZ::EntityId& entityId) const override;
        bool IsAabbVisible(const AZ::Aabb& bounds) const override;

    private
            :
        AZ_DISABLE_COPY(UmbraSceneComponentController);

        //! AZ::Data::AssetBus overrides...
        void OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetError(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloadError(AZ::Data::Asset<AZ::Data::AssetData> asset) override;

        void QueueScene();
        void CreateScene();
        void ReleaseScene();
 
        AZ::EntityId m_entityId;
        UmbraSceneComponentConfig m_configuration;

        const Umbra::Tome* m_tome = nullptr;
        AZStd::unique_ptr<Umbra::Query> m_query;
        AZStd::vector<int32_t> m_objectIndexListStorage;
        AZStd::unique_ptr<Umbra::IndexList> m_objectIndexList;
        AZStd::unique_ptr<Umbra::OcclusionBuffer> m_occlusionBuffer;
        AZStd::unordered_set<AZ::EntityId> m_occlusionEntities;
    };
} // namespace Umbra
