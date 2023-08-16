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
    //! Manages any common functionality between runtime and editor components for umbra view volumes. This may be responsible for rendering
    //! debug data.
    class UmbraViewVolumeComponentController final : UmbraViewVolumeComponentRequestBus::Handler
    {
    public:
        friend class EditorUmbraViewVolumeComponent;

        AZ_CLASS_ALLOCATOR(UmbraViewVolumeComponentController, AZ::SystemAllocator);
        AZ_RTTI(UmbraViewVolumeComponentController, "{EF925CDB-C496-403B-9F2F-2D0FC6B7BCC2}");

        static void Reflect(AZ::ReflectContext* context);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& services);

        UmbraViewVolumeComponentController() = default;
        UmbraViewVolumeComponentController(const UmbraViewVolumeComponentConfig& config);

        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const UmbraViewVolumeComponentConfig& config);
        const UmbraViewVolumeComponentConfig& GetConfiguration() const;

        //! UmbraViewVolumeComponentRequestBus overrides...
        void SetOverrideSceneSettings(bool overrideSceneSettings);
        bool GetOverrideSceneSettings() const;
        void SetSceneSettings(const UmbraSceneSettings& sceneSettings);
        const UmbraSceneSettings& GetSceneSettings() const;

    private:
        AZ_DISABLE_COPY(UmbraViewVolumeComponentController);

        AZ::EntityId m_entityId;
        UmbraViewVolumeComponentConfig m_configuration;
    };
} // namespace Umbra
