/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/Math/Matrix4x4.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <UmbraSceneComponent/UmbraSceneComponentController.h>
#include <AzFramework/Visibility/BoundsBus.h>

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
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<UmbraSceneComponentRequestBus>("UmbraSceneComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
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
        UmbraSceneComponentRequestBus::Handler::BusConnect(m_entityId);
        QueueScene();
    }

    void UmbraSceneComponentController::Deactivate()
    {
        AZ::Data::AssetBus::Handler::BusDisconnect();
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

    bool UmbraSceneComponentController::UpdateVisibility(const AZ::Matrix4x4& cameraWorldToClip, const AZ::Vector3& cameraWorldPos)
    {
        if (!m_tome || !m_query || !m_objectIndexList || !m_occlusionBuffer)
        {
            return false;
        }

        // Reset the object index list and occlusion buffer
        m_objectIndexList->setSize(0);
        m_occlusionBuffer->clear();

        // Set up the visibility query parameters to fill in the object index list and occlusion buffer
        Umbra::Visibility visibility(m_objectIndexList.get(), m_occlusionBuffer.get());

        // Convert the incoming camera transform into an umbra camera transform
        Umbra::Matrix4x4 worldToClip = {};
        cameraWorldToClip.StoreToColumnMajorFloat16(reinterpret_cast<float*>(worldToClip.m));

        Umbra::Vector3 cameraPos = {};
        cameraWorldPos.StoreToFloat3(cameraPos.v);

        Umbra::CameraTransform camera(worldToClip, cameraPos);

        // Perform the visibility query
        if (m_query->queryPortalVisibility(0, visibility, camera) != Umbra::Query::ERROR_OK)
        {
            return false;
        }

        return true;
    }

    bool UmbraSceneComponentController::IsEntityVisible(const AZ::EntityId& entityId) const
    {
        if (!m_tome || !m_query || !m_objectIndexList || !m_occlusionBuffer || !m_configuration.m_sceneAsset->IsReady())
        {
            return false;
        }

        if (m_occlusionEntities.find(entityId) != m_occlusionEntities.end())
        {
            const auto& objectDescs = m_configuration.m_sceneAsset->GetObjectDescriptors();
            const int32_t objectIndexListSize = m_objectIndexList->getSize();
            const int32_t* objectIndexListPtr = m_objectIndexList->getPtr();
            for (int32_t i = 0; i < objectIndexListSize; ++i)
            {
                const int32_t objectIndex = objectIndexListPtr[i];
                const uint32_t userIndex = m_tome->getObjectUserID(objectIndex);
                if (userIndex < objectDescs.size() && objectDescs[userIndex].m_entityId == entityId)
                {
                    return true;
                }
            }

            return false;
        }

        AZ::Aabb bounds = AZ::Aabb::CreateNull();
        AzFramework::BoundsRequestBus::EventResult(bounds, entityId, &AzFramework::BoundsRequestBus::Events::GetWorldBounds);
        return IsAabbVisible(bounds);
    }

    bool UmbraSceneComponentController::IsAabbVisible(const AZ::Aabb& bounds) const
    {
        if (m_occlusionBuffer && bounds.IsValid())
        {
            Umbra::Vector3 min = {};
            bounds.GetMin().StoreToFloat3(min.v);
            Umbra::Vector3 max = {};
            bounds.GetMax().StoreToFloat3(max.v);
            return m_occlusionBuffer->isAABBVisible(min, max);
        }

        return false;
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

            m_query.reset(new Umbra::Query(m_tome));
            m_objectIndexListStorage = AZStd::vector<int32_t>(m_tome->getObjectCount());
            m_objectIndexList.reset(new Umbra::IndexList(m_objectIndexListStorage.data(), m_tome->getObjectCount()));
            m_occlusionBuffer.reset(new Umbra::OcclusionBuffer());

            // Reserve and build a set of entity IDs stored in the scene asset object descriptor table. This will be used to determine if
            // occlusion tests can be based off of the object index list or if occlusion buffer tests are required.
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
        m_query.reset();
        m_objectIndexList.reset();
        m_objectIndexListStorage.clear();
        m_occlusionBuffer.reset();
        m_occlusionEntities.clear();

        if (m_tome)
        {
            Umbra::TomeLoader::freeTome(m_tome);
            m_tome = nullptr;
        }
    }
} // namespace Umbra