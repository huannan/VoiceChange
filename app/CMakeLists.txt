#配置CMake的最低版本
cmake_minimum_required(VERSION 3.4.1)

#配置工程路径
#set(path_project /home/wuhuannan/AndroidPrj/VoiceChange)
set(path_project D:/AndroidStudio/VoiceChange)

#添加自己写的库
add_library(effect-lib
            SHARED
            src/main/cpp/effect_fix.cpp
            )

#添加两个预编译的so库
add_library( fmod
             SHARED
             IMPORTED
            )

add_library( libfmodL
             SHARED
             IMPORTED
            )

#设置两个预编译的库的路径
set_target_properties(
                       fmod
                       PROPERTIES IMPORTED_LOCATION
                       ${path_project}/app/libs/${ANDROID_ABI}/libfmod.so
                    )

set_target_properties(
                       libfmodL
                       PROPERTIES IMPORTED_LOCATION
                       ${path_project}/app/libs/${ANDROID_ABI}/libfmodL.so
                    )

#找到Android的log库（这个库已经在Android平台中了）
find_library(
            log-lib
            log
            )

#把需要的库链接到自己的库中
target_link_libraries(
            effect-lib
            ${log-lib}
            fmod
            libfmodL
            )