/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * its licensors.
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Asset/AssetManager.h>
#include <AzCore/Component/Component.h>

#include <AssetBuilderSDK/AssetBuilderBusses.h>

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

    class UmbraAssetBuilderComponent final : public AZ::Component
    {
    public:
        AZ_COMPONENT(UmbraAssetBuilderComponent, "{F1AF5754-51B2-46C3-A34B-70F02A63ACE5}");
        static void Reflect(AZ::ReflectContext* context);

        UmbraAssetBuilderComponent();
        ~UmbraAssetBuilderComponent() override;

        // AZ::Component overrides...
        void Activate() override;
        void Deactivate() override;

    private:
        UmbraAssetBuilderComponent(const UmbraAssetBuilderComponent&) = delete;

        using AssetWorker = AssetBuilderSDK::AssetBuilderCommandBus::Handler;
        AZStd::vector<AZStd::unique_ptr<AssetWorker>> m_assetWorkers;
    };
} // namespace Umbra
