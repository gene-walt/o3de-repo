/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace Umbra
{
    // ComponentRequestBus provides an interface for UmbraObjectComponent
    class UmbraObjectComponentRequests : public AZ::ComponentBus
    {
    public:
        //! Set a flag that determines if this object can affect the visibility of other objects
        //! @param canOcclude Flag specifying if this object affects the visibility of other objects
        virtual void SetCanOcclude(bool canOcclude) = 0;

        //! @return True if the object will affect visibility of other objects. Otherwise, false.
        virtual bool GetCanOcclude() const = 0;

        //! Set a flag that determines if this object can be hidden by other objects
        //! @param canBeOccluded Flag specifying if this object can be hidden by other objects
        virtual void SetCanBeOccluded(bool canBeOccluded) = 0;

        //! @return True if the object can be hidden by other objects. Otherwise, false.
        virtual bool GetCanBeOccluded() const = 0;
    };
    using UmbraObjectComponentRequestBus = AZ::EBus<UmbraObjectComponentRequests>;

    //! UmbraObjectComponentNotificationBus notifications are sent when UmbraObjectComponent changes.
    class UmbraObjectComponentNotifications : public AZ::ComponentBus
    {
    public:
    };
    using UmbraObjectComponentNotificationBus = AZ::EBus<UmbraObjectComponentNotifications>;
} // namespace Umbra
