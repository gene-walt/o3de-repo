/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzToolsFramework/ToolsComponents/EditorComponentAdapter.h>
#include <Umbra/UmbraLevelComponent/UmbraLevelComponentBus.h>
#include <Umbra/UmbraLevelComponent/UmbraLevelComponentConstants.h>
#include <UmbraLevelComponent/UmbraLevelComponent.h>

namespace Umbra
{
    //!
    class EditorUmbraLevelComponent final
        : public AzToolsFramework::Components::EditorComponentAdapter<UmbraLevelComponentController, UmbraLevelComponent, UmbraLevelComponentConfig>
        , public UmbraLevelComponentNotificationBus::Handler
    {
    public:
        using BaseClass = AzToolsFramework::Components::EditorComponentAdapter<UmbraLevelComponentController, UmbraLevelComponent, UmbraLevelComponentConfig>;
        AZ_EDITOR_COMPONENT(EditorUmbraLevelComponent, EditorUmbraLevelComponentTypeId, BaseClass);

        static void Reflect(AZ::ReflectContext* context);

        EditorUmbraLevelComponent() = default;
        EditorUmbraLevelComponent(const UmbraLevelComponentConfig& config);

    private:
        // BaseClass overrides ...
        void Activate() override;
        void Deactivate() override;

        AZ::u32 GenerateUmbraScene();

        static constexpr const char* GenerateUmbraSceneButtonText = "Generate Umbra Scene...";
        static constexpr const char* GenerateUmbraSceneToolTipText = "Generate and save an umbra scene from level entities.";
    };
} // namespace Umbra
