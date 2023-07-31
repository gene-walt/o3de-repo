/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <Umbra/UmbraObjectComponent/UmbraObjectComponentBus.h>
#include <Umbra/UmbraObjectComponent/UmbraObjectComponentConfig.h>

namespace Umbra
{
    //!
    class UmbraObjectComponentController final : UmbraObjectComponentRequestBus::Handler
    {
    public:
        friend class EditorUmbraObjectComponent;

        AZ_CLASS_ALLOCATOR(UmbraObjectComponentController, AZ::SystemAllocator);
        AZ_RTTI(UmbraObjectComponentController, "{71E651D8-48F3-4605-8A68-F21E71DF49FD}");

        static void Reflect(AZ::ReflectContext* context);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        UmbraObjectComponentController() = default;
        UmbraObjectComponentController(const UmbraObjectComponentConfig& config);

        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const UmbraObjectComponentConfig& config);
        const UmbraObjectComponentConfig& GetConfiguration() const;

        //! UmbraObjectComponentRequestBus overrides...

    private:
        AZ_DISABLE_COPY(UmbraObjectComponentController);

        AZ::EntityId m_entityId;
        UmbraObjectComponentConfig m_configuration;
    };
} // namespace Umbra
