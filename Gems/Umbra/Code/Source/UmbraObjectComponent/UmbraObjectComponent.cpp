/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <UmbraObjectComponent/UmbraObjectComponent.h>

namespace Umbra
{
    UmbraObjectComponent::UmbraObjectComponent(const UmbraObjectComponentConfig& config)
        : BaseClass(config)
    {
    }

    void UmbraObjectComponent::Reflect(AZ::ReflectContext* context)
    {
        BaseClass::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraObjectComponent, BaseClass>()
                ->Version(0)
                ;
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->ConstantProperty("UmbraObjectComponentTypeId", BehaviorConstant(AZ::Uuid(UmbraObjectComponentTypeId)))
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ;
        }
    }
} // namespace Umbra
