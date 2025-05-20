cmake_minimum_required(VERSION 3.10.0)
# 标题
project(GhostEscape VERSION 0.1.0 LANGUAGES C CXX)
set(MyExecutable ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /utf-8")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /utf-8")
elseif()
    add_compile_options(-Wall -Wextra -Wpedantic)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()
set(CMAKE_BUILD_TYPE Release)

# 设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# 添加三方库include/lib
# 查找并载入cmake预设:if/elseif + include_directories/target_link_directories()
# 注意这里下载的库是VC的SDL3;.gitignore忽略了库文件，也行吧
find_package(SDL3 REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Thirdparty/SDL3)
find_package(SDL3_image REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Thirdparty/SDL3)
find_package(SDL3_mixer REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Thirdparty/SDL3)
find_package(SDL3_ttf REQUIRED PATHS ${CMAKE_SOURCE_DIR}/Thirdparty/SDL3)
find_package(glm CONFIG REQUIRED PATHS ${CMAKE_SOURCE_DIR}/ThirdParty/glm)

# 添加可执行文件      
aux_source_directory(src some_sources)  # 可有可无,为了方便编辑器检测新添加源文件
file(GLOB_RECURSE my_sources src/*.cpp src/*.h)
message(STATUS "源文件: ${my_sources}")
add_executable(${MyExecutable} ${my_sources})



# 链接库
target_link_libraries(${MyExecutable}   ${SDL3_LIBRARIES}
                                        SDL3_image::SDL3_image
                                        SDL3_mixer::SDL3_mixer
                                        SDL3_ttf::SDL3_ttf
                                        glm::glm
                            )


# # 关闭控制台窗口
# if(MSVC)
#     Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
# else()
#     target_link_options(${MyExecutable} PRIVATE -mwindows)
# endif()

