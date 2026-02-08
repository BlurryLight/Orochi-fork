cmake_minimum_required(VERSION 3.10)
project(Course VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Platform and architecture
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Orochi Course requires 64-bit build")
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

# Platform-specific definitions
if(WIN32)
    add_definitions(-D__WINDOWS__ -D_WIN32 -D_CRT_SECURE_NO_WARNINGS)
    # Disable specific warnings on Windows
    if(MSVC)
        add_compile_options(/wd4244 /wd4305 /wd4018)
    endif()
elseif(UNIX AND NOT APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ldl")
endif()

# Include Orochi CUDA enablement script
include(${CMAKE_SOURCE_DIR}/../Orochi/enable_cuew.cmake)

# Common include directories
set(COURSE_COMMON_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/../
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../Test
)

# Common source files
set(COURSE_COMMON_SOURCES
    ${CMAKE_SOURCE_DIR}/common/Common.cpp
)

set(COURSE_COMMON_HEADERS
    ${CMAKE_SOURCE_DIR}/common/Common.h
)

# Orochi source files
set(OROCHI_SOURCES
    ${CMAKE_SOURCE_DIR}/../Orochi/Orochi.cpp
    ${CMAKE_SOURCE_DIR}/../Orochi/OrochiUtils.cpp
)

set(OROCHI_HEADERS
    ${CMAKE_SOURCE_DIR}/../Orochi/Orochi.h
    ${CMAKE_SOURCE_DIR}/../Orochi/OrochiUtils.h
    ${CMAKE_SOURCE_DIR}/../Orochi/GpuMemory.h
)

# Contrib source files (cuew and hipew)
set(CONTRIB_SOURCES
    ${CMAKE_SOURCE_DIR}/../contrib/cuew/src/cuew.cpp
    ${CMAKE_SOURCE_DIR}/../contrib/hipew/src/hipew.cpp
)

set(CONTRIB_HEADERS
    ${CMAKE_SOURCE_DIR}/../contrib/cuew/include/cuew.h
    ${CMAKE_SOURCE_DIR}/../contrib/hipew/include/hipew.h
)

# Function to create a course project
function(add_course_project PROJECT_NAME)
    # Gather project-specific sources
    file(GLOB PROJECT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

    # Add any .natvis files for debugging
    file(GLOB PROJECT_VISUALIZER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.natvis)

    # Create executable
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${PROJECT_VISUALIZER_FILES}
        ${COURSE_COMMON_SOURCES}
        ${COURSE_COMMON_HEADERS}
        ${OROCHI_SOURCES}
        ${OROCHI_HEADERS}
        ${CONTRIB_SOURCES}
        ${CONTRIB_HEADERS}
    )

    # Set include directories
    target_include_directories(${PROJECT_NAME} PRIVATE ${COURSE_COMMON_INCLUDE_DIRS})
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/..)

    # Platform-specific linking
    if(WIN32)
        target_link_libraries(${PROJECT_NAME} version)
    elseif(UNIX AND NOT APPLE)
        target_link_libraries(${PROJECT_NAME} dl)
    endif()

    # Copy DLLs to output directory
    if(WIN32)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/../contrib/bin/win64
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying runtime DLLs to output directory"
        )
    endif()

    # Set output directory
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}
    )

    # Add suffix for Debug configuration on Windows
    if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            OUTPUT_NAME "${PROJECT_NAME}64D"
        )
    elseif(WIN32)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            OUTPUT_NAME "${PROJECT_NAME}64"
        )
    endif()
endfunction()

# Add all course projects
add_subdirectory(00_HelloWorld)
add_subdirectory(01_Reduction)
add_subdirectory(02_PrefixScan)
add_subdirectory(03_Enqueue)
add_subdirectory(04_BottomUpTraversal)
add_subdirectory(05_WaterfallScheme)
add_subdirectory(06_PersistentThreads)
add_subdirectory(07_PoolAllocator)
add_subdirectory(08_LinearProbing)
add_subdirectory(09_RadixSort)
