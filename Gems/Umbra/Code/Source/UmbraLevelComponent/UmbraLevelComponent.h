/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzFramework/Components/ComponentAdapter.h>
#include <Umbra/UmbraLevelComponent/UmbraLevelComponentConstants.h>
#include <UmbraLevelComponent/UmbraLevelComponentController.h>

namespace Umbra
{
    //!
    class UmbraLevelComponent final
        : public AzFramework::Components::ComponentAdapter<UmbraLevelComponentController, UmbraLevelComponentConfig>
    {
    public:
        using BaseClass = AzFramework::Components::ComponentAdapter<UmbraLevelComponentController, UmbraLevelComponentConfig>;
        AZ_COMPONENT(UmbraLevelComponent, UmbraLevelComponentTypeId, BaseClass);

        UmbraLevelComponent() = default;
        UmbraLevelComponent(const UmbraLevelComponentConfig& config);

        static void Reflect(AZ::ReflectContext* context);
    };
} // namespace Umbra
