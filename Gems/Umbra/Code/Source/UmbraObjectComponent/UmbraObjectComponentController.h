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
    //! Implements common functionality for umbra object run time and editor components.
    class UmbraObjectComponentController final : public UmbraObjectComponentRequestBus::Handler
    {
    public:
        friend class EditorUmbraObjectComponent;

        AZ_CLASS_ALLOCATOR(UmbraObjectComponentController, AZ::SystemAllocator);
        AZ_RTTI(UmbraObjectComponentController, "{71E651D8-48F3-4605-8A68-F21E71DF49FD}");

        static void Reflect(AZ::ReflectContext* context);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& services);

        UmbraObjectComponentController() = default;
        UmbraObjectComponentController(const UmbraObjectComponentConfig& config);

        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const UmbraObjectComponentConfig& config);
        const UmbraObjectComponentConfig& GetConfiguration() const;

        //! UmbraObjectComponentRequestBus overrides...
        void SetCanOcclude(bool canOcclude) override;
        bool GetCanOcclude() const override;
        void SetCanBeOccluded(bool canBeOccluded) override;
        bool GetCanBeOccluded() const override;

    private:
        AZ_DISABLE_COPY_MOVE(UmbraObjectComponentController);

        AZ::EntityId m_entityId;
        UmbraObjectComponentConfig m_configuration;
    };
} // namespace Umbra
