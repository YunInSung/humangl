include(ExternalProject)

set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)

# glfw3
ExternalProject_Add(
    dep_glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "master"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
    TEST_COMMAND ""
)
set(DEP_LIST ${DEP_LIST} dep_glfw)
set(DEP_LIBS ${DEP_LIBS} libglfw3.a)

# glad
ExternalProject_Add(
    dep_glad
    GIT_REPOSITORY "https://github.com/Dav1dde/glad"
    GIT_TAG "v0.1.34"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        -DGLAD_INSTALL=ON
    TEST_COMMAND ""
    )
set(DEP_LIST ${DEP_LIST} dep_glad)
set(DEP_LIBS ${DEP_LIBS} libglad.a)

# assimp
# ExternalProject_Add(
#     dep_assimp
#     GIT_REPOSITORY "https://github.com/assimp/assimp.git"
#     GIT_TAG "master"
#     GIT_SHALLOW 1
#     UPDATE_COMMAND ""
#     PATCH_COMMAND ""
#     CMAKE_ARGS
#         -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
#         -DBUILD_SHARED_LIBS=OFF
#         -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
#         -DASSIMP_BUILD_TESTS=OFF
#         -DASSIMP_INJECT_DEBUG_POSTFIX=OFF
#         -DASSIMP_BUILD_ZLIB=ON
#     TEST_COMMAND ""
#     )
# set(DEP_LIST ${DEP_LIST} dep_assimp)
# set(DEP_LIBS ${DEP_LIBS} libassimp.a)
# set(DEP_LIBS ${DEP_LIBS} libzlibstatic.a)

# stb_image
# ExternalProject_Add(
#     dep_stb
#     GIT_REPOSITORY "https://github.com/nothings/stb"
#     GIT_TAG "master"
#     GIT_SHALLOW 1
#     UPDATE_COMMAND ""
#     PATCH_COMMAND ""
#     CONFIGURE_COMMAND ""
#     BUILD_COMMAND ""
#     TEST_COMMAND ""
#     INSTALL_COMMAND ${CMAKE_COMMAND} -E copy
#         ${PROJECT_BINARY_DIR}/dep_stb-prefix/src/dep_stb/stb_image.h
#         ${DEP_INSTALL_DIR}/include/stb/stb_image.h
# )
# set(DEP_LIST ${DEP_LIST} dep_stb)

# glm
# ExternalProject_Add(
#     dep_glm
#     GIT_REPOSITORY "https://github.com/g-truc/glm"
#     GIT_TAG "master"
#     GIT_SHALLOW 1
#     UPDATE_COMMAND ""
#     PATCH_COMMAND ""
#     CONFIGURE_COMMAND ""
#     BUILD_COMMAND ""
#     TEST_COMMAND ""
#     INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${PROJECT_BINARY_DIR}/dep_glm-prefix/src/dep_glm/glm
#         ${DEP_INSTALL_DIR}/include/glm
# )
# set(DEP_LIST ${DEP_LIST} dep_glm)

# spdlog
# ExternalProject_Add(
#     dep-spdlog
#     GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
#     GIT_TAG "v1.x"
#     GIT_SHALLOW 1
#     UPDATE_COMMAND ""
#     PATCH_COMMAND ""
#     CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
#     TEST_COMMAND ""
# )
# set(DEP_LIST ${DEP_LIST} dep-spdlog)
# set(DEP_LIBS ${DEP_LIBS} libspdlog.a)

# imgui
# # clone imgui git repo
# include(FetchContent)
# FetchContent_Declare(
#         dep_imgui
#         GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
#         GIT_TAG "master"
# )
# FetchContent_MakeAvailable(dep_imgui)

# set(imgui_dir ${PROJECT_BINARY_DIR}/_deps/dep_imgui-src)

# file(COPY ${imgui_dir}/imconfig.h DESTINATION ${DEP_INCLUDE_DIR}/imgui)
# file(COPY ${imgui_dir}/imgui.h DESTINATION ${DEP_INCLUDE_DIR}/imgui)
# file(COPY ${imgui_dir}/imgui_internal.h DESTINATION ${DEP_INCLUDE_DIR}/imgui)
# file(COPY ${imgui_dir}/imstb_rectpack.h DESTINATION ${DEP_INCLUDE_DIR}/imgui)
# file(COPY ${imgui_dir}/imstb_textedit.h DESTINATION ${DEP_INCLUDE_DIR}/imgui)
# file(COPY ${imgui_dir}/imstb_truetype.h DESTINATION ${DEP_INCLUDE_DIR}/imgui)
# file(COPY ${imgui_dir}/misc/cpp/imgui_stdlib.h DESTINATION ${DEP_INCLUDE_DIR}/imgui)
# file(COPY ${imgui_dir}/backends/imgui_impl_glfw.h DESTINATION ${DEP_INCLUDE_DIR}/imgui)
# file(COPY ${imgui_dir}/backends/imgui_impl_opengl3.h DESTINATION ${DEP_INCLUDE_DIR}/imgui)
# file(COPY ${imgui_dir}/backends/imgui_impl_opengl3_loader.h DESTINATION ${DEP_INCLUDE_DIR}/imgui)

# # add all source code
# set(IMGUI_CODE
# ${imgui_dir}/imgui.cpp
# ${imgui_dir}/imgui_draw.cpp
# ${imgui_dir}/imgui_tables.cpp
# ${imgui_dir}/imgui_widgets.cpp
# ${imgui_dir}/misc/cpp/imgui_stdlib.cpp
# ${imgui_dir}/backends/imgui_impl_glfw.cpp
# ${imgui_dir}/backends/imgui_impl_opengl3.cpp
# )
# # make library
# add_library(imgui ${IMGUI_CODE})
# # change library path to DEP_LIB_DIR
# set_target_properties(imgui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${DEP_LIB_DIR})
# # set library path
# set(IMGUI_INCLUDE_DIR ${DEP_INCLUDE_DIR}/imgui)
# # set include header for imgui library
# target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR})
# target_include_directories(imgui PUBLIC ${DEP_INCLUDE_DIR})
# # others for imgui
# add_dependencies(imgui ${DEP_LIST})
# # for main target
# set(DEP_LIST ${DEP_LIST} imgui)
# set(DEP_LIBS ${DEP_LIBS} imgui)