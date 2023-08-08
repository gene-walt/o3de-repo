/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzFramework/Components/ComponentAdapter.h>
#include <Umbra/UmbraObjectComponent/UmbraObjectComponentConstants.h>
#include <UmbraObjectComponent/UmbraObjectComponentController.h>

namespace Umbra
{
    //! UmbraObjectComponent can be added to entities with compatible renderable components to configure umbra occlusion settings.
    class UmbraObjectComponent final
        : public AzFramework::Components::ComponentAdapter<UmbraObjectComponentController, UmbraObjectComponentConfig>
    {
    public:
        using BaseClass = AzFramework::Components::ComponentAdapter<UmbraObjectComponentController, UmbraObjectComponentConfig>;
        AZ_COMPONENT(UmbraObjectComponent, UmbraObjectComponentTypeId, BaseClass);

        UmbraObjectComponent() = default;
        UmbraObjectComponent(const UmbraObjectComponentConfig& config);

        static void Reflect(AZ::ReflectContext* context);
    };
} // namespace Umbra
