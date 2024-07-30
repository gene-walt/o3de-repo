/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/Math/Matrix4x4.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/Json/JsonUtils.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Utils/Utils.h>
#include <AzFramework/Asset/AssetSystemBus.h>
#include <AzFramework/Visibility/BoundsBus.h>
#include <AzFramework/Visibility/VisibleGeometryBus.h>
#include <AzQtComponents/Components/Widgets/FileDialog.h>
#include <AzToolsFramework/API/EditorAssetSystemAPI.h>
#include <AzToolsFramework/UI/UICore/WidgetHelpers.h>
#include <Umbra/UmbraObjectComponent/UmbraObjectComponentBus.h>
#include <Umbra/UmbraSceneAsset/UmbraSceneAsset.h>
#include <Umbra/UmbraViewVolumeComponent/UmbraViewVolumeComponentBus.h>
#include <UmbraSceneAsset/UmbraSceneDescriptor.h>
#include <UmbraSceneComponent/EditorUmbraSceneComponent.h>

#include <optimizer/umbraScene.hpp>
#include <umbraInfo.hpp>

#include <QMessageBox>
#include <QObject>

namespace Umbra
{
    void EditorUmbraSceneComponent::Reflect(AZ::ReflectContext* context)
    {
        BaseClass::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<EditorUmbraSceneComponent, BaseClass>()
                ->Version(0)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<EditorUmbraSceneComponent>(
                    "Umbra Scene", "This level component allows exporting scene geometry and managing occlusion culling requests with Umbra.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::Category, "Umbra")
                        ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                        ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Icons/Components/Viewport/Component_Placeholder.svg")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Level"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->Attribute(AZ::Edit::Attributes::HelpPageURL, "https://umbra3d.com/")
                        ->Attribute(AZ::Edit::Attributes::PrimaryAssetType, AZ::AzTypeInfo<Umbra::UmbraSceneAsset>::Uuid())
                        ->UIElement(AZ::Edit::UIHandlers::Button, ExportUmbraSceneButtonText, ExportUmbraSceneToolTipText)
                            ->Attribute(AZ::Edit::Attributes::NameLabelOverride, "")
                            ->Attribute(AZ::Edit::Attributes::ButtonText, ExportUmbraSceneButtonText)
                            ->Attribute(AZ::Edit::Attributes::ChangeNotify, &EditorUmbraSceneComponent::ExportUmbraSceneFromUI)
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->ConstantProperty("EditorUmbraSceneComponentTypeId", BehaviorConstant(AZ::Uuid(EditorUmbraSceneComponentTypeId)))
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation)
                ;

            behaviorContext->EBus<EditorUmbraSceneComponentRequestBus>("EditorUmbraSceneComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Event("ExportUmbraScene", &EditorUmbraSceneComponentRequestBus::Events::ExportUmbraScene)
                ->Event("ShouldExportEntity", &EditorUmbraSceneComponentRequestBus::Events::ShouldExportEntity)
                ;
        }
    }

    EditorUmbraSceneComponent::EditorUmbraSceneComponent(const UmbraSceneComponentConfig& sceneComponentConfig)
        : BaseClass(sceneComponentConfig)
    {
    }

    void EditorUmbraSceneComponent::Activate()
    {
        BaseClass::Activate();
        EditorUmbraSceneComponentRequestBus::Handler::BusConnect(GetEntityId());
        m_contextId = GetEntityContextId(GetEntityId());
    }

    void EditorUmbraSceneComponent::Deactivate()
    {
        EditorUmbraSceneComponentRequestBus::Handler::BusDisconnect();
        BaseClass::Deactivate();
    }

    AZ::u32 EditorUmbraSceneComponent::ExportUmbraSceneFromUI()
    {
        // Before prompting the user for a scene file path, initialize it with the level name
        AZStd::string levelName;
        AzToolsFramework::EditorRequestBus::BroadcastResult(levelName, &AzToolsFramework::EditorRequestBus::Events::GetLevelName);
        AZStd::to_lower(levelName.begin(), levelName.end());

        AZStd::string scenePathSeed = AZ::Utils::GetProjectPath().c_str();
        if (!levelName.empty())
        {
            scenePathSeed += "/";
            scenePathSeed += levelName;
            scenePathSeed += ".umbrascene";
        }

        // Prompt the user for a path to save the umbra scene
        const AZStd::string scenePath = AzQtComponents::FileDialog::GetSaveFileName(
            AzToolsFramework::GetActiveWindow(),
            QString("Select Umbra Scene File Path"),
            scenePathSeed.c_str(),
            QString("Umbra Scene (*.umbrascene)"),
            nullptr).toUtf8().constData();

        if (!scenePath.empty())
        {
            if (!ExportUmbraScene(scenePath))
            {
                QMessageBox::critical(
                    AzToolsFramework::GetActiveWindow(),
                    QObject::tr("Umbra Scene Not Saved"),
                    QObject::tr("Failed to save umbra scene from level entities."));
                return AZ::Edit::PropertyRefreshLevels::AttributesAndValues;
            }

            bool sceneAssetFound = false;
            AZ::Data::AssetInfo sceneAssetInfo;
            AZStd::string watchFolder;
            AzToolsFramework::AssetSystemRequestBus::BroadcastResult(
                sceneAssetFound,
                &AzToolsFramework::AssetSystemRequestBus::Events::GetSourceInfoBySourcePath,
                scenePath.c_str(),
                sceneAssetInfo,
                watchFolder);

            if (!sceneAssetFound)
            {
                QMessageBox::critical(
                    AzToolsFramework::GetActiveWindow(),
                    QObject::tr("Umbra Scene Asset Not Assigned"),
                    QObject::tr("Failed to assign the umbra scene asset generated from level entities after saving."));
                return AZ::Edit::PropertyRefreshLevels::AttributesAndValues;
            }

            AzToolsFramework::ScopedUndoBatch undoBatch("Setting scene asset.");
            SetDirty();

            UmbraSceneComponentRequestBus::Event(
                GetEntityId(),
                &UmbraSceneComponentRequestBus::Events::SetSceneAssetId,
                AZ::Data::AssetId(sceneAssetInfo.m_assetId.m_guid, 0));
        }

        return AZ::Edit::PropertyRefreshLevels::EntireTree;
    }

    bool EditorUmbraSceneComponent::ExportUmbraScene(const AZStd::string& scenePath) const
    {
        if (scenePath.empty() || AZ::StringFunc::Path::IsRelative(scenePath.c_str()) ||
            !AZ::StringFunc::Path::IsExtension(scenePath.c_str(), "umbrascene"))
        {
            AZ_Error("Umbra", false, "Scene export path is not valid: %s", scenePath.c_str());
            return false;
        }

        // Create an umbra scene
        Umbra::Scene* scene = Umbra::Scene::create();

        // Copy the global scene settings from the scene component into the scene descriptor.
        UmbraSceneDescriptor sceneDescriptor;
        sceneDescriptor.m_sceneSettings = m_controller.GetConfiguration().m_sceneSettings;

        // Enumerate every component connected to the visible geometry request bus and add all of the static geometry to the scene.
        AzFramework::VisibleGeometryRequestBus::EnumerateHandlers(
            [&sceneDescriptor, scene, this](AzFramework::VisibleGeometryRequestBus::Events* handler) -> bool
            {
                const AZ::EntityId entityId = *(AzFramework::VisibleGeometryRequestBus::GetCurrentBusId());
                if (!ShouldExportEntity(entityId))
                {
                    return true;
                }

                // Override the default scene object settings and flags with values from the umbra object component if one is present.
                uint32_t flags = Umbra::SceneObject::OCCLUDER | Umbra::SceneObject::TARGET;
                UmbraObjectComponentRequestBus::Event(entityId, [&flags](UmbraObjectComponentRequestBus::Events* handler) {
                    flags = 0;
                    flags |= (handler->GetCanOcclude() ? Umbra::SceneObject::OCCLUDER : 0);
                    flags |= (handler->GetCanBeOccluded() ? Umbra::SceneObject::TARGET : 0);
                });

                // Enumerate all visible geometry parts from components attached to this entity.
                AzFramework::VisibleGeometryContainer geometryContainer;
                handler->GetVisibleGeometry(AZ::Aabb::CreateNull(), geometryContainer);

                // Add each visible geometry part from the container to the umbra scene with a unique part ID.
                uint32_t partId = 0;
                for (const auto& geometry : geometryContainer)
                {
                    const uint32_t objectIndex = aznumeric_cast<uint32_t>(sceneDescriptor.m_objectDescriptors.size());

                    // Create and umbra model using the triangle list from the visible geometry.
                    // TODO Implement a mechanism to reuse shared model data if shapes or multiple instances of the same model are included in the level. 
                    const Umbra::SceneModel* model = scene->insertModel(
                        geometry.m_vertices.data(),
                        geometry.m_indices.data(),
                        aznumeric_cast<int>(geometry.m_vertices.size() / 3),
                        aznumeric_cast<int>(geometry.m_indices.size() / 3));

                    Umbra::Matrix4x4 transform = {};
                    geometry.m_transform.StoreToRowMajorFloat16(reinterpret_cast<float*>(transform.m));

                    // Transparent geometry will automatically be excluded as occluders.
                    if (geometry.m_transparent)
                    {
                        flags &= ~Umbra::SceneObject::OCCLUDER;
                    }

                    scene->insertObject(model, transform, objectIndex, flags, Umbra::MF_ROW_MAJOR);

                    UmbraObjectDescriptor objectDescriptor;
                    objectDescriptor.m_entityId = entityId;
                    objectDescriptor.m_partId = partId++;
                    sceneDescriptor.m_objectDescriptors.emplace_back(AZStd::move(objectDescriptor));
                }
                return true;
            });

        // Enumerate all view volumes and add them to the umbra seems to define the area where the camera is expected to move.
        UmbraViewVolumeComponentRequestBus::EnumerateHandlers(
            [&sceneDescriptor, scene, this]([[maybe_unused]] UmbraViewVolumeComponentRequestBus::Events* handler) -> bool
            {
                const AZ::EntityId entityId = *(UmbraViewVolumeComponentRequestBus::GetCurrentBusId());
                if (!ShouldExportEntity(entityId))
                {
                    return true;
                }

                // Use the entity world bounds to determine the bounding box for the view volume.
                AZ::Aabb bounds = AZ::Aabb::CreateNull();
                AzFramework::BoundsRequestBus::EventResult(bounds, entityId, &AzFramework::BoundsRequestBus::Events::GetWorldBounds);
                if (bounds.IsValid() && bounds.IsFinite())
                {
                    const uint32_t objectIndex = aznumeric_cast<uint32_t>(sceneDescriptor.m_objectDescriptors.size());

                    Umbra::Vector3 min = {};
                    bounds.GetMin().StoreToFloat3(min.v);
                    Umbra::Vector3 max = {};
                    bounds.GetMax().StoreToFloat3(max.v);
                    scene->insertViewVolume(min, max, objectIndex);

                    // If the view volume overrode scene settings then store them in the scene settings map for the builder.
                    if (handler->GetOverrideSceneSettings())
                    {
                        sceneDescriptor.m_sceneSettingsByView[objectIndex] = handler->GetSceneSettings();
                    }

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

    bool EditorUmbraSceneComponent::ShouldExportEntity(const AZ::EntityId& entityId) const
    {
        if (m_contextId != GetEntityContextId(entityId))
        {
            return false;
        }

        if (m_controller.GetConfiguration().m_exportStaticObjectsOnly)
        {
            // Use the transform bus to determine if the object has a static transform.
            bool isStatic = false;
            AZ::TransformBus::Event(entityId, [&isStatic](AZ::TransformBus::Events* handler) {
                isStatic = handler->IsStaticTransform();
            });

            if (!isStatic)
            {
                return false;
            }
        }
        return true;
    }

    AzFramework::EntityContextId EditorUmbraSceneComponent::GetEntityContextId(const AZ::EntityId& entityId) const
    {
        AzFramework::EntityContextId contextId = AzFramework::EntityContextId::CreateNull();
        AzFramework::EntityIdContextQueryBus::EventResult(
            contextId, entityId, &AzFramework::EntityIdContextQueryBus::Events::GetOwningContextId);
        return contextId;
    }
} // namespace Umbra
