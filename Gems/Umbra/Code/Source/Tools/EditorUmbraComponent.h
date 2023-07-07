/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <Atom/Feature/Utils/EditorRenderComponentAdapter.h>
#include <Clients/UmbraComponent.h>
#include <Umbra/UmbraTypeIds.h>

namespace Umbra
{
    inline constexpr AZ::TypeId EditorComponentTypeId{ "{3B97D64C-7F39-4E6C-B2D3-E2D678109B73}" };

    class EditorUmbraComponent final
        : public AZ::Render::EditorRenderComponentAdapter<UmbraComponentController, UmbraComponent, UmbraComponentConfig>
    {
    public:
        using BaseClass = AZ::Render::EditorRenderComponentAdapter<UmbraComponentController, UmbraComponent, UmbraComponentConfig>;
        AZ_EDITOR_COMPONENT(EditorUmbraComponent, EditorComponentTypeId, BaseClass);

        static void Reflect(AZ::ReflectContext* context);

        EditorUmbraComponent();
        EditorUmbraComponent(const UmbraComponentConfig& config);

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;
    };
} // namespace Umbra
