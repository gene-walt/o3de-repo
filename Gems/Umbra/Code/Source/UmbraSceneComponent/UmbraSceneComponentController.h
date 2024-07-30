/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Asset/AssetManager.h>
#include <AzCore/Component/Component.h>
#include <AzFramework/Entity/EntityDebugDisplayBus.h>
#include <AzFramework/Visibility/OcclusionBus.h>
#include <Umbra/UmbraSceneComponent/UmbraSceneComponentBus.h>
#include <Umbra/UmbraSceneComponent/UmbraSceneComponentConfig.h>

#include <runtime/umbraQuery.hpp>
#include <runtime/umbraTome.hpp>
#include <umbraInfo.hpp>

namespace Umbra
{
    //! UmbraSceneComponentController loads and manages umbra scene assets containing precomputed visibility data. Once this asset is
    //! loaded, occlusion queries can be made against the precomputed visibility data. Entity visibility checks will first determine if the
    //! entity was part of the occlusion scene And return immediately if the query provides precomputed data for that entity. Otherwise,
    //! entity visibility checks will do simple AABB tests against the occlusion buffer. This class implements OcclusionRequestBus and
    //! supports creating multiple occlusion views from different perspectives and parallel occlusion queries.
    class UmbraSceneComponentController final
        : public UmbraSceneComponentRequestBus::Handler
        , public AzFramework::OcclusionRequestBus::Handler
        , public AzFramework::EntityDebugDisplayEventBus::Handler
        , public AZ::Data::AssetBus::Handler
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
        bool IsSceneReady() const override;

        //! AzFramework::OcclusionRequestBus::Handler overrides...
        void ClearOcclusionViewDebugInfo() override;
        bool IsOcclusionViewValid(const AZ::Name& viewName) const override;
        bool CreateOcclusionView(const AZ::Name& viewName) override;
        bool DestroyOcclusionView(const AZ::Name& viewName) override;
        bool UpdateOcclusionView(const AZ::Name& viewName, const AZ::Vector3& cameraWorldPos, const AZ::Matrix4x4& cameraWorldToClip) override;
        AzFramework::OcclusionState GetOcclusionViewEntityVisibility(const AZ::Name& viewName, const AZ::EntityId& entityId) const override;
        AzFramework::OcclusionState GetOcclusionViewAabbVisibility(const AZ::Name& viewName, const AZ::Aabb& bounds) const override;
        AZStd::vector<AzFramework::OcclusionState> GetOcclusionViewAabbToAabbVisibility(
            const AZ::Name& viewName, const AZ::Aabb& sourceAabb, const AZStd::vector<AZ::Aabb>& targetAabbs) const override;
        AZStd::vector<AzFramework::OcclusionState> GetOcclusionViewSphereToSphereVisibility(
            const AZ::Name& viewName, const AZ::Sphere& sourceSphere, const AZStd::vector<AZ::Sphere>& targetSpheres) const override;
        AZStd::vector<AzFramework::OcclusionState> GetOcclusionViewEntityToEntityVisibility(
            const AZ::Name& viewName, const AZ::EntityId& sourceEntityId, const AZStd::vector<AZ::EntityId>& targetEntityIds) const override;

    private:
        AZ_DISABLE_COPY_MOVE(UmbraSceneComponentController);

        //! AzFramework::DebugDisplayRequestBus::Handler overrides...
        void DisplayEntityViewport(const AzFramework::ViewportInfo& viewportInfo, AzFramework::DebugDisplayRequests& debugDisplay) override;

        //! AZ::Data::AssetBus overrides...
        void OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetError(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloadError(AZ::Data::Asset<AZ::Data::AssetData> asset) override;

        //! Request loading the scene asset from the component configuration
        void QueueScene();
        //! Once the scene asset has been loaded, create an umbra tome and dependencies to support occlusion queries
        void CreateScene();
        //! Destroy the umbra tome dependencies
        void ReleaseScene();

        AZ::EntityId m_entityId;
        UmbraSceneComponentConfig m_configuration;

        // This is the runtime version of the precomputed visibility data created from data serialized with the scene asset.
        const Umbra::Tome* m_tome = nullptr;
        // Vector of entity IDs, whose indices correspond to object indices in the tome.
        AZStd::vector<AZ::EntityId> m_objectIndexToEntityIdTable;
        // Collection of entity ids were referenced in the baked scene data.
        AZStd::unordered_set<AZ::EntityId> m_storedEntityIds;

        // Capturing data for unique line rendering requests. The data is stored as a set of tuples to automatically eliminate duplicates
        // without implementing custom sort and removal logic. Each tuple contains 10 floats, the first four represent rgba color values,
        // the next three represent the starting point for the line segment, the last three represent the end point for the line segment.
        using UmbraDebugRendererLine = AZStd::tuple<float, float, float, float, float, float, float, float, float, float>;
        using UmbraDebugRendererLineSet = AZStd::set<UmbraDebugRendererLine>;

        //! UmbraDebugRenderer works with occlusion queries to capture all of the data for visualizing stats and line segments for umbra
        //! objects like bounding boxes, view frustums, and the visibility buffer.
        class UmbraDebugRenderer : public Umbra::DebugRenderer
        {
        public:
            UmbraDebugRenderer() = default;
            void addLine(const Umbra::Vector3& start, const Umbra::Vector3& end, const Umbra::Vector4& color) override;
            void addStat(const char* stat, int val) override;

            UmbraDebugRendererLineSet m_debugLines;
            AZStd::set<AZStd::pair<AZStd::string, int>> m_debugStats;
        };

        //! UmbraOcclusionView embodies the data for a single occlusion query. Multiple UmbraOcclusionView can exist simultaneously
        //! alongside corresponding graphics views. They can be updated and queried in parallel.
        class UmbraOcclusionView final
        {
        public:
            AZ_CLASS_ALLOCATOR(UmbraOcclusionView, AZ::SystemAllocator);
            AZ_RTTI(UmbraOcclusionView, "{22FA80C6-A096-4A2E-BF93-3AF75E688656}");

            UmbraOcclusionView(UmbraSceneComponentController& controller);
            ~UmbraOcclusionView();
            bool Update(const AZ::Vector3& cameraWorldPos, const AZ::Matrix4x4& cameraWorldToClip);
            AzFramework::OcclusionState GetEntityVisibility(const AZ::EntityId& entityId) const;
            AzFramework::OcclusionState GetAabbVisibility(const AZ::Aabb& bounds) const;
            AZStd::vector<AzFramework::OcclusionState> GetAabbToAabbVisibility(const AZ::Aabb& sourceAabb, const AZStd::vector<AZ::Aabb>& targetAabbs) const;
            AZStd::vector<AzFramework::OcclusionState> GetSphereToSphereVisibility(const AZ::Sphere& sourceSphere, const AZStd::vector<AZ::Sphere>& targetSpheres) const;
            AZStd::vector<AzFramework::OcclusionState> GetEntityToEntityVisibility(const AZ::EntityId& sourceEntityId, const AZStd::vector<AZ::EntityId>& targetEntityIds) const;

        private:
            AZ_DISABLE_COPY_MOVE(UmbraOcclusionView);

            // Reference to controller to access tome, assets, and debug containers
            UmbraSceneComponentController& m_controller;
            // Umbra query object that performs occlusion culling queries against the tome
            AZStd::unique_ptr<Umbra::QueryExt> m_query;
            // Additional working memory for umbrella queries.
            AZStd::vector<AZ::u8> m_queryWorkMem;
            // Vector used as storage for object indices, pre allocating enough space for all object indices in the tome
            AZStd::vector<int32_t> m_objectIndexListStorage;
            // Object list using the above vector as storage. The list will be populated with with indices after each query is executed.
            AZStd::unique_ptr<Umbra::IndexList> m_objectIndexList;
            // Occlusion buffer contains depth data from the previous query to test visibility for dynamic object bounding boxes.
            AZStd::unique_ptr<Umbra::OcclusionBuffer> m_occlusionBuffer;
            // Collection of entity IDs visible since the last query.
            AZStd::unordered_set<AZ::EntityId> m_visibleEntityIds;
            // Interfaces with umbra and the query object to gather debug lines and statistics
            AZStd::unique_ptr<UmbraDebugRenderer> m_debugRenderer;
        };

        // All currently active occlusion views.
        AZStd::unordered_map<AZ::Name, AZStd::unique_ptr<UmbraOcclusionView>> m_occlusionViewMap;

        // Sets of debug data accumulated from all occlusion views.
        AZStd::mutex m_debugMutex;
        UmbraDebugRendererLineSet m_debugLines;
        AZStd::set<AZStd::pair<AZStd::string, int>> m_debugStats;
    };
} // namespace Umbra
