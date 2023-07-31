/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzFramework/Components/ComponentAdapter.h>
#include <Umbra/UmbraViewVolumeComponent/UmbraViewVolumeComponentConstants.h>
#include <UmbraViewVolumeComponent/UmbraViewVolumeComponentController.h>

namespace Umbra
{
    //!
    class UmbraViewVolumeComponent final
        : public AzFramework::Components::ComponentAdapter<UmbraViewVolumeComponentController, UmbraViewVolumeComponentConfig>
    {
    public:
        using BaseClass = AzFramework::Components::ComponentAdapter<UmbraViewVolumeComponentController, UmbraViewVolumeComponentConfig>;
        AZ_COMPONENT(UmbraViewVolumeComponent, UmbraViewVolumeComponentTypeId, BaseClass);

        UmbraViewVolumeComponent() = default;
        UmbraViewVolumeComponent(const UmbraViewVolumeComponentConfig& config);

        static void Reflect(AZ::ReflectContext* context);
    };
} // namespace Umbra
