/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <Umbra/UmbraViewVolumeComponent/UmbraViewVolumeComponentBus.h>
#include <Umbra/UmbraViewVolumeComponent/UmbraViewVolumeComponentConfig.h>

namespace Umbra
{
    //!
    class UmbraViewVolumeComponentController final : UmbraViewVolumeComponentRequestBus::Handler
    {
    public:
        friend class EditorUmbraViewVolumeComponent;

        AZ_CLASS_ALLOCATOR(UmbraViewVolumeComponentController, AZ::SystemAllocator);
        AZ_RTTI(UmbraViewVolumeComponentController, "{EF925CDB-C496-403B-9F2F-2D0FC6B7BCC2}");

        static void Reflect(AZ::ReflectContext* context);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        UmbraViewVolumeComponentController() = default;
        UmbraViewVolumeComponentController(const UmbraViewVolumeComponentConfig& config);

        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const UmbraViewVolumeComponentConfig& config);
        const UmbraViewVolumeComponentConfig& GetConfiguration() const;

        //! UmbraViewVolumeComponentRequestBus overrides...

    private:
        AZ_DISABLE_COPY(UmbraViewVolumeComponentController);

        AZ::EntityId m_entityId;
        UmbraViewVolumeComponentConfig m_configuration;
    };
} // namespace Umbra
