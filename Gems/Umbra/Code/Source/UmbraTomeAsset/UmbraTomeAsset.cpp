/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Umbra/UmbraTomeAsset/UmbraTomeAsset.h>

namespace Umbra
{
    const char* UmbraTomeAsset::DisplayName = "UmbraTomeAsset";
    const char* UmbraTomeAsset::Group = "Umbra";
    const char* UmbraTomeAsset::Extension = "umbratome";

    void UmbraTomeAsset::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraTomeAsset, AZ::Data::AssetData>()
                ->Version(0)
                ->Field("tomeBuffer", &UmbraTomeAsset::m_tomeBuffer)
                ;
        }
    }
    const AZStd::vector<uint8_t>& UmbraTomeAsset::GetTomeBuffer() const
    {
        return m_tomeBuffer;
    }
} // namespace Umbra
