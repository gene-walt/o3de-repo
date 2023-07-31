/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Umbra/UmbraTomeAsset/UmbraTomeAsset.h>
#include <UmbraSystemComponent/UmbraSystemComponentController.h>

namespace Umbra
{
    void UmbraSystemComponentController::Reflect(AZ::ReflectContext* context)
    {
        UmbraTomeAsset::Reflect(context);
        UmbraSystemComponentConfig::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UmbraSystemComponentController>()
                ->Version(0)
                ->Field("Configuration", &UmbraSystemComponentController::m_configuration)
                ;
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<UmbraSystemComponentRequestBus>("UmbraSystemComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "umbra")
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ;
        }
    }

    void UmbraSystemComponentController::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("UmbraSystemService"));
    }

    void UmbraSystemComponentController::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("UmbraSystemService"));
    }

    void UmbraSystemComponentController::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    UmbraSystemComponentController::UmbraSystemComponentController(const UmbraSystemComponentConfig& config)
        : m_configuration(config)
    {
    }

    void UmbraSystemComponentController::Activate(AZ::EntityId entityId)
    {
        m_entityId = entityId;
        m_tomeAssetHandler.reset(aznew UmbraTomeAssetHandler(UmbraTomeAsset::DisplayName, UmbraTomeAsset::Group, UmbraTomeAsset::Extension));
        m_tomeAssetHandler->Register();

        UmbraSystemComponentRequestBus::Handler::BusConnect(m_entityId);
        QueueTome();
    }

    void UmbraSystemComponentController::Deactivate()
    {
        AZ::Data::AssetBus::Handler::BusDisconnect();
        UmbraSystemComponentRequestBus::Handler::BusDisconnect();

        ReleaseTome();

        m_tomeAsset.Release();
        m_tomeAssetHandler->Unregister();
        m_tomeAssetHandler.reset();
        m_entityId = AZ::EntityId(AZ::EntityId::InvalidEntityId);
    }

    void UmbraSystemComponentController::SetConfiguration(const UmbraSystemComponentConfig& config)
    {
        m_configuration = config;
    }

    const UmbraSystemComponentConfig& UmbraSystemComponentController::GetConfiguration() const
    {
        return m_configuration;
    }

    void UmbraSystemComponentController::OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        m_tomeAsset = asset;
        CreateTome();
    }

    void UmbraSystemComponentController::OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        m_tomeAsset = asset;
        CreateTome();
    }

    void UmbraSystemComponentController::OnAssetError(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
    }

    void UmbraSystemComponentController::OnAssetReloadError(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
    }

    void UmbraSystemComponentController::QueueTome()
    {
        ReleaseTome();

        AZ::Data::AssetBus::Handler::BusDisconnect();

        if (m_tomeAsset.GetId().IsValid())
        {
            m_tomeAsset.QueueLoad();
            AZ::Data::AssetBus::Handler::BusConnect(m_tomeAsset.GetId());
        }
    }

    void UmbraSystemComponentController::CreateTome()
    {
        ReleaseTome();

        if (m_tomeAsset.IsReady())
        {
            const auto& tomeBuffer = m_tomeAsset->GetTomeBuffer();
            m_tome = Umbra::TomeLoader::loadFromBuffer(tomeBuffer.data(), tomeBuffer.size());
        }
    }

    void UmbraSystemComponentController::ReleaseTome()
    {
        if (m_tome)
        {
            Umbra::TomeLoader::freeTome(m_tome);
            m_tome = nullptr;
        }
    }
} // namespace Umbra