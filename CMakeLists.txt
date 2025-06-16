# ==============================================================================
#           CMakeLists.txt for the Velvet project (Fully Portable)
# ==============================================================================

cmake_minimum_required(VERSION 3.18)
project(Velvet LANGUAGES CXX CUDA)

# --- (项目设置，与之前版本完全相同) ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_CUDA_ARCHITECTURES 75)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- (查找依赖，增加 OpenGL) ---
find_package(CUDAToolkit REQUIRED)

# 添加调试信息，检查 CUDAToolkit 是否真的被找到了
if(CUDAToolkit_FOUND)
    message(STATUS "CUDAToolkit Found!")
    message(STATUS "    Version: ${CUDAToolkit_VERSION}")
    message(STATUS "    Include Dirs: ${CUDAToolkit_INCLUDE_DIRS}")
    message(STATUS "    Libraries: ${CUDAToolkit_LIBRARIES}")
else()
    message(FATAL_ERROR "CUDAToolkit not found!")
endif()

find_package(OpenGL REQUIRED) # <--- 新增: 查找平台原生的OpenGL
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED COMPONENTS core opengl3-binding glfw-binding)

# 4. 定义源文件列表
#-------------------------------------------------------------------------------
# 将所有文件组织到变量中，以提高可读性
set(SOURCES
    # C++源文件
    "Velvet/main.cpp"
    "Velvet/Actor.cpp"
    "Velvet/Component.cpp"
    "Velvet/GameInstance.cpp"
    "Velvet/GUI.cpp"
    "Velvet/Helper.cpp"
    "Velvet/Input.cpp"
    "Velvet/MeshRenderer.cpp"
    "Velvet/Timer.cpp"
    "Velvet/VtEngine.cpp"
    "Velvet/stb_image.cpp"

    # CUDA源文件
    "Velvet/SpatialHashGPU.cu"
    "Velvet/VtClothSolverGPU.cu"

    # 头文件 (为了在IDE中正确显示和索引)
    "Velvet/Global.hpp"
    "Velvet/Actor.hpp"
    "Velvet/Camera.hpp"
    "Velvet/Component.hpp"
    "Velvet/GameInstance.hpp"
    "Velvet/GUI.hpp"
    "Velvet/Helper.hpp"
    "Velvet/Input.hpp"
    "Velvet/Light.hpp"
    "Velvet/Material.hpp"
    "Velvet/MaterialProperty.hpp"
    "Velvet/Mesh.hpp"
    "Velvet/MeshRenderer.hpp"
    "Velvet/PlayerController.hpp"
    "Velvet/RenderPipeline.hpp"
    "Velvet/Resource.hpp"
    "Velvet/Scene.hpp"
    "Velvet/Timer.hpp"
    "Velvet/Transform.hpp"
    "Velvet/VtEngine.hpp"
    "Velvet/Collider.hpp"
    "Velvet/Common.cuh"
    "Velvet/Common.hpp"
    "Velvet/MouseGrabber.hpp"
    "Velvet/ParticleGeometryRenderer.hpp"
    "Velvet/ParticleInstancedRenderer.hpp"
    "Velvet/SpatialHashCPU.hpp"
    "Velvet/SpatialHashGPU.cuh"
    "Velvet/SpatialHashGPU.hpp"
    "Velvet/VtBuffer.hpp"
    "Velvet/VtClothObjectCPU.hpp"
    "Velvet/VtClothObjectGPU.hpp"
    "Velvet/VtClothSolverCPU.hpp"
    "Velvet/VtClothSolverGPU.cuh"
    "Velvet/VtClothSolverGPU.hpp"
    "Velvet/External/cuda/helper_cuda.h"
    "Velvet/External/cuda/helper_string.h"
    "Velvet/External/stb_image.h"
)

# --- (定义可执行目标和链接库，这是关键修改) ---
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    # Graphics and Windowing
    OpenGL::GL          # <--- 修改: 使用CMake的跨平台OpenGL目标
    glfw
    glad::glad
    
    # Utility Libraries
    glm::glm
    assimp::assimp
    imgui::imgui
    fmt::fmt
)

if(TARGET CUDA::cudart)
    target_link_libraries(${PROJECT_NAME} PRIVATE CUDA::cudart)
    message(STATUS "Linking with modern CUDA::cudart target.")
else()
    # 如果现代目标不可用，回退到旧方式
    # 这也解释了为什么 CUDAToolkit::cudart_static 找不到
    message(WARNING "Modern CUDA::cudart target not found. Will rely on automatic linking.")
endif()

# --- (包含目录、编译定义、资源文件、文件过滤器等，均与之前版本相同) ---
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Velvet"
    "${CMAKE_CURRENT_SOURCE_DIR}/Velvet/External/cuda"
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32 WIN64 _CONSOLE)
endif()

file(COPY "Velvet/Assets" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Velvet" PREFIX "Source" FILES ${SOURCES})