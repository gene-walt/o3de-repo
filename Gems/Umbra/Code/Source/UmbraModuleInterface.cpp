/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/Memory/Memory.h>
#include <Umbra/UmbraTypeIds.h>
#include <UmbraModuleInterface.h>

#include <UmbraLevelComponent/UmbraLevelComponent.h>
#include <UmbraObjectComponent/UmbraObjectComponent.h>
#include <UmbraSystemComponent/UmbraSystemComponent.h>
#include <UmbraViewVolumeComponent/UmbraViewVolumeComponent.h>

namespace Umbra
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(UmbraModuleInterface, "UmbraModuleInterface", UmbraModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(UmbraModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(UmbraModuleInterface, AZ::SystemAllocator);

    UmbraModuleInterface::UmbraModuleInterface()
    {
        // Register descriptors for any components added by this module
        m_descriptors.insert(
            m_descriptors.end(),
            {
                UmbraLevelComponent::CreateDescriptor(),
                UmbraObjectComponent::CreateDescriptor(),
                UmbraSystemComponent::CreateDescriptor(),
                UmbraViewVolumeComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList UmbraModuleInterface::GetRequiredSystemComponents() const
    {
        // Return a list of any system components that need to be automatically created by this module
        return AZ::ComponentTypeList{
            azrtti_typeid<UmbraSystemComponent>(),
        };
    }
} // namespace Umbra
