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

# This is for arm64. You should change the folder name to 'android-armeabi-v7a' if you use x86.
set(UMBRA_STATIC_LIBRARIES_DEBUG
    ${UMBRA_SDK_PATH}/lib/android-arm64-v8a/libumbraruntime_d.a
    ${UMBRA_SDK_PATH}/lib/android-arm64-v8a/libumbracommon_d.a)
set(UMBRA_STATIC_LIBRARIES_RELEASE
    ${UMBRA_SDK_PATH}/lib/android-arm64-v8a/libumbraruntime.a
    ${UMBRA_SDK_PATH}/lib/android-arm64-v8a/libumbracommon.a)
