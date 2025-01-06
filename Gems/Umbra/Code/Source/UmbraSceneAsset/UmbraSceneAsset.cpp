/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Umbra/UmbraSceneAsset/UmbraSceneAsset.h>

namespace Umbra
{
    const char* UmbraSceneAsset::DisplayName = "Umbra Scene Asset";
    const char* UmbraSceneAsset::Group = "Umbra";
    const char* UmbraSceneAsset::Extension = "umbrasceneasset";

    void UmbraSceneAsset::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraSceneAsset, AZ::Data::AssetData>()
                ->Version(0)
                ->Field("tomeBuffer", &UmbraSceneAsset::m_tomeBuffer)
                ->Field("objectDescriptors", &UmbraSceneAsset::m_objectDescriptors)
                ;
        }
    }

    const AZStd::vector<uint8_t>& UmbraSceneAsset::GetTomeBuffer() const
    {
        return m_tomeBuffer;
    }

    const AZStd::vector<UmbraObjectDescriptor>& UmbraSceneAsset::GetObjectDescriptors() const
    {
        return m_objectDescriptors;
    }

} // namespace Umbra
