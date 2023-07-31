/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzFramework/Components/ComponentAdapter.h>
#include <Umbra/UmbraSystemComponent/UmbraSystemComponentConstants.h>
#include <UmbraSystemComponent/UmbraSystemComponentController.h>

namespace Umbra
{
    //!
    class UmbraSystemComponent final
        : public AzFramework::Components::ComponentAdapter<UmbraSystemComponentController, UmbraSystemComponentConfig>
    {
    public:
        using BaseClass = AzFramework::Components::ComponentAdapter<UmbraSystemComponentController, UmbraSystemComponentConfig>;
        AZ_COMPONENT(UmbraSystemComponent, UmbraSystemComponentTypeId, BaseClass);

        UmbraSystemComponent() = default;
        UmbraSystemComponent(const UmbraSystemComponentConfig& config);

        static void Reflect(AZ::ReflectContext* context);
    };
} // namespace Umbra
