/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzFramework/Asset/GenericAssetHandler.h>

namespace Umbra
{
    //! UmbraTomeAsset contains a byte buffer representing serialized visibility data
    class UmbraTomeAsset : public AZ::Data::AssetData
    {
    public:
        friend class UmbraTomeAssetBuilder;

        AZ_RTTI(UmbraTomeAsset, "{95F92BDF-5D27-4DDA-88E8-9549E7FB2E1D}", AZ::Data::AssetData);
        AZ_CLASS_ALLOCATOR(UmbraTomeAsset, AZ::SystemAllocator);
        static void Reflect(AZ::ReflectContext* context);

        static const char* DisplayName;
        static const char* Group;
        static const char* Extension;

        const AZStd::vector<uint8_t>& GetTomeBuffer() const;

    private:
        AZStd::vector<uint8_t> m_tomeBuffer;
    };

    using UmbraTomeAssetHandler = AzFramework::GenericAssetHandler<UmbraTomeAsset>;

} // namespace Umbra
