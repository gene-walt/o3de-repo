/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include "UmbraSystemComponent.h"
#include <AzCore/RTTI/RTTI.h>
#include <Umbra/UmbraTypeIds.h>
#include <UmbraModuleInterface.h>

namespace Umbra
{
    class UmbraModule : public UmbraModuleInterface
    {
    public:
        AZ_RTTI(UmbraModule, UmbraModuleTypeId, UmbraModuleInterface);
        AZ_CLASS_ALLOCATOR(UmbraModule, AZ::SystemAllocator);
    };
} // namespace Umbra

AZ_DECLARE_MODULE_CLASS(Gem_Umbra, Umbra::UmbraModule)
