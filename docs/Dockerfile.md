# Base docker image

[Dockerfile](../dockerfiles/Dockerfile) builds a base image `android-cpp-base-image` upon [Ubuntu 20.04 LTS](https://wiki.ubuntu.com/FocalFossa/ReleaseNotes).

> ⓘ **Info**
>
> This docker file also copies whole code

## Main packages
- [OpenJDK](https://openjdk.org/): The place to collaborate on an open-source implementation of the Java Platform, Standard Edition, and related projects.
- [cURL](https://curl.se/): Client-side URL transfer library
- [OpenSSL](https://openssl-library.org/): Full-featured toolkit for general-purpose cryptography and secure communication.
- [GNU Wget](https://www.gnu.org/software/wget/): Free software package for retrieving files using Internet protocols.
- [unzip](https://linux.die.net/man/1/unzip): List, test, or extract files from a ZIP archive.
- [build-essential](https://packages.debian.org/en/sid/build-essential): Collection of essential tools for compiling software.
- [CMake](https://cmake.org/cmake/help/latest/): Cross-Platform Makefile Generator.
- [Git](https://git-scm.com/): Distributed version control system.
- [Python 3](https://devguide.python.org/): Programming language to work quickly and integrate systems.
- [GCC 9](https://gcc.gnu.org/gcc-9/changes.html): GNU Compiler Collection with C++ 17 support.
- [Boost](https://www.boost.org/doc/libs/1_86_0/more/getting_started/unix-variants.html): Peer-reviewed portable C++ source libraries.

## Secondary packages
- [nlohmann/json](https://github.com/nlohmann/json): JSON for Modern C++.
- [Android NDK](https://developer.android.com/ndk/guides): The Native Development Kit that allows C/C++ code usage with Android.
- [Crow](https://github.com/CrowCpp/Crow): A Fast and Easy to use microframework for the web.
- [cpr](https://github.com/libcpr/cpr): Wrapper around libcurl.

## Building
`docker build -t android-cpp-base-image:latest -f dockerfiles/Dockerfile .`

> ⚠️ **Warning**
> 
> Pay attention to dot (.) at the end of this command, which helps to set docker build context.

## Running
`docker run --privileged -it --network host --volume=$PWD:/android-cpp --name android-cpp android-cpp-base-image:latest`

# Minimal docker image
[Dockerfile-1.0-minimal](../dockerfiles/test/Dockerfile-1.0-minimal) builds a `minimal` docker image only with installing [Ubuntu 20.04 LTS](https://wiki.ubuntu.com/FocalFossa/ReleaseNotes).

## Building
`docker build -t minimal:latest -f dockerfiles/test/Dockerfile-1.0-minimal .`

> ⚠️ **Warning**
> 
> Pay attention to dot (.) at the end of this command, which helps to set docker build context.

## Running
`docker run --privileged -it --network host --volume=$PWD:/android-cpp --name minimal minimal:latest`