/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <UmbraLevelComponent/UmbraLevelComponent.h>

namespace Umbra
{
    UmbraLevelComponent::UmbraLevelComponent(const UmbraLevelComponentConfig& config)
        : BaseClass(config)
    {
    }

    void UmbraLevelComponent::Reflect(AZ::ReflectContext* context)
    {
        BaseClass::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraLevelComponent, BaseClass>()
                ->Version(0)
                ;
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->ConstantProperty("UmbraLevelComponentTypeId", BehaviorConstant(AZ::Uuid(UmbraLevelComponentTypeId)))
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ;
        }
    }
} // namespace Umbra
