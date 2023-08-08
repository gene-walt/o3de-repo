/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/ComponentBus.h>
#include <Umbra/UmbraSceneAsset/UmbraSceneAsset.h>

namespace AZ
{
    class Aabb;
    class EntityId;
    class Matrix4x4;
    class Vector3;
} // namespace AZ

namespace Umbra
{
    //! UmbraSceneComponentRequestBus provides an interface for UmbraSceneComponent
    class UmbraSceneComponentRequests : public AZ::ComponentBus
    {
    public:
        //! Sets the scene asset used by the component.
        virtual void SetSceneAsset(AZ::Data::Asset<UmbraSceneAsset> asset) = 0;
        //! Sets the scene asset used by the component by AssetId.
        virtual void SetSceneAssetId(const AZ::Data::AssetId& assetId) = 0;
        //! Sets the scene asset used by the component by path.
        virtual void SetSceneAssetPath(const AZStd::string& path) = 0;
        //! Returns the scene asset used by the component.
        virtual AZ::Data::Asset<const UmbraSceneAsset> GetSceneAsset() const = 0;
        //! Returns the AssetId for the scene used by the component.
        virtual AZ::Data::AssetId GetSceneAssetId() const = 0;
        //! Update the scenes visibility state based on the camera transform. 
        virtual bool UpdateVisibility(const AZ::Matrix4x4& cameraWorldToClip, const AZ::Vector3& cameraWorldPos) = 0;
        //! Returns true if the entity is not occluded based on the current scene state. 
        virtual bool IsEntityVisible(const AZ::EntityId& entityId) const = 0;
        //! Returns true if the AABB is visible based on the current scene state. 
        virtual bool IsAabbVisible(const AZ::Aabb& bounds) const = 0;
    };
    using UmbraSceneComponentRequestBus = AZ::EBus<UmbraSceneComponentRequests>;

    //! UmbraSceneComponentNotificationBus notifications are sent when UmbraSceneComponent changes.
    class UmbraSceneComponentNotifications : public AZ::ComponentBus
    {
    public:
    };
    using UmbraSceneComponentNotificationBus = AZ::EBus<UmbraSceneComponentNotifications>;
} // namespace Umbra
