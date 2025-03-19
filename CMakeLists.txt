cmake_minimum_required(VERSION 3.20)

project(CustomQuickchat VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.37.32822/bin/Hostx64/x64/cl.exe" CACHE FILEPATH "C Compiler" FORCE)
set(CMAKE_CXX_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.37.32822/bin/Hostx64/x64/cl.exe" CACHE FILEPATH "C++ Compiler" FORCE)
set(CMAKE_LINKER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.37.32822/bin/Hostx64/x64/link.exe" CACHE FILEPATH "Linker" FORCE)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_GENERATOR_TOOLSET "v143" CACHE STRING "Toolset version")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")  # `/MD`
#add_definitions(-DBOOST_EXCEPTION_DISABLE)
#add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
# Boost Config (Forces Dynamic Linking)
#set(Boost_USE_STATIC_LIBS OFF)  # Use shared libraries (.dll)
#set(Boost_USE_STATIC_RUNTIME OFF)  # Use dynamic runtime (/MD)
set(Boost_ARCHITECTURE "-x64")  # Force 64-bit architecture
set(Boost_USE_MULTITHREADED ON) # Ensure multi-threaded libs
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/DBOOST_ALL_DYN_LINK>)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/DCMAKE_BUILD_TYPE:Release>)
#add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/bigobj>)

set(compile_options
        /bigobj
        /Zm200 # increase heap space by 200%
    #    /wd4244 # Conversion from 'type1' to 'type2', possible loss of data
    #/wd4267 # Conversion from 'size_t' to 'type', possible loss of data
    #/wd4018 # Signed/unsigned mismatch
    #/wd4389 # Signed/unsigned mismatch
    #/external:W0 # Disable warnings from external headers
    #/external:I "${CMAKE_SOURCE_DIR}/lib" # mark lib/ as external
        /std:c++20
        /EHsc
        /MP
)

add_compile_options(${compile_options})

file(GLOB_RECURSE SDK_SOURCES "${CMAKE_SOURCE_DIR}/lib/SDK/**/*.cpp")

set(SRC_SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/Commands.cpp
    ${CMAKE_SOURCE_DIR}/src/Events.cpp
    ${CMAKE_SOURCE_DIR}/src/Functions.cpp
    ${CMAKE_SOURCE_DIR}/src/SpeechToText.cpp
    ${CMAKE_SOURCE_DIR}/src/TextEffects.cpp
    ${CMAKE_SOURCE_DIR}/src/LobbyInfo.cpp
    ${CMAKE_SOURCE_DIR}/src/core/Instances.cpp
    ${CMAKE_SOURCE_DIR}/src/core/Utils.cpp
    ${CMAKE_SOURCE_DIR}/src/core/WebsocketManager.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/Base.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/Tools.cpp
    ${CMAKE_SOURCE_DIR}/src/settings/CvarChangeCallbacks.cpp
    ${CMAKE_SOURCE_DIR}/src/settings/RegisterCvars.cpp
    ${CMAKE_SOURCE_DIR}/src/settings/Settings.cpp
)

set(LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/lib/SDK/GameDefines.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui_bm/imgui.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui_bm/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui_bm/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui_bm/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui_bm/imgui_rangeslider.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui_bm/imgui_searchablecombo.cpp
)

# Explicitly listing source files for better build performance and caching consistency.
add_library(CustomQuickchat SHARED
    ${SRC_SOURCES}
    ${LIB_SOURCES}
    ${SDK_SOURCES}
)

set_property(GLOBAL PROPERTY JOB_POOLS main_pool=2)  # Limit to 2 parallel jobs
set_property(TARGET CustomQuickchat PROPERTY JOB_POOL main_pool)

set(BOOST_ROOT "C:/local/boost-1.87.0")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")

set(CMAKE_PREFIX_PATH "${BOOST_ROOT}/stage/lib/cmake/Boost-1.87.0")
list(APPEND CMAKE_MODULE_PATH "${BOOST_ROOT}/stage/lib/cmake/Boost-1.87.0")

find_package(Boost 1.87.0 REQUIRED COMPONENTS
    system
    thread
    exception
    random
)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found. Please check your Boost installation.")
    message(STATUS "Boost paths configured as:")
    message(STATUS "  BOOST_ROOT: ${BOOST_ROOT}")
    message(STATUS "  CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
    message(STATUS "  CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
else()
    message(STATUS "Boost Found:")
    message(STATUS "  Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "  Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
endif()

set(SRC_INCLUDES
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/include/core
    ${CMAKE_SOURCE_DIR}/src/include/gui
)

set(EXTERNAL_INCLUDES
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/resources

    ${CMAKE_SOURCE_DIR}/lib/SDK
    ${CMAKE_SOURCE_DIR}/lib/SDK/SDK_HEADERS

    ${CMAKE_SOURCE_DIR}/lib/json/include/nlohmann
    ${CMAKE_SOURCE_DIR}/lib/json/include/nlohmann/detail
    ${CMAKE_SOURCE_DIR}/lib/json/include/nlohmann/thirdparty/hedley
    ${WEBSOCKETPP_INCLUDE_DIRS}
    ${BAKKESMOD_SDK_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib/imgui_bm

    ${CMAKE_SOURCE_DIR}/lib/json/include
    ${CMAKE_SOURCE_DIR}/lib/BakkesModSDK/include
    ${CMAKE_SOURCE_DIR}/lib/BakkesModSDK/include/bakkesmod/wrappers

    ${CMAKE_SOURCE_DIR}/lib/websocketpp
)
    

target_include_directories(CustomQuickchat PRIVATE
    ${SRC_INCLUDES}
    ${EXTERNAL_INCLUDES}
)

target_link_libraries(CustomQuickchat PRIVATE
    Boost::system Boost::thread Boost::exception Boost::random
    ${CMAKE_SOURCE_DIR}/lib/BakkesModSDK/lib/pluginsdk.lib
)

# Use more selective export handling instead of exporting all symbols
# This reduces the multiple export warnings
set_target_properties(CustomQuickchat PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)

find_program(
    CLANG_TIDY_EXE
    NAMES
        clang-tidy
    NO_CACHE
)

if(CLANG_TIDY_EXE)
    # Generate a files list without using command line paths
    set(FILES_LIST "${CMAKE_BINARY_DIR}/files_to_analyze.txt")
    file(WRITE ${FILES_LIST} "")

    # Add each source file on its own line
    foreach(SRC_FILE ${SRC_SOURCES})
        file(APPEND ${FILES_LIST} "${SRC_FILE}\n")
    endforeach()

    # Find header files and add them
    file(GLOB_RECURSE HEADER_FILES 
        "${CMAKE_SOURCE_DIR}/src/include/*.h"
        "${CMAKE_SOURCE_DIR}/src/include/*.hpp"
    )
    foreach(HEADER_FILE ${HEADER_FILES})
        file(APPEND ${FILES_LIST} "${SRC_FILE}\n")
    endforeach()
    
    # Create a small wrapper script that CMake will call
    set(WRAPPER_SCRIPT "${CMAKE_BINARY_DIR}/run_clang_tidy_wrapper.cmake")
    file(WRITE ${WRAPPER_SCRIPT} "
    # CMake script to run clang-tidy without shell quoting issues
    file(STRINGS \"${FILES_LIST}\" ALL_FILES)
    
    # Debug information to verify paths
    message(STATUS \"Looking for compilation database at: ${CMAKE_BINARY_DIR}/compile_commands.json\")
    if(EXISTS \"${CMAKE_BINARY_DIR}/compile_commands.json\")
        message(STATUS \"Compilation database found\")
    else()
        message(STATUS \"Compilation database NOT found\")
    endif()

    foreach(FILE \${ALL_FILES})
        message(STATUS \"Running clang-tidy on: \${FILE}\")
        execute_process(
            COMMAND \"${CLANG_TIDY_EXE}\" 
                    -p \"${CMAKE_BINARY_DIR}\"
                    --checks=modernize-*,performance-*,bugprone-*
                    --header-filter=\"^.*src.*$\"
                    \"\${FILE}\"
            RESULT_VARIABLE RESULT
        )
        
        if(NOT RESULT EQUAL 0)
            message(STATUS \"clang-tidy returned: \${RESULT} for \${FILE}\")
        endif()
    endforeach()
    ")
    
    # Add custom command to run the script before building
    add_custom_target(run_clang_tidy
        COMMAND ${CMAKE_COMMAND} -P "${WRAPPER_SCRIPT}"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy analysis"
        VERBATIM
    )

    #add_dependencies(CustomQuickchat run_clang_tidy)

endif()

add_custom_command(
    TARGET CustomQuickchat
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:CustomQuickchat>
	"$ENV{APPDATA}/bakkesmod/bakkesmod/plugins/CustomQuickchat-dev.dll"
)

add_custom_target(clean_but_not_lib
        COMMAND powershell -NoProfile -ExecutionPolicy Bypass -Command "& {Get-ChildItem -Path '${CMAKE_BINARY_DIR}' -Exclude 'lib' | Remove-Item -Recurse -Force}"

        COMMENT "Clean all except lib directory"
        VERBATIM
)

add_custom_target(clean_all
        COMMAND powershell -NoProfile -ExecutionPolicy Bypass -Command "& {Get-ChildItem -Path '${CMAKE_BINARY_DIR}' | Remove-Item -Recurse -Force}"

        COMMENT "Clean all"
        VERBATIM
)
