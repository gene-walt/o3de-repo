/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Umbra/UmbraSceneAsset/UmbraObjectDescriptor.h>

namespace Umbra
{
    void UmbraObjectDescriptor::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraObjectDescriptor>()
                ->Version(0)
                ->Field("entityId", &UmbraObjectDescriptor::m_entityId)
                ->Field("partId", &UmbraObjectDescriptor::m_partId)
                ;
        }
    }
} // namespace Umbra
