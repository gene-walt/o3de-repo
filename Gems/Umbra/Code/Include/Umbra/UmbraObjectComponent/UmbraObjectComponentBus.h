/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace Umbra
{
    //! UmbraObjectComponentRequestBus provides an interface to request operations on a UmbraObjectComponent
    class UmbraObjectComponentRequests : public AZ::ComponentBus
    {
    public:
    };
    using UmbraObjectComponentRequestBus = AZ::EBus<UmbraObjectComponentRequests>;

    //! UmbraObjectComponentNotificationBus notifications are sent whenever the state of the UmbraObjectComponent changes.
    class UmbraObjectComponentNotifications : public AZ::ComponentBus
    {
    public:
    };
    using UmbraObjectComponentNotificationBus = AZ::EBus<UmbraObjectComponentNotifications>;
} // namespace Umbra
