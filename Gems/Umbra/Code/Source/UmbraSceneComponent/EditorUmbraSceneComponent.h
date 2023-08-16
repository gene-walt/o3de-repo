/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzToolsFramework/ToolsComponents/EditorComponentAdapter.h>
#include <Umbra/UmbraSceneComponent/UmbraSceneComponentBus.h>
#include <Umbra/UmbraSceneComponent/UmbraSceneComponentConstants.h>
#include <UmbraSceneComponent/UmbraSceneComponent.h>

namespace Umbra
{
    //! Implements editor specific features for corresponding component and controller. This includes UI and logic for generating umbra
    //! scene and descriptor files from the currently loaded level.
    class EditorUmbraSceneComponent final
        : public AzToolsFramework::Components::EditorComponentAdapter<UmbraSceneComponentController, UmbraSceneComponent, UmbraSceneComponentConfig>
        , public UmbraSceneComponentNotificationBus::Handler
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

        AZ::u32 GenerateUmbraScene();

        bool GenerateUmbraSceneFromLevel(const AZStd::string& scenePath) const;

        static constexpr const char* GenerateUmbraSceneButtonText = "Generate Umbra Scene...";
        static constexpr const char* GenerateUmbraSceneToolTipText = "Generate and save an umbra scene from level entities.";
    };
} // namespace Umbra
