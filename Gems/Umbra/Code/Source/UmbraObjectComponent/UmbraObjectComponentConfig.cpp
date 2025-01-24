/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Umbra/UmbraObjectComponent/UmbraObjectComponentConfig.h>

namespace Umbra
{
    void UmbraObjectComponentConfig::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraObjectComponentConfig, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("canOcclude", &UmbraObjectComponentConfig::m_canOcclude)
                ->Field("canBeOccluded", &UmbraObjectComponentConfig::m_canBeOccluded)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<UmbraObjectComponentConfig>("UmbraObjectComponentConfig", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Show)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraObjectComponentConfig::m_canOcclude, "Can Occlude", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraObjectComponentConfig::m_canBeOccluded, "Can Be Occluded", "")
                    ;
                ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<UmbraObjectComponentConfig>("UmbraObjectComponentConfig")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "Umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Constructor()
                ->Constructor<const UmbraObjectComponentConfig&>()
                ->Property("canOcclude", BehaviorValueProperty(&UmbraObjectComponentConfig::m_canOcclude))
                ->Property("canBeOccluded", BehaviorValueProperty(&UmbraObjectComponentConfig::m_canBeOccluded))
                ;
        }
    }
} // namespace Umbra
