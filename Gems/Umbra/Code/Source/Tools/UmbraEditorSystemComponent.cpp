/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include "UmbraEditorSystemComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <Umbra/UmbraTypeIds.h>

namespace Umbra
{
    AZ_COMPONENT_IMPL(UmbraEditorSystemComponent, "UmbraEditorSystemComponent", UmbraEditorSystemComponentTypeId, BaseSystemComponent);

    void UmbraEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraEditorSystemComponent, UmbraSystemComponent>()
                ->Version(0)
                ;
        }
    }

    UmbraEditorSystemComponent::UmbraEditorSystemComponent() = default;

    UmbraEditorSystemComponent::~UmbraEditorSystemComponent() = default;

    void UmbraEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("UmbraSystemEditorService"));
    }

    void UmbraEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("UmbraSystemEditorService"));
    }

    void UmbraEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void UmbraEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void UmbraEditorSystemComponent::Activate()
    {
        UmbraSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void UmbraEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        UmbraSystemComponent::Deactivate();
    }

} // namespace Umbra
