/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace Umbra
{
    struct UmbraSceneSettings;

    //! UmbraViewVolumeComponentRequestBus provides an interface for UmbraViewVolumeComponent
    class UmbraViewVolumeComponentRequests : public AZ::ComponentBus
    {
    public:
        //! Set a flag that enables or disables scene settings overrides
        //! @param overrideSceneSettings If true, enable scene settings overrides
        virtual void SetOverrideSceneSettings(bool overrideSceneSettings) = 0;

        //! @return True if scene settings overrides are enabled, otherwise false
        virtual bool GetOverrideSceneSettings() const = 0;

        //! Assign scene settings that will take effect within this view volume
        //! @param sceneSettings Scene settings that will override the global settings
        virtual void SetSceneSettings(const UmbraSceneSettings& sceneSettings) = 0;

        //! @return Scene settings that take effect within this view volume
        virtual const UmbraSceneSettings& GetSceneSettings() const = 0;
    };
    using UmbraViewVolumeComponentRequestBus = AZ::EBus<UmbraViewVolumeComponentRequests>;

    //! UmbraViewVolumeComponentNotificationBus notifications are sent when UmbraViewVolumeComponent changes.
    class UmbraViewVolumeComponentNotifications : public AZ::ComponentBus
    {
    public:
    };
    using UmbraViewVolumeComponentNotificationBus = AZ::EBus<UmbraViewVolumeComponentNotifications>;
} // namespace Umbra
