
#include <AWSNativeSDKInitModuleInterface.h>
#include "AWSNativeSDKInitSystemComponent.h"

namespace AWSNativeSDKInit
{
    class AWSNativeSDKInitModule
        : public AWSNativeSDKInitModuleInterface
    {
    public:
        AZ_RTTI(AWSNativeSDKInitModule, "{762E740F-0898-4BD4-97FC-11F414B7E8EC}", AWSNativeSDKInitModuleInterface);
        AZ_CLASS_ALLOCATOR(AWSNativeSDKInitModule, AZ::SystemAllocator);
    };
}// namespace AWSNativeSDKInit

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), AWSNativeSDKInit::AWSNativeSDKInitModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_AWSNativeSDKInit, AWSNativeSDKInit::AWSNativeSDKInitModule)
#endif
