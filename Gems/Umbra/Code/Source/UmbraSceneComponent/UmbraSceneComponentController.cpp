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

    bool UmbraSceneComponentController::CreateOcclusionView(const AZ::Name& name)
    {
        if (!IsSceneReady())
        {
            return false;
        }

        return m_occlusionViewMap.emplace(name, AZStd::make_unique<UmbraOcclusionView>(*this)).second;
    }

    bool UmbraSceneComponentController::DestroyOcclusionView(const AZ::Name& name)
    {
        return m_occlusionViewMap.erase(name) != 0;
    }

    bool UmbraSceneComponentController::UpdateOcclusionView(
        const AZ::Name& name, const AZ::Vector3& cameraWorldPos, const AZ::Matrix4x4& cameraWorldToClip)
    {
        const auto occlusionViewItr = m_occlusionViewMap.find(name);
        return occlusionViewItr != m_occlusionViewMap.end() ? occlusionViewItr->second->Update(cameraWorldPos, cameraWorldToClip) : false;
    }

    bool UmbraSceneComponentController::IsEntityVisibleInOcclusionView(const AZ::Name& name, const AZ::EntityId& entityId) const
    {
        const auto occlusionViewItr = m_occlusionViewMap.find(name);
        return occlusionViewItr != m_occlusionViewMap.end() ? occlusionViewItr->second->IsEntityVisible(entityId) : true;
    }

    bool UmbraSceneComponentController::IsAabbVisibleInOcclusionView(const AZ::Name& name, const AZ::Aabb& bounds) const
    {
        const auto occlusionViewItr = m_occlusionViewMap.find(name);
        return occlusionViewItr != m_occlusionViewMap.end() ? occlusionViewItr->second->IsAabbVisible(bounds) : true;
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
            m_occlusionEntities.clear();
            m_occlusionEntities.reserve(m_configuration.m_sceneAsset->GetObjectDescriptors().size());
            for (const auto& objectDesc : m_configuration.m_sceneAsset->GetObjectDescriptors())
            {
                m_occlusionEntities.insert(objectDesc.m_entityId);
            }
        }
    }

    void UmbraSceneComponentController::ReleaseScene()
    {
        m_occlusionViewMap.clear();
        m_occlusionEntities.clear();

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

        m_query.reset(new Umbra::Query(m_controller.m_tome));
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

        const Umbra::Query::ErrorCode result = m_query->queryPortalVisibility(flags, visibility, camera);
        switch (result)
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
            AZ_Warning("UmbraSceneComponent", false, "queryPortalVisibility returned unsupported result value.");
            break;
        }

        // Add any recorded data for debug lines and stats to the accumulated sets
        if (renderDebugInfo)
        {
            AZStd::scoped_lock<decltype(m_controller.m_debugMutex)> lock(m_controller.m_debugMutex);
            m_controller.m_debugLines.insert(m_debugRenderer->m_debugLines.begin(), m_debugRenderer->m_debugLines.end());
            m_controller.m_debugStats.insert(m_debugRenderer->m_debugStats.begin(), m_debugRenderer->m_debugStats.end());
        }

        return result == Umbra::Query::ERROR_OK;
    }

    bool UmbraSceneComponentController::UmbraOcclusionView::IsEntityVisible(const AZ::EntityId& entityId) const
    {
        AZ_Assert(m_controller.IsSceneReady(), "Umbra scene component controller and assets must be valid and ready before query.");
        AZ_Assert(m_query, "Umbra query object must be valid before query.");
        AZ_Assert(m_objectIndexList, "Umbra object index list must be valid before query.");

        if (!m_controller.IsSceneReady() || !m_query || !m_objectIndexList)
        {
            return true;
        }

        // If the entity being tested was part of the precomputed visibility data then search for and return its state for the current view.
        if (m_controller.m_occlusionEntities.contains(entityId))
        {
            const auto& objectDescs = m_controller.m_configuration.m_sceneAsset->GetObjectDescriptors();
            const int32_t objectIndexListSize = m_objectIndexList->getSize();
            const int32_t* objectIndexListPtr = m_objectIndexList->getPtr();
            for (int32_t i = 0; i < objectIndexListSize; ++i)
            {
                const int32_t objectIndex = objectIndexListPtr[i];
                const uint32_t userIndex = m_controller.m_tome->getObjectUserID(objectIndex);
                if (userIndex < objectDescs.size() && objectDescs[userIndex].m_entityId == entityId)
                {
                    return true;
                }
            }

            return false;
        }

        // The entity being tested is not part of the precomputed occlusion scene. Check its bounding box against the occlusion buffer.
        AZ::Aabb bounds = AZ::Aabb::CreateNull();
        AzFramework::BoundsRequestBus::EventResult(bounds, entityId, &AzFramework::BoundsRequestBus::Events::GetWorldBounds);
        return IsAabbVisible(bounds);
    }

    bool UmbraSceneComponentController::UmbraOcclusionView::IsAabbVisible(const AZ::Aabb& bounds) const
    {
        AZ_Assert(m_occlusionBuffer, "Umbra occlusion buffer must be instantiated before query.");

        if (m_occlusionBuffer && bounds.IsValid())
        {
            Umbra::Vector3 min = {};
            Umbra::Vector3 max = {};
            bounds.GetMin().StoreToFloat3(min.v);
            bounds.GetMax().StoreToFloat3(max.v);
            return m_occlusionBuffer->isAABBVisible(min, max);
        }
 
        return true;
    }
} // namespace Umbra