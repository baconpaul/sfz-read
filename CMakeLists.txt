cmake_minimum_required(VERSION 3.10)
project(sfz-read VERSION 1.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

set(SFZ_READ_COMMON_SOURCES
    src/sfz-read.cpp
)

set(SFZ_READ_COMMON_INCLUDES
    src
    libs/PEGTL/include
)

add_executable(sfz-read-main
    ${SFZ_READ_COMMON_SOURCES}
   src/main.cpp
)

target_compile_features(sfz-read-main
    PRIVATE
        cxx_std_17
)

target_compile_definitions(sfz-read-main
    PRIVATE
)

target_include_directories(sfz-read-main
    PRIVATE
        ${SFZ_READ_COMMON_INCLUDES}
)

find_package(LibSndFile ${PACKAGE_OPTION})
if(NOT LIBSNDFILE_FOUND)
    message(WARNING "LibSndFile not installed; building without wav support")
    message(WARNING "You can 'brew install libsndfile' or 'apt-get install libsndfile1-dev'")
else()
    target_compile_definitions(sfz-read-main
        PRIVATE
            LIBSNDFILE=1
    )
    target_link_libraries(sfz-read-main ${LIBSNDFILE_LIBRARIES})
    include_directories(sfz-read-main ${LIBSNDFILE_INCLUDE_DIRS})
endif()

# *NIX
if (UNIX)
    target_compile_options(sfz-read-main
        PRIVATE
    )
endif()

# macOS
if (APPLE)
    target_compile_definitions(sfz-read-main
        PRIVATE
            MAC=1
    )

    target_include_directories(sfz-read-main
        PRIVATE
    )

    target_link_libraries(sfz-read-main
        "-framework ApplicationServices"
        "-framework CoreFoundation"
    )
endif()

# Linux
if (UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)

    target_compile_definitions(sfz-read-main
        PRIVATE
            LINUX=1
    )

    target_sources(sfz-read-main
        PRIVATE
    )

    target_include_directories(sfz-read-main
        PRIVATE
    )

    target_link_libraries(sfz-read-main
        stdc++fs
        Threads::Threads
    )
endif()

if (WIN32)
    target_compile_definitions(sfz-read-main
        PRIVATE
            WINDOWS=1
            NOMINMAX=1
    )

    target_compile_options(sfz-read-main
        PRIVATE
            /arch:SSE2
            "/FI precompiled.h"
            /Zc:alignedNew
            /bigobj
    )

    target_link_libraries(sfz-read-main winmm.lib)
endif()
