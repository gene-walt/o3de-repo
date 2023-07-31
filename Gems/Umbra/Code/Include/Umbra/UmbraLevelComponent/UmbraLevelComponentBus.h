/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace Umbra
{
    //! UmbraLevelComponentRequestBus provides an interface to request operations on a UmbraLevelComponent
    class UmbraLevelComponentRequests : public AZ::ComponentBus
    {
    public:
    };
    using UmbraLevelComponentRequestBus = AZ::EBus<UmbraLevelComponentRequests>;

    //! UmbraLevelComponentNotificationBus notifications are sent whenever the state of the UmbraLevelComponent changes.
    class UmbraLevelComponentNotifications : public AZ::ComponentBus
    {
    public:
    };
    using UmbraLevelComponentNotificationBus = AZ::EBus<UmbraLevelComponentNotifications>;
} // namespace Umbra
