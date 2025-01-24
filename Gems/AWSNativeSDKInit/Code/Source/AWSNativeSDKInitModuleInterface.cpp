
#include "AWSNativeSDKInitModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <Clients/AWSNativeSDKInitSystemComponent.h>

namespace AWSNativeSDKInit
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(AWSNativeSDKInitModuleInterface,
        "AWSNativeSDKInitModuleInterface", "{15C10BB5-4266-4130-B2D4-14C304ABBF4D}");
    AZ_RTTI_NO_TYPE_INFO_IMPL(AWSNativeSDKInitModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(AWSNativeSDKInitModuleInterface, AZ::SystemAllocator);

    AWSNativeSDKInitModuleInterface::AWSNativeSDKInitModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            AWSNativeSDKInitSystemComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList AWSNativeSDKInitModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<AWSNativeSDKInitSystemComponent>(),
        };
    }
} // namespace AWSNativeSDKInit
