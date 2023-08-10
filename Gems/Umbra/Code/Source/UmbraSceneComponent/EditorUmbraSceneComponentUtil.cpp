/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/Math/Matrix4x4.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/Serialization/Json/JsonUtils.h>
#include <AzCore/Utils/Utils.h>
#include <AzFramework/Visibility/BoundsBus.h>
#include <AzFramework/Visibility/VisibleGeometryBus.h>
#include <Umbra/UmbraObjectComponent/UmbraObjectComponentBus.h>
#include <Umbra/UmbraSceneAsset/UmbraSceneAsset.h>
#include <Umbra/UmbraViewVolumeComponent/UmbraViewVolumeComponentBus.h>
#include <UmbraSceneAsset/UmbraSceneDescriptor.h>
#include <UmbraSceneComponent/EditorUmbraSceneComponentUtil.h>

#include <umbra/optimizer/umbraScene.hpp>
#include <umbra/umbraInfo.hpp>

namespace Umbra
{
    bool ExportUmbraSceneFromLevel(const AZStd::string& scenePath, float collisionRadius, float smallestHole, float smallestOccluder)
    {
        if (scenePath.empty() || AZ::StringFunc::Path::IsRelative(scenePath.c_str()) ||
            !AZ::StringFunc::Path::IsExtension(scenePath.c_str(), "umbrascene"))
        {
            AZ_Error("Umbra", false, "Scene export path is not valid: %s", scenePath.c_str());
            return false;
        }

        // Create an umbra scene
        Umbra::Scene* scene = Umbra::Scene::create();

        UmbraSceneDescriptor sceneDescriptor;
        sceneDescriptor.m_collisionRadius = collisionRadius;
        sceneDescriptor.m_smallestHole = smallestHole;
        sceneDescriptor.m_smallestOccluder = smallestOccluder;
        AZ::StringFunc::Path::GetFullFileName(scenePath.c_str(), sceneDescriptor.m_sceneFilename);

        AzFramework::VisibleGeometryRequestBus::EnumerateHandlers(
            [&sceneDescriptor, scene](AzFramework::VisibleGeometryRequestBus::Events* handler) -> bool
            {
                const AZ::EntityId entityId = *(AzFramework::VisibleGeometryRequestBus::GetCurrentBusId());

                // Get occlusion settings from object component if one is present
                // UmbraObjectComponentRequestBus::Event(entityId, [](UmbraObjectComponentRequestBus::Events* handler) {
                //    });

                // Use the transform bus to determine if the object has a static transform
                // AZ::TransformBus::Event(entityId, [](AZ::TransformBus::Events* handler) {
                //    });

                AzFramework::VisibleGeometryContainer geometryContainer;
                handler->GetVisibleGeometry(AZ::Aabb::CreateNull(), geometryContainer);

                uint32_t partId = 0;
                for (const auto& geometry : geometryContainer)
                {
                    const Umbra::SceneModel* model = scene->insertModel(
                        geometry.m_vertices.data(),
                        geometry.m_indices.data(),
                        aznumeric_cast<int>(geometry.m_vertices.size() / 3),
                        aznumeric_cast<int>(geometry.m_indices.size() / 3));

                    Umbra::Matrix4x4 transform = {};
                    geometry.m_transform.StoreToColumnMajorFloat16(reinterpret_cast<float*>(transform.m));

                    scene->insertObject(
                        model,
                        transform,
                        aznumeric_cast<uint32_t>(sceneDescriptor.m_objectDescriptors.size()),
                        Umbra::SceneObject::OCCLUDER | Umbra::SceneObject::TARGET,
                        Umbra::MF_COLUMN_MAJOR);

                    UmbraObjectDescriptor objectDescriptor;
                    objectDescriptor.m_entityId = entityId;
                    objectDescriptor.m_partId = partId++;
                    sceneDescriptor.m_objectDescriptors.emplace_back(AZStd::move(objectDescriptor));
                }
                return true;
            });

        UmbraViewVolumeComponentRequestBus::EnumerateHandlers(
            [&sceneDescriptor, scene]([[maybe_unused]] UmbraViewVolumeComponentRequestBus::Events* handler) -> bool
            {
                const AZ::EntityId entityId = *(UmbraViewVolumeComponentRequestBus::GetCurrentBusId());

                AZ::Aabb bounds = AZ::Aabb::CreateNull();
                AzFramework::BoundsRequestBus::EventResult(bounds, entityId, &AzFramework::BoundsRequestBus::Events::GetWorldBounds);
                if (bounds.IsValid() && bounds.IsFinite())
                {
                    // Create a view volume to determine the area in the scene where camera is allowed to move.
                    Umbra::Vector3 min = {};
                    bounds.GetMin().StoreToFloat3(min.v);
                    Umbra::Vector3 max = {};
                    bounds.GetMax().StoreToFloat3(max.v);
                    scene->insertViewVolume(min, max, aznumeric_cast<uint32_t>(sceneDescriptor.m_objectDescriptors.size()));

                    UmbraObjectDescriptor objectDescriptor;
                    objectDescriptor.m_entityId = entityId;
                    objectDescriptor.m_partId = 0;
                    sceneDescriptor.m_objectDescriptors.emplace_back(AZStd::move(objectDescriptor));
                }
                return true;
            });

        // The scene will be written to a file that can be picked up by the asset processor and converted into an umbra tome.
        if (!scene->writeToFile(scenePath.c_str()))
        {
            AZ_Error("Umbra", false, "Failed to save umbra scene: %s", scenePath.c_str());
            scene->release();
            return false;
        }

        scene->release();

        AZStd::string sceneDescPath = scenePath;
        AZ::StringFunc::Path::ReplaceExtension(sceneDescPath, UmbraSceneDescriptor::Extension);
        if (!AZ::JsonSerializationUtils::SaveObjectToFile(&sceneDescriptor, sceneDescPath))
        {
            AZ_Error("Umbra", false, "Failed to save umbra scene descriptor: %s", sceneDescPath.c_str());
            return false;
        }

        return true;
    }
} // namespace Umbra