/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace Umbra
{
    //! UmbraViewVolumeComponentRequestBus provides an interface to request operations on a UmbraViewVolumeComponent
    class UmbraViewVolumeComponentRequests : public AZ::ComponentBus
    {
    public:
    };
    using UmbraViewVolumeComponentRequestBus = AZ::EBus<UmbraViewVolumeComponentRequests>;

    //! UmbraViewVolumeComponentNotificationBus notifications are sent whenever the state of the UmbraViewVolumeComponent changes.
    class UmbraViewVolumeComponentNotifications : public AZ::ComponentBus
    {
    public:
    };
    using UmbraViewVolumeComponentNotificationBus = AZ::EBus<UmbraViewVolumeComponentNotifications>;
} // namespace Umbra
