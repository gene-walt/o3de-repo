/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

namespace Umbra
{
    // System Component TypeIds
    inline constexpr const char* UmbraSystemComponentTypeId = "{B60EFEF7-0B01-4D21-8CBC-47D9B22FFC98}";
    inline constexpr const char* UmbraEditorSystemComponentTypeId = "{E7D9C845-EAA8-42F9-8B7F-F1DE9C8D6108}";

    // Module derived classes TypeIds
    inline constexpr const char* UmbraModuleInterfaceTypeId = "{72EFC227-63BF-414F-AAE8-69C871E64670}";
    inline constexpr const char* UmbraModuleTypeId = "{445776EF-014A-46C4-BE89-66C80D215BAD}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* UmbraEditorModuleTypeId = UmbraModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* UmbraRequestsTypeId = "{C0DE4C5D-043D-4BF2-9FCB-844D1E7ADCDF}";
} // namespace Umbra
