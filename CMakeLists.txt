#标题
cmake_minimum_required(VERSION 3.10.0)
project(deskpet VERSION 0.1.0 LANGUAGES C CXX)

#c++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

#include和link路径
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)


#编译提示
add_compile_options(/W4)

#色织编译输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})
#可执行文件
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})
add_executable(${TARGET} 
                        "src/main.cpp"
                        "src/Pet.cpp"
                        "src/Game.cpp"
                        "src/animation.cpp"
                      
                        )

                        if(MSVC)
target_compile_options(${TARGET} PRIVATE /utf-8)
endif()

#链接库
target_link_libraries(${TARGET} 
                           ${SDL2_LIBRARIES}
                           SDL2_image::SDL2_image
                           SDL2_mixer::SDL2_mixer
                            
                           )

# 如果存在 assets 目录，则在构建后复制到可执行文件目录，保证运行时能找到图片/音效等资源
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/assets"
                "$<TARGET_FILE_DIR:${TARGET}>/assets"
        COMMENT "Copying assets folder to runtime directory"
    )
endif()