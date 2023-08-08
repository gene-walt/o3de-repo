/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/Component.h>
#include <Umbra/UmbraSystemComponent/UmbraSystemComponentBus.h>
#include <Umbra/UmbraSystemComponent/UmbraSystemComponentConfig.h>
#include <Umbra/UmbraSceneAsset/UmbraSceneAsset.h>

namespace Umbra
{
    //! This registers asset handlers and other data types needed in runtime and editor.
    class UmbraSystemComponentController final : UmbraSystemComponentRequestBus::Handler
    {
    public:
        friend class EditorUmbraSystemComponent;

        AZ_CLASS_ALLOCATOR(UmbraSystemComponentController, AZ::SystemAllocator);
        AZ_RTTI(UmbraSystemComponentController, "{E1DC4DD8-73E3-44A9-8F29-FC6731E108F4}");

        static void Reflect(AZ::ReflectContext* context);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& services);

        UmbraSystemComponentController() = default;
        UmbraSystemComponentController(const UmbraSystemComponentConfig& config);

        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const UmbraSystemComponentConfig& config);
        const UmbraSystemComponentConfig& GetConfiguration() const;

        //! UmbraSystemComponentRequestBus overrides...

    private:
        AZ_DISABLE_COPY(UmbraSystemComponentController);

        AZ::EntityId m_entityId;
        UmbraSystemComponentConfig m_configuration;
        AZStd::unique_ptr<UmbraSceneAssetHandler> m_sceneAssetHandler;
    };
} // namespace Umbra
