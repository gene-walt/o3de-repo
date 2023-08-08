/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzToolsFramework/ToolsComponents/EditorComponentAdapter.h>
#include <Umbra/UmbraObjectComponent/UmbraObjectComponentBus.h>
#include <Umbra/UmbraObjectComponent/UmbraObjectComponentConstants.h>
#include <UmbraObjectComponent/UmbraObjectComponent.h>

namespace Umbra
{
    //! EditorUmbraObjectComponent extends the corresponding component and controller with edit contexts and other editor specific features.
    class EditorUmbraObjectComponent final
        : public AzToolsFramework::Components::EditorComponentAdapter<UmbraObjectComponentController, UmbraObjectComponent, UmbraObjectComponentConfig>
        , public UmbraObjectComponentNotificationBus::Handler
    {
    public:
        using BaseClass = AzToolsFramework::Components::EditorComponentAdapter<UmbraObjectComponentController, UmbraObjectComponent, UmbraObjectComponentConfig>;
        AZ_EDITOR_COMPONENT(EditorUmbraObjectComponent, EditorUmbraObjectComponentTypeId, BaseClass);

        static void Reflect(AZ::ReflectContext* context);

        EditorUmbraObjectComponent() = default;
        EditorUmbraObjectComponent(const UmbraObjectComponentConfig& config);

    private:
        // BaseClass overrides ...
        void Activate() override;
        void Deactivate() override;
    };
} // namespace Umbra
