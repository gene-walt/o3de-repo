/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/ComponentBus.h>

namespace Umbra
{
    //! EditorUmbraSceneComponentRequestBus provides an interface for EditorUmbraSceneComponent
    class EditorUmbraSceneComponentRequests : public AZ::ComponentBus
    {
    public:
        using MutexType = AZStd::mutex;

        //! @brief Enumerate geometry from the current level and export it as an Umbra scene and descriptor.
        //! @param scenePath Path where the scene file will be saved. The scene descriptor will be saved at the same path with a different extension.
        //! @return true if the scene was successfully exported, otherwise false.
        virtual bool ExportUmbraScene(const AZStd::string& scenePath) const = 0;

        //! @brief Determine if an entity can be exported as part of the Umbra scene.
        //! @param entityId The entity id that will be checked.
        //! @return true if the entity can be exported as part of the scene, otherwise false. 
        virtual bool ShouldExportEntity(const AZ::EntityId& entityId) const = 0;
    };
    using EditorUmbraSceneComponentRequestBus = AZ::EBus<EditorUmbraSceneComponentRequests>;

    //! EditorUmbraSceneComponentNotificationBus notifications are sent when EditorUmbraSceneComponent changes.
    class EditorUmbraSceneComponentNotifications : public AZ::ComponentBus
    {
    public:
    };
    using EditorUmbraSceneComponentNotificationBus = AZ::EBus<EditorUmbraSceneComponentNotifications>;
} // namespace Umbra
