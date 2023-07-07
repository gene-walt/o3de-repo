/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <Umbra/UmbraTypeIds.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace Umbra
{
    class UmbraRequests
    {
    public:
        AZ_RTTI(UmbraRequests, UmbraRequestsTypeId);
        virtual ~UmbraRequests() = default;
    };

    class UmbraBusTraits
        : public AZ::EBusTraits
    {
    public:
        // EBusTraits overrides ...
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
    };

    using UmbraRequestBus = AZ::EBus<UmbraRequests, UmbraBusTraits>;
    using UmbraInterface = AZ::Interface<UmbraRequests>;

} // namespace Umbra
