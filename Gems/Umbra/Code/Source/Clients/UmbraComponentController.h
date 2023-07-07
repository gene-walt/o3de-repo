/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/Component.h>

namespace Umbra
{
    class UmbraComponentConfig final : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(UmbraComponentConfig, "{4F86B076-9D17-4727-A90A-FDB172996D7C}", ComponentConfig);
        AZ_CLASS_ALLOCATOR(UmbraComponentConfig, AZ::SystemAllocator);
        static void Reflect(AZ::ReflectContext* context);

        UmbraComponentConfig() = default;
    };

    class UmbraComponentController final
    {
    public:
        friend class EditorUmbraComponent;

        AZ_RTTI(UmbraComponentController, "{2C235AC6-DE01-43D5-BEFC-96EB3D78056A}");
        AZ_CLASS_ALLOCATOR(UmbraComponentController, AZ::SystemAllocator);

        static void Reflect(AZ::ReflectContext* context);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        UmbraComponentController() = default;
        UmbraComponentController(const UmbraComponentConfig& config);

        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const UmbraComponentConfig& config);
        const UmbraComponentConfig& GetConfiguration() const;

    private:
        AZ_DISABLE_COPY(UmbraComponentController);

        AZ::EntityId m_entityId;

        UmbraComponentConfig m_configuration;
    };
} // namespace Umbra
