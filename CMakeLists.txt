cmake_minimum_required(VERSION 3.21)
set(This GL)
project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

get_directory_property(ParentDirectory PARENT_DIRECTORY)

if(ParentDirectory STREQUAL "")

    # Define option for linking with the runtime statically.
    if(MSVC)
        option(RUNTIME_STATIC_LINKAGE "Link with runtime library statically" OFF)
    endif(MSVC)

    # If the runtime static linkage option is turned on, replace
    # compiler flag that selects runtime library variant.
    if(MSVC AND RUNTIME_STATIC_LINKAGE)
        foreach(flag
                CMAKE_C_FLAGS
                CMAKE_CXX_FLAGS
                CMAKE_C_FLAGS_DEBUG
                CMAKE_CXX_FLAGS_DEBUG
                CMAKE_C_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_RELEASE
                CMAKE_C_FLAGS_MINSIZEREL
                CMAKE_CXX_FLAGS_MINSIZEREL
                CMAKE_C_FLAGS_RELWITHDEBINFO
                CMAKE_CXX_FLAGS_RELWITHDEBINFO
                )
            if(${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "$(${flag})")
            endif(${flag} MATCHES "/MD")
        endforeach(flag)
    endif(MSVC AND RUNTIME_STATIC_LINKAGE)

    # Organize application, libraries, etc. in folder in the IDE.
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Ensure proper loading of side-loaded shared libraries
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    if(APPLE)
        set(CMAKE_INSTALL_RPATH "@loader_path")
    elseif(UNIX)
        set(CMAKE_INSTALL_RPATH "\$ORIGIN")
    endif()

    # Don't add the goofy "d" postfix for debug builds of libraries
    set(CMAKE_DEBUG_POSTFIX "")

    # Suppress Microsoft warnings about functions being
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    include(FindPkgConfig) # <-- include pkg-config needed by FindLibreSSL.cmake script

    add_subdirectory(rabbitmq-c)
    add_subdirectory(deps/glm)
    add_subdirectory(deps/glfw)

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

    include_directories(C:\\VulkanSDK\\1.2.198.1\\Include)
    include_directories(deps/glm)
    include_directories(deps/glfw/include)
    include_directories(deps/tinyObjLoader)

    add_subdirectory(Abstractions)
    add_subdirectory(AMQP)
    add_subdirectory(Engine)
    add_subdirectory(Game)

    #add_custom_command(
    #        TARGET ${This} PRE_BUILD
    #        COMMAND ${CMAKE_COMMAND} -E copy
    #        ${CMAKE_SOURCE_DIR}/Libs/apex/Documentation/TML_API-APEX_Interface_v1.0.0.pdf
    #        ${CMAKE_CURRENT_BINARY_DIR}/bin/TML_API-APEX_Interface_v1.0.0.pdf)

endif(ParentDirectory STREQUAL "")
