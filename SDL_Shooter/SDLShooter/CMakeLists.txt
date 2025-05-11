cmake_minimum_required(VERSION 3.10.0)
# 标题
set(MyExecutable SDLShooter)
project(${MyExecutable} VERSION 0.1.0 LANGUAGES C CXX)

# 设置C++标准
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED True)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4)
elseif()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# # ctest ? 
# enable_testing()

# 设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/../bin)

# 添加头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 查找并载入cmake预设:if/elseif + include_directories
find_package(SDL2 REQUIRED PATHS ../Thirdparty/SDL2)
find_package(SDL2_image REQUIRED PATHS ../Thirdparty/SDL2_image)
find_package(SDL2_mixer REQUIRED PATHS ../Thirdparty/SDL2_mixer)
find_package(SDL2_ttf REQUIRED PATHS ../Thirdparty/SDL2_ttf)

# 添加可执行文件               
add_executable(${MyExecutable} main.cpp)

# 链接库
target_link_libraries(${MyExecutable} SDL2::SDL2 SDL2::SDL2main
                            SDL2_image::SDL2_image
                            SDL2_mixer::SDL2_mixer
                            SDL2_ttf::SDL2_ttf
                            )




