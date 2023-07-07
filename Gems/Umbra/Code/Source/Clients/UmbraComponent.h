/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzFramework/Components/ComponentAdapter.h>
#include <Clients/UmbraComponentController.h>

namespace Umbra
{
    inline constexpr AZ::TypeId UmbraComponentTypeId{ "{4C9BC8E9-52DB-43EB-9055-574866BD87DC}" };

    class UmbraComponent final : public AzFramework::Components::ComponentAdapter<UmbraComponentController, UmbraComponentConfig>
    {
    public:
        using BaseClass = AzFramework::Components::ComponentAdapter<UmbraComponentController, UmbraComponentConfig>;
        AZ_COMPONENT(UmbraComponent, UmbraComponentTypeId, BaseClass);

        UmbraComponent() = default;
        UmbraComponent(const UmbraComponentConfig& config);

        static void Reflect(AZ::ReflectContext* context);
    };
} // namespace Umbra
