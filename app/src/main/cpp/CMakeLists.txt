# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.16.3)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("android_cpp" VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include external libraries
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(cpr REQUIRED)

# Configure crow to use SSL
message(STATUS "Configuring crow to use SSL")
add_definitions(-DCROW_ENABLE_SSL)
message(STATUS "Configuring crow to use SSL -- done")

# Option to build the project as a shared library (default)
# or as an executable to run and test the C++ part only.
option(EXECUTABLE "Build as an executable instead of a shared library" OFF)
option(DEBUG "Turn on debug instructions" OFF)

# Add debug instructions
if(DEBUG)
    set(CMAKE_CXX_FLAGS "-g")
endif()

if(NOT EXECUTABLE)
    # JNI configuration
    set(JAVA_HOME "/usr/lib/jvm/java-11-openjdk-amd64")
    include_directories(${JAVA_HOME}/include)
    include_directories(${JAVA_HOME}/include/linux)
    set(JVM_LIBRARY ${JAVA_HOME}/lib/server/libjvm.so)
endif()

# C++ source files
set(SOURCE_FILES
    src/app/action/action.cpp
    src/app/requests/manual.cpp
    src/app/services/SActions.cpp
    src/app/wsmanager/wsmanager.cpp
    src/base/config/Config.cpp
    src/base/helpers/mappers.cpp
    src/base/helpers/utils.cpp
    src/base/helpers/valid.cpp
    src/base/logger/logger.cpp
    src/Main.cpp)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.

# Building C++ code as a shared library: libandroidcpp.so
if(EXECUTABLE)
    add_executable(${CMAKE_PROJECT_NAME}
        ${SOURCE_FILES})
    target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE Threads::Threads
        PRIVATE OpenSSL::SSL
        PRIVATE OpenSSL::Crypto
        PRIVATE cpr::cpr
        PRIVATE nlohmann_json::nlohmann_json)
else()
    add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        ${SOURCE_FILES}
        native-lib.cpp)
    # Specifies libraries CMake should link to your target library. You
    # can link libraries from various origins, such as libraries defined in this
    # build script, prebuilt third-party libraries, or Android system libraries.
    target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        log
        PRIVATE Threads::Threads
        PRIVATE OpenSSL::SSL
        PRIVATE OpenSSL::Crypto
        PRIVATE cpr::cpr
        PRIVATE nlohmann_json::nlohmann_json
        PRIVATE ${JVM_LIBRARY})
    # Set Android NDK Configuration
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_TOOLCHAIN_FILE} CACHE STRING "Android NDK Toolchain")
    set(CMAKE_SYSTEM_NAME Android)  # Cross-compilation for Android
    set(CMAKE_ANDROID_ARCH_ABI arm64-v8a)  # Set your architecture (e.g., arm64-v8a, armeabi-v7a)
    set(CMAKE_ANDROID_NDK ${ANDROID_NDK_HOME})  # Android NDK location
    set(CMAKE_ANDROID_STL_TYPE c++_shared)  # Use shared STL
    # Installation Setup
    install(TARGETS myandroidcpp
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
    )
    message(STATUS "JNI Library Path: ${JVM_LIBRARY}")
    message(STATUS "Boost Libraries: ${Boost_LIBRARIES}")
    message(STATUS "Android NDK: ${ANDROID_NDK_HOME}")
    message(STATUS "CMake Toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()