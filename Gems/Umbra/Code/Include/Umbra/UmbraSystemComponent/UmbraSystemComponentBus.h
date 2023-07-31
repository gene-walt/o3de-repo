/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace Umbra
{
    //! UmbraSystemComponentRequestBus provides an interface to request operations on a UmbraSystemComponent
    class UmbraSystemComponentRequests : public AZ::ComponentBus
    {
    public:
    };
    using UmbraSystemComponentRequestBus = AZ::EBus<UmbraSystemComponentRequests>;

    //! UmbraSystemComponentNotificationBus notifications are sent whenever the state of the UmbraSystemComponent changes.
    class UmbraSystemComponentNotifications : public AZ::ComponentBus
    {
    public:
    };
    using UmbraSystemComponentNotificationBus = AZ::EBus<UmbraSystemComponentNotifications>;
} // namespace Umbra
