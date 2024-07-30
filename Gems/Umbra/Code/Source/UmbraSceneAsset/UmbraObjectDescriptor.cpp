/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Serialization/EditContext.h>
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

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<UmbraObjectDescriptor>("UmbraObjectDescriptor", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Show)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraObjectDescriptor::m_entityId, "Entity Id", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraObjectDescriptor::m_partId, "Part Id", "")
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<UmbraObjectDescriptor>("UmbraObjectDescriptor")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "Umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Constructor()
                ->Constructor<const UmbraObjectDescriptor&>()
                ->Property("entityId", BehaviorValueProperty(&UmbraObjectDescriptor::m_entityId))
                ->Property("partId", BehaviorValueProperty(&UmbraObjectDescriptor::m_partId))
                ;
        }
    }
} // namespace Umbra
