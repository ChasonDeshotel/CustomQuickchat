cmake_minimum_required(VERSION 3.20)

project(CustomQuickchat VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64) 

#add_definitions(-DBOOST_EXCEPTION_DISABLE)
#add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")  # `/MD`

set(compile_options
    /bigobj
    #    /wd4244 # Conversion from 'type1' to 'type2', possible loss of data
    #/wd4267 # Conversion from 'size_t' to 'type', possible loss of data
    #/wd4018 # Signed/unsigned mismatch
    #/wd4389 # Signed/unsigned mismatch
    #/external:W0 # Disable warnings from external headers
    #/external:I "${CMAKE_SOURCE_DIR}/lib" # mark lib/ as external
)

add_compile_options(${compile_options})

file(GLOB_RECURSE SDK_SOURCES "${CMAKE_SOURCE_DIR}/lib/SDK/**/*.cpp")

set(SRC_SOURCES
    src/main.cpp
    src/Commands.cpp
    src/Events.cpp
    src/Functions.cpp
    src/SpeechToText.cpp
    src/TextEffects.cpp
    src/LobbyInfo.cpp
    src/core/Instances.cpp
    src/core/Utils.cpp
    src/core/WebsocketManager.cpp
    src/gui/Base.cpp
    src/gui/Tools.cpp
    src/settings/CvarChangeCallbacks.cpp
    src/settings/RegisterCvars.cpp
    src/settings/Settings.cpp
)

set(LIB_SOURCES
    lib/SDK/GameDefines.cpp
    lib/imgui_bm/imgui.cpp
    lib/imgui_bm/imgui_demo.cpp
    lib/imgui_bm/imgui_draw.cpp
    lib/imgui_bm/imgui_widgets.cpp
    lib/imgui_bm/imgui_rangeslider.cpp
    lib/imgui_bm/imgui_searchablecombo.cpp
)

# Explicitly listing source files for better build performance and caching consistency.
add_library(CustomQuickchat SHARED
    ${SRC_SOURCES}
    ${LIB_SOURCES}
    ${SDK_SOURCES}
)

# Boost Config (Forces Dynamic Linking)
set(Boost_USE_STATIC_LIBS OFF)  # Use shared libraries (.dll)
set(Boost_USE_STATIC_RUNTIME OFF)  # Use dynamic runtime (/MD)
add_definitions(-DBOOST_ALL_DYN_LINK)

set(BOOST_ROOT "C:/local/boost_1_87_0")
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

find_program(
    CLANG_TIDY_EXE
    NAMES
        clang-tidy
    NO_CACHE
)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY
        "${CLANG_TIDY_EXE}"
        #        "--quiet"
        #        "--header-filter=.*src.*$"
        #        "--system-headers=0"
        #        "--extra-arg=-isystem${CMAKE_SOURCE_DIR}/libC:/Users/Billy Maizere/source/repos/CustomQuickchat/lib"
        #        "--extra-arg-before=--driver-mode=cl"
    )
    message(STATUS "CLANG TIDY: ${CLANG_TIDY_EXE}")
    #    set_target_properties(CustomQuickchat PROPERTIES CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")
    # Create a separate static library for your code
        add_library(project_sources STATIC ${SRC_SOURCES})
        
        # Add the same include directories and compile definitions
        target_include_directories(project_sources PRIVATE 
            ${SRC_INCLUDES}
            ${EXTERNAL_INCLUDES}
        )
        
        # Copy compile definitions
        target_compile_definitions(project_sources PRIVATE
            BOOST_ALL_DYN_LINK
            BOOST_EXCEPTION_DISABLE
            _ITERATOR_DEBUG_LEVEL=0
            CustomQuickchat_EXPORTS
        )
    
    # Apply clang-tidy ONLY to this static library with specific header filter
        set_target_properties(project_sources PROPERTIES
    	#CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--quiet;--header-filter=^${CMAKE_SOURCE_DIR}/src/include/.*$"
            CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--quiet;--header-filter=.*src.*$;--system-headers=0"
        )
    
    #    # Remove source files from main target
        set_target_properties(CustomQuickchat PROPERTIES 
            SOURCES "${LIB_SOURCES};${SDK_SOURCES}"
        )
    
    # Link the static library to your main target
    target_link_libraries(CustomQuickchat PRIVATE project_sources)
endif()

add_custom_command(
    TARGET CustomQuickchat
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:CustomQuickchat>
	"$ENV{APPDATA}/bakkesmod/bakkesmod/plugins/CustomQuickchat-dev.dll"
)
