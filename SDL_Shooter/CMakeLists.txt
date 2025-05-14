cmake_minimum_required(VERSION 3.10.0)
# 标题
project(SDLShooter VERSION 0.1.0 LANGUAGES C CXX)
set(MyExecutable ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
elseif()
    add_compile_options(-Wall -Wextra -Wpedantic)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()
set(CMAKE_BUILD_TYPE Release)

# 设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# 添加头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/src/include)

# 查找并载入cmake预设:if/elseif + include_directories/target_link_directories()
find_package(SDL2 REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Thirdparty/SDL2)
find_package(SDL2_image REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Thirdparty/SDL2_image)
find_package(SDL2_mixer REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Thirdparty/SDL2_mixer)
find_package(SDL2_ttf REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Thirdparty/SDL2_ttf)

# 添加可执行文件, 记得编译.cpp,尤其是刚写的; 也可以*.cpp             
add_executable(${MyExecutable} src/main.cpp
                                src/gameMgr.cpp
                                src/resourcesMgr.cpp
                                src/scene.cpp
                                src/sceneMain.cpp
                                src/sceneBegin.cpp
                                src/sceneEnd.cpp
                                src/player.cpp
                                )

# 链接库
target_link_libraries(${MyExecutable} SDL2::SDL2 SDL2::SDL2main
                                SDL2_image::SDL2_image
                                SDL2_mixer::SDL2_mixer
                                SDL2_ttf::SDL2_ttf
                            )


# 关闭控制台窗口
if(MSVC)
    target_link_options(${MyExecutable} PRIVATE "/SUBSYSTEM:WINDOWS")
else()
    target_link_options(${MyExecutable} PRIVATE -mwindows)
endif()

# 其他
# set(CMAKE_CXX_COMPILER "g++")
# set(CMAKE_GENERATOR "MinGW Makefiles")