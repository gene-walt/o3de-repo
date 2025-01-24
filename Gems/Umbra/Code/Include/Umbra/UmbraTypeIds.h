/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

namespace Umbra
{
    // Module derived classes TypeIds
    inline constexpr const char* UmbraModuleInterfaceTypeId = "{72EFC227-63BF-414F-AAE8-69C871E64670}";
    inline constexpr const char* UmbraModuleTypeId = "{445776EF-014A-46C4-BE89-66C80D215BAD}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* UmbraEditorModuleTypeId = UmbraModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* UmbraRequestsTypeId = "{C0DE4C5D-043D-4BF2-9FCB-844D1E7ADCDF}";
} // namespace Umbra
