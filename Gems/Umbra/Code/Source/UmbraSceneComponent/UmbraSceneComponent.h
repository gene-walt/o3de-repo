/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzFramework/Components/ComponentAdapter.h>
#include <Umbra/UmbraSceneComponent/UmbraSceneComponentConstants.h>
#include <UmbraSceneComponent/UmbraSceneComponentController.h>

namespace Umbra
{
    //! UmbraSceneComponent and the corresponding controller loads and manages umbra scene and tome data to process occlusion queries.
    class UmbraSceneComponent final
        : public AzFramework::Components::ComponentAdapter<UmbraSceneComponentController, UmbraSceneComponentConfig>
    {
    public:
        using BaseClass = AzFramework::Components::ComponentAdapter<UmbraSceneComponentController, UmbraSceneComponentConfig>;
        AZ_COMPONENT(UmbraSceneComponent, UmbraSceneComponentTypeId, BaseClass);

        UmbraSceneComponent() = default;
        UmbraSceneComponent(const UmbraSceneComponentConfig& config);

        static void Reflect(AZ::ReflectContext* context);
    };
} // namespace Umbra
