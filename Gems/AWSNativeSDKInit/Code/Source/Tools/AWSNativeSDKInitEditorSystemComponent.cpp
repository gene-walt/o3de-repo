
#include <AzCore/Serialization/SerializeContext.h>
#include "AWSNativeSDKInitEditorSystemComponent.h"


namespace AWSNativeSDKInit
{
    AZ_COMPONENT_IMPL(AWSNativeSDKInitEditorSystemComponent, "AWSNativeSDKInitEditorSystemComponent",
        "{07382678-59E4-46AA-86D1-8A3B792A4906}", BaseSystemComponent);

    void AWSNativeSDKInitEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AWSNativeSDKInitEditorSystemComponent, AWSNativeSDKInitSystemComponent>()
                ->Version(0);
        }
    }

    AWSNativeSDKInitEditorSystemComponent::AWSNativeSDKInitEditorSystemComponent() = default;

    AWSNativeSDKInitEditorSystemComponent::~AWSNativeSDKInitEditorSystemComponent() = default;

    void AWSNativeSDKInitEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("AWSNativeSDKInitEditorService"));
    }

    void AWSNativeSDKInitEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("AWSNativeSDKInitEditorService"));
    }

    void AWSNativeSDKInitEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void AWSNativeSDKInitEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void AWSNativeSDKInitEditorSystemComponent::Activate()
    {
        AWSNativeSDKInitSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void AWSNativeSDKInitEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        AWSNativeSDKInitSystemComponent::Deactivate();
    }

} // namespace AWSNativeSDKInit
