/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/Math/Matrix4x4.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <Umbra/UmbraTomeAsset/UmbraTomeAsset.h>
#include <UmbraLevelComponent/EditorUmbraLevelComponent.h>
#include <UmbraLevelComponent/EditorUmbraLevelComponentUtil.h>

#include <umbra/optimizer/umbraScene.hpp>
#include <umbra/umbraInfo.hpp>

namespace Umbra
{
    void EditorUmbraLevelComponent::Reflect(AZ::ReflectContext* context)
    {
        BaseClass::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<EditorUmbraLevelComponent, BaseClass>()
                ->Version(0)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<EditorUmbraLevelComponent>(
                    "UmbraLevelComponent", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::Category, "Umbra")
                        ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                        ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Icons/Components/Viewport/Component_Placeholder.svg")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Level"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->Attribute(AZ::Edit::Attributes::HelpPageURL, "")
                        ->Attribute(AZ::Edit::Attributes::PrimaryAssetType, AZ::AzTypeInfo<Umbra::UmbraTomeAsset>::Uuid())
                        ->UIElement(AZ::Edit::UIHandlers::Button, GenerateUmbraSceneButtonText, GenerateUmbraSceneToolTipText)
                            ->Attribute(AZ::Edit::Attributes::NameLabelOverride, "")
                            ->Attribute(AZ::Edit::Attributes::ButtonText, GenerateUmbraSceneButtonText)
                            ->Attribute(AZ::Edit::Attributes::ChangeNotify, &EditorUmbraLevelComponent::GenerateUmbraScene)
                    ;

                editContext->Class<UmbraLevelComponentConfig>(
                    "UmbraLevelComponentConfig", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Hide)
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->ConstantProperty("EditorUmbraLevelComponentTypeId", BehaviorConstant(AZ::Uuid(EditorUmbraLevelComponentTypeId)))
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation)
                ;
        }
    }

    EditorUmbraLevelComponent::EditorUmbraLevelComponent(const UmbraLevelComponentConfig& config)
        : BaseClass(config)
    {
    }

    void EditorUmbraLevelComponent::Activate()
    {
        BaseClass::Activate();
        UmbraLevelComponentNotificationBus::Handler::BusConnect(GetEntityId());
    }

    void EditorUmbraLevelComponent::Deactivate()
    {
        UmbraLevelComponentNotificationBus::Handler::BusDisconnect();
        BaseClass::Deactivate();
    }

    AZ::u32 EditorUmbraLevelComponent::GenerateUmbraScene()
    {
        // Create the scene
        Umbra::Scene* scene = Umbra::Scene::create();

        // Populate the scene with Models (geometry) and
        // their transformed instances (Objects). Normally,
        // the editor would do this, but in this case we
        // just create a simple scene programmatically.

        // Our scene has only one box model, but multiple instances
        float vertices[8 * 3] = {
            -1.f, -1.f, -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, -1.f, -1.f, 1.f, -1.f,
            -1.f, -1.f, 1.f,  1.f, -1.f, 1.f,  1.f, 1.f, 1.f,  -1.f, 1.f, 1.f,
        };
        UINT16 indices[12 * 3] = {
            0, 2, 1, 0, 3, 2, 1, 6, 5, 1, 2, 6, 4, 7, 0, 7, 3, 0, 7, 6, 3, 6, 2, 3, 4, 0, 5, 0, 1, 5, 5, 7, 4, 5, 6, 7,
        };
        const Umbra::SceneModel* box = scene->insertModel(vertices, indices, 8, 12);

        // Insert ten box objects into the scene
        for (int i = 0; i < 10; i++)
        {
            // every other box in the positive Z half-space
            Umbra::Matrix4x4 mtx;
            const AZ::Matrix4x4 mtxAZ = AZ::Matrix4x4::CreateTranslation(AZ::Vector3(-10.f + i / 2 * 5.f, 0.f, (i & 1) ? -5.f : 5.f));
            mtxAZ.StoreToColumnMajorFloat16(reinterpret_cast<float*>(mtx.m));

            scene->insertObject(
                box, // box model
                mtx, // transformation
                (Umbra::UINT32)i, // id
                Umbra::SceneObject::OCCLUDER | Umbra::SceneObject::TARGET, // flags (object occludes and can be occluded)
                Umbra::MF_COLUMN_MAJOR); // matrix format, translation in mem locations (12, 13, 14)
        }

        // Insert a large wall between the half-spaces
        Umbra::Matrix4x4 mtx;
        const AZ::Matrix4x4 mtxAZ = AZ::Matrix4x4::CreateScale(AZ::Vector3(15.f, 15.f, 1.f));
        mtxAZ.StoreToColumnMajorFloat16(reinterpret_cast<float*>(mtx.m));

        scene->insertObject(
            box, // box model
            mtx, // transformation
            10, // id
            Umbra::SceneObject::OCCLUDER | Umbra::SceneObject::TARGET, // flags (object occludes and can be occluded)
            Umbra::MF_COLUMN_MAJOR); // matrix format, translation in mem locations (12, 13, 14)

        // Create a view volume.
        // View volumes determine the area in the scene where camera is allowed to move.
        Umbra::Vector3 mn = { { -25.f, -25.f, -25.f } };
        Umbra::Vector3 mx = { { 25.f, 25.f, 25.f } };
        scene->insertViewVolume(mn, mx, 0);

        // Scene can be serialized into a file. The resulting .scene
        // file can be opened and inspected in umbra viewer.
        // This serialization step isn't required for visibility computation.
        const AZStd::string path = "D:/projects/amzn/O3DEUmbra3Gem/Gems/Umbra/Assets/test.umbrascene";
        scene->writeToFile(path.c_str());
        scene->release();

        return AZ::Edit::PropertyRefreshLevels::EntireTree;
    }
} // namespace Umbra
