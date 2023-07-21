#
# All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
# its licensors.
#

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "s3://ly-dev-3p-packages/")
set(package_name umbra3-3.4.24)
set(pk_package_hash 89ec8451247e9fc42d04f9b15380df51601df1aaf3a929087c7bc0f5d890f009)

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS umbra PACKAGE_HASH ${pk_package_hash})
set(O3DE_UMBRA_TARGET "3rdParty::umbra")

set(PAL_TRAIT_UMBRA_SUPPORTED TRUE)
set(PAL_TRAIT_UMBRA_TEST_SUPPORTED FALSE)
set(PAL_TRAIT_UMBRA_EDITOR_TEST_SUPPORTED FALSE)
