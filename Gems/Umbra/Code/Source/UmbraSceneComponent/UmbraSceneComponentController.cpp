/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/Math/Matrix4x4.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzFramework/Entity/EntityContextBus.h>
#include <AzFramework/Entity/GameEntityContextComponent.h>
#include <AzFramework/Visibility/BoundsBus.h>
#include <UmbraSceneComponent/UmbraSceneComponentController.h>

namespace Umbra
{
    namespace Internal
    {
        void PrintQueryErrorMessage(const Umbra::Query::ErrorCode errorCode)
        {
            switch (errorCode)
            {
            case Umbra::Query::ERROR_OK:
                break;
            case Umbra::Query::ERROR_GENERIC_ERROR:
                AZ_Warning("UmbraSceneComponent", false, "Something completely unexpected happened.");
                break;
            case Umbra::Query::ERROR_OUT_OF_MEMORY:
                AZ_Warning("UmbraSceneComponent", false, "Not enough memory was available in the Query context to perform the operation.");
                break;
            case Umbra::Query::ERROR_INVALID_ARGUMENT:
                AZ_Warning("UmbraSceneComponent", false, "An invalid value was passed into queryPortalVisibility.");
                break;
            case Umbra::Query::ERROR_SLOTDATA_UNAVAILABLE:
                AZ_Warning("UmbraSceneComponent", false, "A tile required to complete the Query was not present in the tome.");
                break;
            case Umbra::Query::ERROR_OUTSIDE_SCENE:
                AZ_Warning("UmbraSceneComponent", false, "A query location was found to be outside of the scene boundaries.");
                break;
            case Umbra::Query::ERROR_NO_TOME:
                AZ_Warning("UmbraSceneComponent", false, "queryPortalVisibility no data was given to the Query.");
                break;
            case Umbra::Query::ERROR_NO_PATH:
                AZ_Warning("UmbraSceneComponent", false, "queryPortalVisibility path does not exist.");
                break;
            case Umbra::Query::ERROR_UNSUPPORTED_OPERATION:
                AZ_Warning("UmbraSceneComponent", false, "queryPortalVisibility operation not supported.");
                break;
            case Umbra::Query::ERROR_FEATURE_REQUIRES_BASELEVEL:
                AZ_Warning("UmbraSceneComponent", false, "Use of query features that don't work with accurate occlusion threshold < FLT_MAX.");
                break;
            default:
                AZ_Warning("UmbraSceneComponent", false, "queryPortalVisibility returned unrecognized error code.");
                break;
            }
        }

        AZ::Aabb GetEntityAabb(const AZ::EntityId& entityId)
        {
            AZ::Aabb bounds = AZ::Aabb::CreateNull();
            AzFramework::BoundsRequestBus::EventResult(bounds, entityId, &AzFramework::BoundsRequestBus::Events::GetWorldBounds);
            if (bounds.IsValid())
            {
                return bounds;
            }

            AZ::Transform transform = AZ::Transform::CreateIdentity();
            AZ::TransformBus::EventResult(transform, entityId, &AZ::TransformBus::Events::GetWorldTM);
            return AZ::Aabb::CreateFromPoint(transform.GetTranslation());
        }
    }

    void UmbraSceneComponentController::Reflect(AZ::ReflectContext* context)
    {
        UmbraSceneComponentConfig::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraSceneComponentController>()
                ->Version(0)
                ->Field("Configuration", &UmbraSceneComponentController::m_configuration)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<UmbraSceneComponentController>("UmbraSceneComponentController", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentController::m_configuration, "Configuration", "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<UmbraSceneComponentRequestBus>("UmbraSceneComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Event("SetSceneAsset", &UmbraSceneComponentRequestBus::Events::SetSceneAsset)
                ->Event("SetSceneAssetId", &UmbraSceneComponentRequestBus::Events::SetSceneAssetId)
                ->Event("SetSceneAssetPath", &UmbraSceneComponentRequestBus::Events::SetSceneAssetPath)
                ->Event("GetSceneAsset", &UmbraSceneComponentRequestBus::Events::GetSceneAsset)
                ->Event("GetSceneAssetId", &UmbraSceneComponentRequestBus::Events::GetSceneAssetId)
                ->Event("IsSceneReady", &UmbraSceneComponentRequestBus::Events::IsSceneReady)
                ;
        }
    }

    void UmbraSceneComponentController::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC_CE("UmbraSceneService"));
    }

    void UmbraSceneComponentController::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC_CE("UmbraobjectIndexListervice"));
        services.push_back(AZ_CRC_CE("UmbraSceneService"));
        services.push_back(AZ_CRC_CE("UmbraViewVolumeService"));
    }

    void UmbraSceneComponentController::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& services)
    {
    }

    UmbraSceneComponentController::UmbraSceneComponentController(const UmbraSceneComponentConfig& config)
        : m_configuration(config)
    {
    }

    void UmbraSceneComponentController::Activate(AZ::EntityId entityId)
    {
        m_entityId = entityId;

        //AzFramework::EntityContextId contextId = AzFramework::EntityContextId::CreateNull();
        //AzFramework::EntityIdContextQueryBus::EventResult(
        //    contextId, m_entityId, &AzFramework::EntityIdContextQueryBus::Events::GetOwningContextId);

        AzFramework::EntityContext* gameEntityContext = nullptr;
        AzFramework::GameEntityContextRequestBus::BroadcastResult(
            gameEntityContext, &AzFramework::GameEntityContextRequests::GetGameEntityContextInstance);
        const AzFramework::EntityContextId contextId = gameEntityContext->GetContextId();

        UmbraSceneComponentRequestBus::Handler::BusConnect(m_entityId);
        AzFramework::EntityDebugDisplayEventBus::Handler::BusConnect(m_entityId);
        AzFramework::OcclusionRequestBus::Handler::BusConnect(contextId);

        QueueScene();
    }

    void UmbraSceneComponentController::Deactivate()
    {
        AZ::Data::AssetBus::Handler::BusDisconnect();
        AzFramework::OcclusionRequestBus::Handler::BusDisconnect();
        AzFramework::EntityDebugDisplayEventBus::Handler::BusDisconnect();
        UmbraSceneComponentRequestBus::Handler::BusDisconnect();

        ReleaseScene();

        m_configuration.m_sceneAsset.Release();
        m_entityId = AZ::EntityId(AZ::EntityId::InvalidEntityId);
    }

    void UmbraSceneComponentController::SetConfiguration(const UmbraSceneComponentConfig& config)
    {
        m_configuration = config;
    }

    const UmbraSceneComponentConfig& UmbraSceneComponentController::GetConfiguration() const
    {
        return m_configuration;
    }

    void UmbraSceneComponentController::SetSceneAsset(AZ::Data::Asset<UmbraSceneAsset> asset)
    {
        m_configuration.m_sceneAsset = asset;
        QueueScene();
    }

    void UmbraSceneComponentController::SetSceneAssetId(const AZ::Data::AssetId& assetId)
    {
        SetSceneAsset(AZ::Data::AssetManager::Instance().FindOrCreateAsset(
            assetId, azrtti_typeid<UmbraSceneAsset>(), AZ::Data::AssetLoadBehavior::PreLoad));
    }

    void UmbraSceneComponentController::SetSceneAssetPath(const AZStd::string& path)
    {
        AZ::Data::AssetId assetId;
        AZ::Data::AssetCatalogRequestBus::BroadcastResult(
            assetId, &AZ::Data::AssetCatalogRequestBus::Events::GetAssetIdByPath, path.c_str(), azrtti_typeid<UmbraSceneAsset>(), false);
        SetSceneAssetId(assetId);
    }

    AZ::Data::Asset<const UmbraSceneAsset> UmbraSceneComponentController::GetSceneAsset() const
    {
        return m_configuration.m_sceneAsset;
    }

    AZ::Data::AssetId UmbraSceneComponentController::GetSceneAssetId() const
    {
        return m_configuration.m_sceneAsset.GetId();
    }

    bool UmbraSceneComponentController::IsSceneReady() const
    {
        return m_configuration.m_sceneAsset.IsReady() && m_tome;
    }

    void UmbraSceneComponentController::ClearOcclusionViewDebugInfo()
    {
        if (!m_configuration.m_pauseDebugInfo)
        {
            AZStd::scoped_lock<decltype(m_debugMutex)> lock(m_debugMutex);
            m_debugLines.clear();
            m_debugStats.clear();
        }
    }

    bool UmbraSceneComponentController::IsOcclusionViewValid(const AZ::Name& viewName) const
    {
        if (!IsSceneReady())
        {
            return false;
        }

        return m_occlusionViewMap.contains(viewName);
    }

    bool UmbraSceneComponentController::CreateOcclusionView(const AZ::Name& viewName)
    {
        if (!IsSceneReady())
        {
            return false;
        }

        return m_occlusionViewMap.emplace(viewName, AZStd::make_unique<UmbraOcclusionView>(*this)).second;
    }

    bool UmbraSceneComponentController::DestroyOcclusionView(const AZ::Name& viewName)
    {
        return m_occlusionViewMap.erase(viewName) != 0;
    }

    bool UmbraSceneComponentController::UpdateOcclusionView(
        const AZ::Name& viewName, const AZ::Vector3& cameraWorldPos, const AZ::Matrix4x4& cameraWorldToClip)
    {
        const auto occlusionViewItr = m_occlusionViewMap.find(viewName);
        return occlusionViewItr != m_occlusionViewMap.end() ? occlusionViewItr->second->Update(cameraWorldPos, cameraWorldToClip) : false;
    }

    AzFramework::OcclusionState UmbraSceneComponentController::GetOcclusionViewEntityVisibility(const AZ::Name& viewName, const AZ::EntityId& entityId) const
    {
        const auto occlusionViewItr = m_occlusionViewMap.find(viewName);
        return occlusionViewItr != m_occlusionViewMap.end() ? occlusionViewItr->second->GetEntityVisibility(entityId) : AzFramework::OcclusionState::Unknown;
    }

    AzFramework::OcclusionState UmbraSceneComponentController::GetOcclusionViewAabbVisibility(const AZ::Name& viewName, const AZ::Aabb& bounds) const
    {
        const auto occlusionViewItr = m_occlusionViewMap.find(viewName);
        return occlusionViewItr != m_occlusionViewMap.end() ? occlusionViewItr->second->GetAabbVisibility(bounds) : AzFramework::OcclusionState::Unknown;
    }

    AZStd::vector<AzFramework::OcclusionState> UmbraSceneComponentController::GetOcclusionViewAabbToAabbVisibility(const AZ::Name& viewName, const AZ::Aabb& sourceAabb, const AZStd::vector<AZ::Aabb>& targetAabbs) const
    {
        const auto occlusionViewItr = m_occlusionViewMap.find(viewName);
        return occlusionViewItr != m_occlusionViewMap.end() ? occlusionViewItr->second->GetAabbToAabbVisibility(sourceAabb, targetAabbs) : AZStd::vector<AzFramework::OcclusionState>{};
    }

    AZStd::vector<AzFramework::OcclusionState> UmbraSceneComponentController::GetOcclusionViewSphereToSphereVisibility(const AZ::Name& viewName, const AZ::Sphere& sourceSphere, const AZStd::vector<AZ::Sphere>& targetSpheres) const
    {
        const auto occlusionViewItr = m_occlusionViewMap.find(viewName);
        return occlusionViewItr != m_occlusionViewMap.end() ? occlusionViewItr->second->GetSphereToSphereVisibility(sourceSphere, targetSpheres) : AZStd::vector<AzFramework::OcclusionState>{};
    }

    AZStd::vector<AzFramework::OcclusionState> UmbraSceneComponentController::GetOcclusionViewEntityToEntityVisibility(const AZ::Name& viewName, const AZ::EntityId& sourceEntityId, const AZStd::vector<AZ::EntityId>& targetEntityIds) const
    {
        const auto occlusionViewItr = m_occlusionViewMap.find(viewName);
        return occlusionViewItr != m_occlusionViewMap.end() ? occlusionViewItr->second->GetEntityToEntityVisibility(sourceEntityId, targetEntityIds) : AZStd::vector<AzFramework::OcclusionState>{};
    }

    void UmbraSceneComponentController::DisplayEntityViewport(
        [[maybe_unused]] const AzFramework::ViewportInfo& viewportInfo, AzFramework::DebugDisplayRequests& debugDisplay)
    {
        if (m_configuration.m_renderDebugInfo)
        {
            AZStd::scoped_lock<decltype(m_debugMutex)> lock(m_debugMutex);

            AZ::Color color = {};
            AZStd::vector<AZ::Vector3> verts;
            verts.reserve(m_debugLines.size() * 2);
            for (const auto& line : m_debugLines)
            {
                AZ::Color nextColor(AZStd::get<0>(line), AZStd::get<1>(line), AZStd::get<2>(line), AZStd::get<3>(line));
                if (color != nextColor)
                {
                    if (!verts.empty())
                    {
                        debugDisplay.DrawLines(verts, color);
                        verts.clear();
                    }
                    color = nextColor;
                }
                verts.emplace_back(AZStd::get<4>(line), AZStd::get<5>(line), AZStd::get<6>(line));
                verts.emplace_back(AZStd::get<7>(line), AZStd::get<8>(line), AZStd::get<9>(line));
            }
            if (!verts.empty())
            {
                debugDisplay.DrawLines(verts, color);
            }
        }
    }

    void UmbraSceneComponentController::OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        m_configuration.m_sceneAsset = asset;
        CreateScene();
    }

    void UmbraSceneComponentController::OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        m_configuration.m_sceneAsset = asset;
        CreateScene();
    }

    void UmbraSceneComponentController::OnAssetError(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        ReleaseScene();
    }

    void UmbraSceneComponentController::OnAssetReloadError(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        ReleaseScene();
    }

    void UmbraSceneComponentController::QueueScene()
    {
        ReleaseScene();

        AZ::Data::AssetBus::Handler::BusDisconnect();

        if (m_configuration.m_sceneAsset.GetId().IsValid())
        {
            m_configuration.m_sceneAsset.QueueLoad();
            AZ::Data::AssetBus::Handler::BusConnect(m_configuration.m_sceneAsset.GetId());
        }
    }

    void UmbraSceneComponentController::CreateScene()
    {
        if (m_configuration.m_sceneAsset.IsReady())
        {
            const auto& tomeBuffer = m_configuration.m_sceneAsset->GetTomeBuffer();
            m_tome = Umbra::TomeLoader::loadFromBuffer(tomeBuffer.data(), tomeBuffer.size());

            // Reserve and build a set of entity IDs stored in the scene asset object descriptor table. This will be used to determine if
            // occlusion tests can be based off of the object index list or if occlusion buffer tests are required.
            m_occlusionViewMap.clear();
            m_storedEntityIds.clear();
            m_objectIndexToEntityIdTable.clear();
            m_objectIndexToEntityIdTable.resize(m_tome->getObjectCount());

            const auto& objectDescs = m_configuration.m_sceneAsset->GetObjectDescriptors();
            for (int32_t objectIndex = 0; objectIndex < m_tome->getObjectCount(); ++objectIndex)
            {
                const uint32_t userIndex = m_tome->getObjectUserID(objectIndex);
                if (userIndex < objectDescs.size())
                {
                    const auto& objectDesc = objectDescs[userIndex];
                    m_storedEntityIds.insert(objectDesc.m_entityId);
                    m_objectIndexToEntityIdTable[objectIndex] = objectDesc.m_entityId;
                }
            }
        }
    }

    void UmbraSceneComponentController::ReleaseScene()
    {
        m_occlusionViewMap.clear();
        m_storedEntityIds.clear();
        m_objectIndexToEntityIdTable.clear();

        if (m_tome)
        {
            Umbra::TomeLoader::freeTome(m_tome);
            m_tome = nullptr;
        }
    }

    void UmbraSceneComponentController::UmbraDebugRenderer::addLine(
        const Umbra::Vector3& start, const Umbra::Vector3& end, const Umbra::Vector4& color)
    {
        m_debugLines.emplace(AZStd::make_tuple(
            color.v[0], color.v[1], color.v[2], color.v[3], start.v[0], start.v[1], start.v[2], end.v[0], end.v[1], end.v[2]));
    }

    void UmbraSceneComponentController::UmbraDebugRenderer::addStat(const char* stat, int val)
    {
        m_debugStats.emplace(AZStd::make_pair(stat, val));
    }

    UmbraSceneComponentController::UmbraOcclusionView::UmbraOcclusionView(
        UmbraSceneComponentController& controller)
        : m_controller(controller)
    {
        AZ_Assert(m_controller.IsSceneReady(), "Umbra occlusion view cannot be created before scene data is ready.");

        m_query.reset(new Umbra::QueryExt(m_controller.m_tome));
        m_queryWorkMem.resize(m_controller.GetConfiguration().m_additionalMemoryPerQuery);
        m_query->setWorkMem(m_queryWorkMem.data(), aznumeric_cast<size_t>(m_queryWorkMem.size()));
        m_objectIndexListStorage = AZStd::vector<int32_t>(m_controller.m_tome->getObjectCount());
        m_objectIndexList.reset(new Umbra::IndexList(m_objectIndexListStorage.data(), m_controller.m_tome->getObjectCount()));
        m_occlusionBuffer.reset(new Umbra::OcclusionBuffer());

        if (m_controller.m_configuration.m_renderDebugInfo)
        {
            m_debugRenderer.reset(new UmbraDebugRenderer());
            m_query->setDebugRenderer(m_debugRenderer.get());
        }
    }

    UmbraSceneComponentController::UmbraOcclusionView::~UmbraOcclusionView()
    {
        m_query.reset();
        m_objectIndexList.reset();
        m_visibleEntityIds.clear();
        m_objectIndexListStorage.clear();
        m_occlusionBuffer.reset();
        m_debugRenderer.reset();
    }

    bool UmbraSceneComponentController::UmbraOcclusionView::Update(
        const AZ::Vector3& cameraWorldPos, const AZ::Matrix4x4& cameraWorldToClip)
    {
        AZ_Assert(m_controller.IsSceneReady(), "Umbra scene component controller and assets must be valid and ready before query.");
        AZ_Assert(m_query, "Umbra query object must be valid before query.");
        AZ_Assert(m_objectIndexList, "Umbra object index list must be valid before query.");
        AZ_Assert(m_occlusionBuffer, "Umbra occlusion buffer must be instantiated before query.");

        // Reset the object index list and occlusion buffer
        m_objectIndexList->setSize(0);
        m_occlusionBuffer->clear();

        if (m_debugRenderer)
        {
            m_debugRenderer->m_debugLines.clear();
            m_debugRenderer->m_debugStats.clear();
        }

        // Convert the incoming camera transform into an umbra camera transform
        Umbra::Matrix4x4 worldToClip = {};
        cameraWorldToClip.StoreToRowMajorFloat16(reinterpret_cast<float*>(worldToClip.m));

        // Flip z-axis due to reverse Z perspective transform to get depth range [0,1]
        worldToClip.m[2][0] = -worldToClip.m[2][0] + worldToClip.m[3][0];
        worldToClip.m[2][1] = -worldToClip.m[2][1] + worldToClip.m[3][1];
        worldToClip.m[2][2] = -worldToClip.m[2][2] + worldToClip.m[3][2];
        worldToClip.m[2][3] = -worldToClip.m[2][3] + worldToClip.m[3][3];

        Umbra::Vector3 cameraPos = {};
        cameraWorldPos.StoreToFloat3(cameraPos.v);

        Umbra::CameraTransform camera(worldToClip, cameraPos, Umbra::CameraTransform::DEPTHRANGE_ZERO_TO_ONE, Umbra::MF_ROW_MAJOR);

        // Set up the visibility query parameters to fill in the object index list and occlusion buffer
        Umbra::Visibility visibility(m_objectIndexList.get(), m_occlusionBuffer.get());

        const auto& configuration = m_controller.m_configuration;
        const bool renderDebugInfo = m_debugRenderer && configuration.m_renderDebugInfo && !configuration.m_pauseDebugInfo;

        // Configure debug flags for data to be captured during the query
        uint32_t flags = 0;
        flags |= Umbra::Query::VisibilityQueryFlags::QUERYFLAG_IGNORE_CAMERA_POSITION;
        if (renderDebugInfo)
        {
            flags |= configuration.m_renderDebugBuffers ? Umbra::Query::VisibilityQueryFlags::DEBUGFLAG_VISIBILITY_LINES : 0;
            flags |= configuration.m_renderDebugBounds ? Umbra::Query::VisibilityQueryFlags::DEBUGFLAG_OBJECT_BOUNDS : 0;
            flags |= configuration.m_renderDebugVolumes ? Umbra::Query::VisibilityQueryFlags::DEBUGFLAG_VISIBLE_VOLUME : 0;
            flags |= configuration.m_renderDebugFrustums ? Umbra::Query::VisibilityQueryFlags::DEBUGFLAG_VIEW_FRUSTUM : 0;
            flags |= configuration.m_renderDebugStats ? Umbra::Query::VisibilityQueryFlags::DEBUGFLAG_STATISTICS : 0;
        }

        const Umbra::Query::ErrorCode errorCode = m_query->queryPortalVisibility(flags, visibility, camera);
        if (errorCode != Umbra::Query::ErrorCode::ERROR_OK)
        {
            Internal::PrintQueryErrorMessage(errorCode);
        }

        // Store a container of entity IDs corresponding to visible object indices.
        m_visibleEntityIds.clear();
        const int32_t objectIndexListSize = m_objectIndexList->getSize();
        const int32_t* objectIndexListPtr = m_objectIndexList->getPtr();
        m_visibleEntityIds.clear();
        m_visibleEntityIds.reserve(objectIndexListSize);
        for (int32_t i = 0; i < objectIndexListSize; ++i)
        {
            const int32_t objectIndex = objectIndexListPtr[i];
            if (m_controller.m_objectIndexToEntityIdTable.size() > objectIndex)
            {
                m_visibleEntityIds.insert(m_controller.m_objectIndexToEntityIdTable[objectIndex]);
            }
        }

        // Add any recorded data for debug lines and stats to the accumulated sets
        if (renderDebugInfo)
        {
            AZStd::scoped_lock<decltype(m_controller.m_debugMutex)> lock(m_controller.m_debugMutex);
            m_controller.m_debugLines.insert(m_debugRenderer->m_debugLines.begin(), m_debugRenderer->m_debugLines.end());
            m_controller.m_debugStats.insert(m_debugRenderer->m_debugStats.begin(), m_debugRenderer->m_debugStats.end());
        }

        return errorCode == Umbra::Query::ERROR_OK;
    }

    AzFramework::OcclusionState UmbraSceneComponentController::UmbraOcclusionView::GetEntityVisibility(const AZ::EntityId& entityId) const
    {
        AZ_Assert(m_controller.IsSceneReady(), "Umbra scene component controller and assets must be valid and ready before query.");
        AZ_Assert(m_query, "Umbra query object must be valid before query.");
        AZ_Assert(m_objectIndexList, "Umbra object index list must be valid before query.");

        // If the occlusion data or query are not ready or the entity ID is not correspond to one that was recorded with the scene then assume it is not occluded.
        if (!m_controller.IsSceneReady() || !m_query || !m_objectIndexList || !entityId.IsValid() || !m_controller.m_storedEntityIds.contains(entityId))
        {
            return AzFramework::OcclusionState::Unknown;
        }

        // If the entity ID is part of the baked scene data and found in the visible entity ID list then it is visible.
        return m_visibleEntityIds.contains(entityId) ? AzFramework::OcclusionState::Visible : AzFramework::OcclusionState::Hidden;
    }

    AzFramework::OcclusionState UmbraSceneComponentController::UmbraOcclusionView::GetAabbVisibility(const AZ::Aabb& bounds) const
    {
        AZ_Assert(m_occlusionBuffer, "Umbra occlusion buffer must be instantiated before query.");

        if (m_occlusionBuffer && bounds.IsValid())
        {
            Umbra::Vector3 min = {};
            Umbra::Vector3 max = {};
            bounds.GetMin().StoreToFloat3(min.v);
            bounds.GetMax().StoreToFloat3(max.v);
            return m_occlusionBuffer->isAABBVisible(min, max) ? AzFramework::OcclusionState::Visible : AzFramework::OcclusionState::Hidden;
        }

        return AzFramework::OcclusionState::Unknown;
    }

    AZStd::vector<AzFramework::OcclusionState> UmbraSceneComponentController::UmbraOcclusionView::GetAabbToAabbVisibility(const AZ::Aabb& sourceAabb, const AZStd::vector<AZ::Aabb>& targetAabbs) const
    {
        AZ_Assert(m_controller.IsSceneReady(), "Umbra scene component controller and assets must be valid and ready before query.");
        AZ_Assert(m_query, "Umbra query object must be valid before query.");

        if (!m_controller.IsSceneReady() || !m_query)
        {
            return {};
        }

        // Return early if any of the bounding boxes are invalid or there are no target bounding boxes to test
        if (!sourceAabb.IsValid() || targetAabbs.empty() || !AZStd::all_of(targetAabbs.begin(), targetAabbs.end(), [](const auto& aabb) { return aabb.IsValid(); }))
        {
            return {};
        }

        // Convert all of the O3DE data types into Umbra data types for the query
        Umbra::Vector3 sourceMin = {};
        Umbra::Vector3 sourceMax = {};
        sourceAabb.GetMin().StoreToFloat3(sourceMin.v);
        sourceAabb.GetMax().StoreToFloat3(sourceMax.v);

        const int targetCount = aznumeric_cast<int>(targetAabbs.size());
        AZStd::vector<Umbra::Vector3> targetMin(targetCount, Umbra::Vector3{});
        AZStd::vector<Umbra::Vector3> targetMax(targetCount, Umbra::Vector3{});
        for (int i = 0; i < targetCount; ++i)
        {
            targetAabbs[i].GetMin().StoreToFloat3(targetMin[i].v);
            targetAabbs[i].GetMax().StoreToFloat3(targetMax[i].v);
        }

        // Create a container for all of the intersection results, defaulting all objects to being visible.
        AZStd::vector<Umbra::QueryExt::IntersectionResult> intersections(targetCount, Umbra::QueryExt::IntersectionResult::IRESULT_NO_INTERSECTION);

        const Umbra::Query::ErrorCode errorCode = m_query->queryAABBVisibility(sourceMin, sourceMax, targetMin.data(), targetMax.data(), targetCount, 0, intersections.data());
        if (errorCode != Umbra::Query::ErrorCode::ERROR_OK)
        {
            Internal::PrintQueryErrorMessage(errorCode);
            return {};
        }

        // Convert all of the intersection data into flags, returning true if source and target into these can see each other. 
        AZStd::vector<AzFramework::OcclusionState> results(targetCount, AzFramework::OcclusionState::Unknown);
        for (int i = 0; i < targetCount; ++i)
        {
            switch (intersections[i])
            {
            case Umbra::QueryExt::IntersectionResult::IRESULT_INTERSECTION:
                results[i] = AzFramework::OcclusionState::Hidden;
                break;
            case Umbra::QueryExt::IntersectionResult::IRESULT_NO_INTERSECTION:
                results[i] = AzFramework::OcclusionState::Visible;
                break;
            default:
                results[i] = AzFramework::OcclusionState::Unknown;
                break;
            }
        }
        return results;
    }

    AZStd::vector<AzFramework::OcclusionState> UmbraSceneComponentController::UmbraOcclusionView::GetSphereToSphereVisibility(const AZ::Sphere& sourceSphere, const AZStd::vector<AZ::Sphere>& targetSpheres) const
    {
        AZ_Assert(m_controller.IsSceneReady(), "Umbra scene component controller and assets must be valid and ready before query.");
        AZ_Assert(m_query, "Umbra query object must be valid before query.");

        if (!m_controller.IsSceneReady() || !m_query || targetSpheres.empty())
        {
            return {};
        }

        // Convert all of the O3DE data types into Umbra data types for the query
        Umbra::Sphere source = {};
        sourceSphere.GetCenter().StoreToFloat3(source.center.v);
        source.radius = sourceSphere.GetRadius();

        const int targetCount = aznumeric_cast<int>(targetSpheres.size());
        AZStd::vector<Umbra::Sphere> targets(targetCount, Umbra::Sphere{});
        for (int i = 0; i < targetCount; ++i)
        {
            targetSpheres[i].GetCenter().StoreToFloat3(targets[i].center.v);
            targets[i].radius = targetSpheres[i].GetRadius();
        }

        // Create a container for all of the intersection results, defaulting all objects to being visible.
        AZStd::vector<Umbra::QueryExt::IntersectionResult> intersections(targetCount, Umbra::QueryExt::IntersectionResult::IRESULT_NO_INTERSECTION);

        const Umbra::Query::ErrorCode errorCode = m_query->querySphereVisibility(source, targets.data(), targetCount, 0, intersections.data());
        if (errorCode != Umbra::Query::ErrorCode::ERROR_OK)
        {
            Internal::PrintQueryErrorMessage(errorCode);
            return {};
        }

        // Convert all of the intersection data into flags, returning true if source and target can see each other. 
        AZStd::vector<AzFramework::OcclusionState> results(targetCount, AzFramework::OcclusionState::Unknown);
        for (int i = 0; i < targetCount; ++i)
        {
            switch (intersections[i])
            {
            case Umbra::QueryExt::IntersectionResult::IRESULT_INTERSECTION:
                results[i] = AzFramework::OcclusionState::Hidden;
                break;
            case Umbra::QueryExt::IntersectionResult::IRESULT_NO_INTERSECTION:
                results[i] = AzFramework::OcclusionState::Visible;
                break;
            default:
                results[i] = AzFramework::OcclusionState::Unknown;
                break;
            }
        }
        return results;
    }

    AZStd::vector<AzFramework::OcclusionState> UmbraSceneComponentController::UmbraOcclusionView::GetEntityToEntityVisibility(const AZ::EntityId& sourceEntityId, const AZStd::vector<AZ::EntityId>& targetEntityIds) const
    {
        // Retrieve the world bounding box for the source entity and use it to perform visibility checks against the other entities.
        const AZ::Aabb sourceAabb = Internal::GetEntityAabb(sourceEntityId);

        const int targetCount = aznumeric_cast<int>(targetEntityIds.size());

        // Retrieve world bounding boxes for all of the target entities to be tested.
        AZStd::vector<AZ::Aabb> targetAabbs(targetCount, AZ::Aabb::CreateFromPoint(AZ::Vector3::CreateZero()));
        for (int i = 0; i < targetCount; ++i)
        {
            targetAabbs[i] = Internal::GetEntityAabb(targetEntityIds[i]);
        }

        return GetAabbToAabbVisibility(sourceAabb, targetAabbs);
    }
} // namespace Umbra