#
# All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
# its licensors.
#

set(PAL_TRAIT_UMBRA_SUPPORTED TRUE)
set(PAL_TRAIT_UMBRA_TEST_SUPPORTED FALSE)
set(PAL_TRAIT_UMBRA_EDITOR_TEST_SUPPORTED FALSE)

set(O3DE_UMBRA_TARGET "3rdParty::umbra")

set(UMBRA_LIBRARIES_DEBUG
    )
set(UMBRA_LIBRARIES_RELEASE
    )
set(UMBRA_STATIC_LIBRARIES_DEBUG
    ${LY_UMBRA_INSTALL_PATH}/lib/win64_md_vs2019/umbracommon64_d.lib
    ${LY_UMBRA_INSTALL_PATH}/lib/win64_md_vs2019/umbraobjectgrouper64_d.lib
    ${LY_UMBRA_INSTALL_PATH}/lib/win64_md_vs2019/umbraruntime64_d.lib
    ${LY_UMBRA_INSTALL_PATH}/lib/win64_md_vs2019/umbraoptimizer_static64_d.lib)
set(UMBRA_STATIC_LIBRARIES_RELEASE
    ${LY_UMBRA_INSTALL_PATH}/lib/win64_md_vs2019/umbracommon64.lib
    ${LY_UMBRA_INSTALL_PATH}/lib/win64_md_vs2019/umbraobjectgrouper64.lib
    ${LY_UMBRA_INSTALL_PATH}/lib/win64_md_vs2019/umbraruntime64.lib
    ${LY_UMBRA_INSTALL_PATH}/lib/win64_md_vs2019/umbraoptimizer_static64.lib)
