/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzToolsFramework/ToolsComponents/EditorComponentAdapter.h>
#include <Umbra/UmbraViewVolumeComponent/UmbraViewVolumeComponentConstants.h>
#include <UmbraViewVolumeComponent/UmbraViewVolumeComponent.h>

namespace Umbra
{
    //! Reflects edit contacts and other editor specific data for corresponding component and controller.
    class EditorUmbraViewVolumeComponent final
        : public AzToolsFramework::Components::EditorComponentAdapter<UmbraViewVolumeComponentController, UmbraViewVolumeComponent, UmbraViewVolumeComponentConfig>
    {
    public:
        using BaseClass = AzToolsFramework::Components::EditorComponentAdapter<UmbraViewVolumeComponentController, UmbraViewVolumeComponent, UmbraViewVolumeComponentConfig>;
        AZ_EDITOR_COMPONENT(EditorUmbraViewVolumeComponent, EditorUmbraViewVolumeComponentTypeId, BaseClass);

        static void Reflect(AZ::ReflectContext* context);

        EditorUmbraViewVolumeComponent() = default;
        EditorUmbraViewVolumeComponent(const UmbraViewVolumeComponentConfig& config);

    private:
        // BaseClass overrides ...
        void Activate() override;
        void Deactivate() override;
    };
} // namespace Umbra
