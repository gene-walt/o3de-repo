/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Utils/Utils.h>
#include <AzFramework/Asset/AssetSystemBus.h>
#include <AzQtComponents/Components/Widgets/FileDialog.h>
#include <AzToolsFramework/API/EditorAssetSystemAPI.h>
#include <AzToolsFramework/UI/UICore/WidgetHelpers.h>
#include <Umbra/UmbraSceneAsset/UmbraSceneAsset.h>
#include <UmbraSceneAsset/UmbraSceneDescriptor.h>
#include <UmbraSceneComponent/EditorUmbraSceneComponent.h>
#include <UmbraSceneComponent/EditorUmbraSceneComponentUtil.h>

#include <QMessageBox>
#include <QObject>

namespace Umbra
{
    void EditorUmbraSceneComponent::Reflect(AZ::ReflectContext* context)
    {
        BaseClass::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<EditorUmbraSceneComponent, BaseClass>()
                ->Version(0)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<EditorUmbraSceneComponent>(
                    "Umbra Scene", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::Category, "Umbra")
                        ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                        ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Icons/Components/Viewport/Component_Placeholder.svg")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Level"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->Attribute(AZ::Edit::Attributes::HelpPageURL, "")
                        ->Attribute(AZ::Edit::Attributes::PrimaryAssetType, AZ::AzTypeInfo<Umbra::UmbraSceneAsset>::Uuid())
                        ->UIElement(AZ::Edit::UIHandlers::Button, GenerateUmbraSceneButtonText, GenerateUmbraSceneToolTipText)
                            ->Attribute(AZ::Edit::Attributes::NameLabelOverride, "")
                            ->Attribute(AZ::Edit::Attributes::ButtonText, GenerateUmbraSceneButtonText)
                            ->Attribute(AZ::Edit::Attributes::ChangeNotify, &EditorUmbraSceneComponent::GenerateUmbraScene)
                    ;

                editContext->Class<UmbraSceneComponentController>(
                    "Umbra Scene Component Controller", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentController::m_configuration, "Configuration", "")
                        ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ;

                editContext->Class<UmbraSceneComponentConfig>(
                    "Umbra Scene Component Config", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Hide)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_collisionRadius, "Collision Radius", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_smallestHole, "Smallest Hole", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_smallestOccluder, "Smallest Occluder", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &UmbraSceneComponentConfig::m_sceneAsset, "Scene Asset", "")
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->ConstantProperty("EditorUmbraSceneComponentTypeId", BehaviorConstant(AZ::Uuid(EditorUmbraSceneComponentTypeId)))
                ->Attribute(AZ::Script::Attributes::Module, "umbra")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation)
                ;
        }
    }

    EditorUmbraSceneComponent::EditorUmbraSceneComponent(const UmbraSceneComponentConfig& sceneComponentConfig)
        : BaseClass(sceneComponentConfig)
    {
    }

    void EditorUmbraSceneComponent::Activate()
    {
        BaseClass::Activate();
        UmbraSceneComponentNotificationBus::Handler::BusConnect(GetEntityId());
    }

    void EditorUmbraSceneComponent::Deactivate()
    {
        UmbraSceneComponentNotificationBus::Handler::BusDisconnect();
        BaseClass::Deactivate();
    }

    AZ::u32 EditorUmbraSceneComponent::GenerateUmbraScene()
    {
        // Prompt the user for a path to save the umbra scene
        const AZStd::string scenePath = AzQtComponents::FileDialog::GetSaveFileName(
            AzToolsFramework::GetActiveWindow(),
            QString("Select Umbra Scene File Path"),
            AZ::Utils::GetProjectPath().c_str(),
            QString("Umbra Scene (*.umbrascene)"),
            nullptr).toUtf8().constData();

        if (!scenePath.empty())
        {
            if (!ExportUmbraSceneFromLevel(
                    scenePath,
                    m_controller.GetConfiguration().m_collisionRadius,
                    m_controller.GetConfiguration().m_smallestHole,
                    m_controller.GetConfiguration().m_smallestOccluder))
            {
                QMessageBox::critical(
                    AzToolsFramework::GetActiveWindow(),
                    QObject::tr("Umbra Scene Not Saved"),
                    QObject::tr("Failed to save umbra scene from level entities."));
                return AZ::Edit::PropertyRefreshLevels::AttributesAndValues;
            }

            bool sceneAssetFound = false;
            AZ::Data::AssetInfo sceneAssetInfo;
            AZStd::string watchFolder;
            AzToolsFramework::AssetSystemRequestBus::BroadcastResult(
                sceneAssetFound,
                &AzToolsFramework::AssetSystemRequestBus::Events::GetSourceInfoBySourcePath,
                scenePath.c_str(),
                sceneAssetInfo,
                watchFolder);

            if (!sceneAssetFound)
            {
                QMessageBox::critical(
                    AzToolsFramework::GetActiveWindow(),
                    QObject::tr("Umbra Scene Asset Not Assigned"),
                    QObject::tr("Failed to assign the umbra scene asset generated from level entities after saving."));
                return AZ::Edit::PropertyRefreshLevels::AttributesAndValues;
            }

            UmbraSceneComponentRequestBus::Event(
                GetEntityId(),
                &UmbraSceneComponentRequestBus::Events::SetSceneAssetId,
                AZ::Data::AssetId(sceneAssetInfo.m_assetId.m_guid, 0));

            AzToolsFramework::ToolsApplicationRequestBus::Broadcast(
                &AzToolsFramework::ToolsApplicationRequestBus::Events::AddDirtyEntity, GetEntityId());
        }

        return AZ::Edit::PropertyRefreshLevels::AttributesAndValues;
    }
} // namespace Umbra
