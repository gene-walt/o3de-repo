/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * its licensors.
 */

#pragma once

#include <AssetBuilderSDK/AssetBuilderBusses.h>
#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Asset/AssetManager.h>
#include <AzCore/Component/Component.h>

namespace Umbra
{
    //! This is a helper function for creating an asset builder unique_ptr instance and registering it.
    template<class T>
    AZStd::unique_ptr<T> MakeAssetBuilder()
    {
        static_assert(
            (AZStd::is_base_of<AssetBuilderSDK::AssetBuilderCommandBus::Handler, T>::value),
            "Can only specify desired type if it's an AssetBuilderCommandBus::Handler");

        auto assetWorker = AZStd::make_unique<T>();
        assetWorker->RegisterBuilder();
        return AZStd::move(assetWorker);
    }

    //! Creates and registers an instance of the umbra scene asset builder
    class UmbraSceneAssetBuilderComponent final : public AZ::Component
    {
    public:
        AZ_COMPONENT(UmbraSceneAssetBuilderComponent, "{F1AF5754-51B2-46C3-A34B-70F02A63ACE5}");
        static void Reflect(AZ::ReflectContext* context);

        UmbraSceneAssetBuilderComponent();
        ~UmbraSceneAssetBuilderComponent() override;

        // AZ::Component overrides...
        void Activate() override;
        void Deactivate() override;

    private:
        UmbraSceneAssetBuilderComponent(const UmbraSceneAssetBuilderComponent&) = delete;

        using AssetWorker = AssetBuilderSDK::AssetBuilderCommandBus::Handler;
        AZStd::vector<AZStd::unique_ptr<AssetWorker>> m_assetWorkers;
    };
} // namespace Umbra
