/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * its licensors.
 */

#include <Tools/EditorUmbraComponent.h>
#include <Tools/UmbraAssetBuilderComponent.h>
#include <Tools/UmbraEditorSystemComponent.h>
#include <Umbra/UmbraTypeIds.h>
#include <UmbraModuleInterface.h>

namespace Umbra
{
    class UmbraEditorModule : public UmbraModuleInterface
    {
    public:
        AZ_RTTI(UmbraEditorModule, UmbraEditorModuleTypeId, UmbraModuleInterface);
        AZ_CLASS_ALLOCATOR(UmbraEditorModule, AZ::SystemAllocator);

        UmbraEditorModule()
        {
            // Register descriptors for any components added by this module
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    UmbraAssetBuilderComponent::CreateDescriptor(),
                    UmbraEditorSystemComponent::CreateDescriptor(),
                    EditorUmbraComponent::CreateDescriptor(),
                });
        }

        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            // Return a list of any system components that need to be automatically created by this module
            return AZ::ComponentTypeList{
                azrtti_typeid<UmbraEditorSystemComponent>(),
            };
        }
    };
} // namespace Umbra

AZ_DECLARE_MODULE_CLASS(Gem_Umbra, Umbra::UmbraEditorModule)
