/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Clients/UmbraAsset.h>

namespace Umbra
{
    const char* UmbraAsset::DisplayName = "UmbraAsset";
    const char* UmbraAsset::Group = "Umbra";
    const char* UmbraAsset::Extension = "umbra";

    void UmbraAsset::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraAsset, AZ::Data::AssetData>()
                ->Version(0)
                ->Field("tomeBuffer", &UmbraAsset::m_tomeBuffer)
                ;
        }
    }
    const AZStd::vector<uint8_t>& UmbraAsset::GetTomeBuffer() const
    {
        return m_tomeBuffer;
    }
} // namespace Umbra
