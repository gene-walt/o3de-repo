/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/std/containers/vector.h>
#include <AzFramework/Asset/GenericAssetHandler.h>
#include <Umbra/UmbraSceneAsset/UmbraObjectDescriptor.h>

namespace Umbra
{
    //! UmbraSceneAsset contains a byte buffer for serialized tome data as well as object descriptors for each part of an entity added to
    //! the scene. The scene asset must be loaded by the scene component in order to create a tome that can be used for runtime occlusion
    //! checks against precomputed visibility data for static scene objects.
    class UmbraSceneAsset final : public AZ::Data::AssetData
    {
    public:
        friend class UmbraSceneAssetBuilder;

        AZ_RTTI(UmbraSceneAsset, "{95F92BDF-5D27-4DDA-88E8-9549E7FB2E1D}", AZ::Data::AssetData);
        AZ_CLASS_ALLOCATOR(UmbraSceneAsset, AZ::SystemAllocator);
        static void Reflect(AZ::ReflectContext* context);

        static const char* DisplayName;
        static const char* Group;
        static const char* Extension;

        const AZStd::vector<uint8_t>& GetTomeBuffer() const;
        const AZStd::vector<UmbraObjectDescriptor>& GetObjectDescriptors() const;

    private:
        AZStd::vector<uint8_t> m_tomeBuffer;
        AZStd::vector<UmbraObjectDescriptor> m_objectDescriptors;
    };

    using UmbraSceneAssetHandler = AzFramework::GenericAssetHandler<UmbraSceneAsset>;

} // namespace Umbra
