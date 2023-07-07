/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <Clients/UmbraSystemComponent.h>

namespace Umbra
{
    /// System component for Umbra within the editor
    class UmbraEditorSystemComponent
        : public UmbraSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = UmbraSystemComponent;

    public:
        AZ_COMPONENT_DECL(UmbraEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        UmbraEditorSystemComponent();
        ~UmbraEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace Umbra
