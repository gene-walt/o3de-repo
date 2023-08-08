/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <UmbraSceneComponent/UmbraSceneComponent.h>

namespace Umbra
{
    UmbraSceneComponent::UmbraSceneComponent(const UmbraSceneComponentConfig& config)
        : BaseClass(config)
    {
    }

    void UmbraSceneComponent::Reflect(AZ::ReflectContext* context)
    {
        BaseClass::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraSceneComponent, BaseClass>()
                ->Version(0)
                ;
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->ConstantProperty("UmbraSceneComponentTypeId", BehaviorConstant(AZ::Uuid(UmbraSceneComponentTypeId)))
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ;
        }
    }
} // namespace Umbra
