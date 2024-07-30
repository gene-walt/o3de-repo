/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzToolsFramework/ToolsComponents/EditorComponentAdapter.h>
#include <Umbra/UmbraSceneComponent/EditorUmbraSceneComponentBus.h>
#include <Umbra/UmbraSceneComponent/UmbraSceneComponentConstants.h>
#include <UmbraSceneComponent/UmbraSceneComponent.h>

namespace Umbra
{
    //! Implements editor specific features for corresponding component and controller. This includes UI and logic for generating umbra
    //! scene and descriptor files from the currently loaded level.
    class EditorUmbraSceneComponent final
        : public AzToolsFramework::Components::EditorComponentAdapter<UmbraSceneComponentController, UmbraSceneComponent, UmbraSceneComponentConfig>
        , public EditorUmbraSceneComponentRequestBus::Handler
    {
    public:
        using BaseClass = AzToolsFramework::Components::EditorComponentAdapter<UmbraSceneComponentController, UmbraSceneComponent, UmbraSceneComponentConfig>;
        AZ_EDITOR_COMPONENT(EditorUmbraSceneComponent, EditorUmbraSceneComponentTypeId, BaseClass);

        static void Reflect(AZ::ReflectContext* context);

        EditorUmbraSceneComponent() = default;
        EditorUmbraSceneComponent(const UmbraSceneComponentConfig& config);

    private:
        // BaseClass overrides ...
        void Activate() override;
        void Deactivate() override;

        // EditorUmbraSceneComponentRequestBus::Handler overrides ...
        bool ExportUmbraScene(const AZStd::string& scenePath) const override;
        bool ShouldExportEntity(const AZ::EntityId& entityId) const override;

        AZ::u32 ExportUmbraSceneFromUI();
        AzFramework::EntityContextId GetEntityContextId(const AZ::EntityId& entityId) const;

        AzFramework::EntityContextId m_contextId = AzFramework::EntityContextId::CreateNull();

        static constexpr const char* ExportUmbraSceneButtonText = "Export Umbra Scene...";
        static constexpr const char* ExportUmbraSceneToolTipText = "Export and save an umbra scene from level entities.";
    };
} // namespace Umbra
