/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <Umbra/UmbraBus.h>

namespace Umbra
{
    class UmbraSystemComponent
        : public AZ::Component
        , protected UmbraRequestBus::Handler
    {
    public:
        AZ_COMPONENT_DECL(UmbraSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        UmbraSystemComponent();
        ~UmbraSystemComponent();

    protected:
        // UmbraRequestBus overrides...

        // AZ::Component overrides...
        void Init() override;
        void Activate() override;
        void Deactivate() override;
    };

} // namespace Umbra
