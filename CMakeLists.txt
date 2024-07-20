cmake_minimum_required(VERSION 3.28)
project(DetectBug)
include(FetchContent)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

function(fetch_external_module NAME GIT_REPOSITORY REPO GIT_TAG TAG)
    if(NOT DEFINED LANGULUS_EXTERNAL_DIRECTORY)
        set(LANGULUS_EXTERNAL_DIRECTORY "${CMAKE_SOURCE_DIR}/external" CACHE PATH
            "Place where external dependencies will be downloaded")
        message(WARNING "LANGULUS_EXTERNAL_DIRECTORY not defined, using default: ${LANGULUS_EXTERNAL_DIRECTORY}")
    endif()

    message(STATUS "Fetching external library ${NAME}...")
    FetchContent_Declare(
        ${NAME}
        GIT_REPOSITORY  ${REPO}
        GIT_TAG         ${TAG}
        SOURCE_DIR      "${LANGULUS_EXTERNAL_DIRECTORY}/${NAME}-src"
        SUBBUILD_DIR    "${CMAKE_BINARY_DIR}/external/${NAME}-subbuild"
        ${ARGN}
		EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(${NAME})
	
	string(TOLOWER ${NAME} LOWERCASE_NAME)
    set(${NAME}_SOURCE_DIR "${${LOWERCASE_NAME}_SOURCE_DIR}" CACHE INTERNAL "${NAME} source directory")
    set(${NAME}_BINARY_DIR "${${LOWERCASE_NAME}_BINARY_DIR}" CACHE INTERNAL "${NAME} binary directory")
endfunction()

fetch_external_module(
    Catch2
    GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
    GIT_TAG         d4b0b34 #v2.x
)

fetch_external_module(
    SIMDe
    GIT_REPOSITORY  https://github.com/simd-everywhere/simde.git
    GIT_TAG         32c959c1039a15cec465bfa1103b4bcf34b3ac3a # master branch
)

add_executable(DetectBug Main.cpp TestPow.cpp)
target_compile_features(DetectBug PRIVATE cxx_std_23)
target_link_libraries(DetectBug PRIVATE Catch2)
target_include_directories(DetectBug PRIVATE ${SIMDe_SOURCE_DIR})

enable_testing()
add_test(NAME DetectBug COMMAND DetectBug)