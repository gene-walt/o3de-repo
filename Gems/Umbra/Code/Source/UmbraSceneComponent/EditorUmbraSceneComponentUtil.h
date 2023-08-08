/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 */

#pragma once

#include <AzCore/std/string/string.h>

namespace Umbra
{
    bool ExportUmbraSceneFromLevel(const AZStd::string& scenePath, float collisionRadius, float smallestHole, float smallestOccluder);
} // namespace Umbra