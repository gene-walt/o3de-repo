#
# All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
# its licensors.
#

set(PAL_TRAIT_UMBRA_SUPPORTED TRUE)
set(PAL_TRAIT_UMBRA_TEST_SUPPORTED FALSE)
set(PAL_TRAIT_UMBRA_EDITOR_TEST_SUPPORTED FALSE)

set(UMBRA_TARGET "3rdParty::umbra")

set(UMBRA_LIBRARIES_DEBUG
    )
set(UMBRA_LIBRARIES_RELEASE
    )
set(UMBRA_STATIC_LIBRARIES_DEBUG
    ${UMBRA_SDK_PATH}/lib/ios/Debug/libumbraruntime.a
    ${UMBRA_SDK_PATH}/lib/ios/Debug/libumbracommon.a)
set(UMBRA_STATIC_LIBRARIES_RELEASE
    ${UMBRA_SDK_PATH}/lib/ios/Release/libumbraruntime.a
    ${UMBRA_SDK_PATH}/lib/ios/Release/libumbracommon.a)
