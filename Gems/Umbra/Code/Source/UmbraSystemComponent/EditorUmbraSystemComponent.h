/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzToolsFramework/ToolsComponents/EditorComponentAdapter.h>
#include <Umbra/UmbraSystemComponent/UmbraSystemComponentConstants.h>
#include <UmbraSystemComponent/UmbraSystemComponent.h>

namespace Umbra
{
    //! Managers editor only reflection of system level data types for the gem.
    class EditorUmbraSystemComponent final
        : public AzToolsFramework::Components::EditorComponentAdapter<UmbraSystemComponentController, UmbraSystemComponent, UmbraSystemComponentConfig>
    {
    public:
        using BaseClass = AzToolsFramework::Components::EditorComponentAdapter<UmbraSystemComponentController, UmbraSystemComponent, UmbraSystemComponentConfig>;
        AZ_EDITOR_COMPONENT(EditorUmbraSystemComponent, EditorUmbraSystemComponentTypeId, BaseClass);

        static void Reflect(AZ::ReflectContext* context);

        EditorUmbraSystemComponent() = default;
        EditorUmbraSystemComponent(const UmbraSystemComponentConfig& config);

    private:
        // BaseClass overrides ...
        void Activate() override;
        void Deactivate() override;
    };
} // namespace Umbra
