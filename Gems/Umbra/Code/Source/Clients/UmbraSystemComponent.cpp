/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Clients/UmbraSystemComponent.h>
#include <Umbra/UmbraTypeIds.h>

namespace Umbra
{
    AZ_COMPONENT_IMPL(UmbraSystemComponent, "UmbraSystemComponent", UmbraSystemComponentTypeId);

    void UmbraSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        UmbraAsset::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void UmbraSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("UmbraSystemService"));
    }

    void UmbraSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("UmbraSystemService"));
    }

    void UmbraSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("RPISystem"));
    }

    void UmbraSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    UmbraSystemComponent::UmbraSystemComponent()
    {
        if (UmbraInterface::Get() == nullptr)
        {
            UmbraInterface::Register(this);
        }
    }

    UmbraSystemComponent::~UmbraSystemComponent()
    {
        if (UmbraInterface::Get() == this)
        {
            UmbraInterface::Unregister(this);
        }
    }

    void UmbraSystemComponent::Init()
    {
    }

    void UmbraSystemComponent::Activate()
    {
        m_assetHandler.reset(aznew UmbraAssetHandler(UmbraAsset::DisplayName, UmbraAsset::Group, UmbraAsset::Extension));
        m_assetHandler->Register();
        UmbraRequestBus::Handler::BusConnect();
    }

    void UmbraSystemComponent::Deactivate()
    {
        UmbraRequestBus::Handler::BusDisconnect();
        m_assetHandler->Unregister();
        m_assetHandler.reset();
    }
} // namespace Umbra
