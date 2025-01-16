
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/AWSNativeSDKInitSystemComponent.h>

namespace AWSNativeSDKInit
{
    /// System component for AWSNativeSDKInit editor
    class AWSNativeSDKInitEditorSystemComponent
        : public AWSNativeSDKInitSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = AWSNativeSDKInitSystemComponent;
    public:
        AZ_COMPONENT_DECL(AWSNativeSDKInitEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        AWSNativeSDKInitEditorSystemComponent();
        ~AWSNativeSDKInitEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace AWSNativeSDKInit
