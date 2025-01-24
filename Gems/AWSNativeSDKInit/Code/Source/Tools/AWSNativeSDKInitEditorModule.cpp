
#include <AWSNativeSDKInitModuleInterface.h>
#include "AWSNativeSDKInitEditorSystemComponent.h"

namespace AWSNativeSDKInit
{
    class AWSNativeSDKInitEditorModule
        : public AWSNativeSDKInitModuleInterface
    {
    public:
        AZ_RTTI(AWSNativeSDKInitEditorModule, "{1ECC58B4-AFC8-45BE-996C-3ECB1F4136EA}", AWSNativeSDKInitModuleInterface);
        AZ_CLASS_ALLOCATOR(AWSNativeSDKInitEditorModule, AZ::SystemAllocator);

        AWSNativeSDKInitEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                AWSNativeSDKInitEditorSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<AWSNativeSDKInitEditorSystemComponent>(),
            };
        }
    };
}// namespace AWSNativeSDKInit

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), AWSNativeSDKInit::AWSNativeSDKInitEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_AWSNativeSDKInit_Editor, AWSNativeSDKInit::AWSNativeSDKInitEditorModule)
#endif
