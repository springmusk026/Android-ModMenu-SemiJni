cmake_minimum_required(VERSION 3.18.1)

set(LINKER_FLAGS "-ffixed-x18 -Wl,--hash-style=both")
set(C_FLAGS "-Werror=format -fdata-sections -ffunction-sections")
set(CXX_FLAGS "${CXX_FLAGS} -fno-exceptions -fno-rtti")

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(C_FLAGS "${C_FLAGS} -O2 -fvisibility=hidden -fvisibility-inlines-hidden")
    set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,-exclude-libs,ALL -Wl,--gc-sections -Wl,--strip-all")
else ()
    set(C_FLAGS "${C_FLAGS} -O0")
endif ()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C_FLAGS} ${CXX_FLAGS}")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")


add_library(libcurl STATIC IMPORTED)
set_target_properties(libcurl PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/prebuildssl/${CMAKE_ANDROID_ARCH_ABI}/libcurl.a")


set(LINKER_FLAGS "-ffixed-x18 -Wl,--hash-style=both")
set(C_FLAGS "-Werror=format -fdata-sections -ffunction-sections")
set(CXX_FLAGS "${CXX_FLAGS} -fno-exceptions -fno-rtti")


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C_FLAGS} ${CXX_FLAGS}")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_CXX_FLAGS "-Werror,-Wformat-security")

include_directories(include)
include_directories(
        ImGui
        ImGui/backends
)


include_directories(include ${ANDROID_NDK}/sources/android/native_app_glue)


include_directories(LGL2Cpp)
include_directories(Includes)
include_directories(Substrate)
include_directories(KittyMemory)
include_directories(Unity)
include_directories(Stuffs)
include_directories(Codes)

find_package(Dobby REQUIRED CONFIG)
include_directories(prefab/modules/dobby/include)

add_library(MyLibName SHARED
        Main.cpp
        LGL2Cpp/Il2Cpp.cpp
        KittyMemory/KittyMemory.cpp
        KittyMemory/MemoryPatch.cpp
        KittyMemory/MemoryBackup.cpp
        KittyMemory/KittyUtils.cpp
        ImGui/imgui.cpp
        ImGui/imgui_draw.cpp
        ImGui/imgui_demo.cpp
        ImGui/imgui_widgets.cpp
        ImGui/imgui_tables.cpp
        ImGui/backends/imgui_impl_opengl3.cpp
        ImGui/backends/imgui_impl_android.cpp
        )

target_link_libraries(MyLibName  dobby::dobby z log android EGL GLESv1_CM GLESv2 GLESv3)
