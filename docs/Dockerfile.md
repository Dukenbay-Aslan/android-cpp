# Base docker image

[Dockerfile](../dockerfiles/Dockerfile) builds a base image `android-cpp-base-image` upon [Ubuntu 20.04 LTS](https://wiki.ubuntu.com/FocalFossa/ReleaseNotes).

<div style="border: 1px solid #17a2b8; background-color: #e8f7fc; padding: 15px; border-radius: 5px; display: flex; align-items: center; font-family: Arial, sans-serif;">
  <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="#17a2b8" width="24px" height="24px" style="margin-right: 10px;">
    <path d="M0 0h24v24H0z" fill="none"/>
    <path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8zm1-13h-2v2h2V7zm0 4h-2v6h2v-6z"/>
  </svg>
  <span style="color: #0c5460; font-weight: bold; font-size: 14px;">
    INFO (Testing):<br>This docker file also copies whole code
  </span>
</div>

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

<div style="border: 1px solid #f5c500; background-color: #fffbe6; padding: 15px; border-radius: 5px; display: flex; align-items: center; font-family: Arial, sans-serif;">
  <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="#f5c500" width="24px" height="24px" style="margin-right: 10px;">
    <path d="M0 0h24v24H0z" fill="none"/>
    <path d="M1 21h22L12 2 1 21zm13-3h-2v-2h2v2zm0-4h-2v-4h2v4z"/>
  </svg>
  <span style="color: #856404; font-weight: bold; font-size: 14px;">
    WARNING (Testing):<br>Pay attention to dot (.) at the end of this command, which helps to set docker build context.
  </span>
</div>

## Running
`docker run --privileged -it --network host --volume=$PWD:/android-cpp --name android-cpp android-cpp-base-image:latest`

# Minimal docker image
[Dockerfile-1.0-minimal](../dockerfiles/test/Dockerfile-1.0-minimal) builds a `minimal` docker image only with installing [Ubuntu 20.04 LTS](https://wiki.ubuntu.com/FocalFossa/ReleaseNotes).

## Building
`docker build -t minimal:latest -f dockerfiles/test/Dockerfile-1.0-minimal .`

<div style="border: 1px solid #f5c500; background-color: #fffbe6; padding: 15px; border-radius: 5px; display: flex; align-items: center; font-family: Arial, sans-serif;">
  <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="#f5c500" width="24px" height="24px" style="margin-right: 10px;">
    <path d="M0 0h24v24H0z" fill="none"/>
    <path d="M1 21h22L12 2 1 21zm13-3h-2v-2h2v2zm0-4h-2v-4h2v4z"/>
  </svg>
  <span style="color: #856404; font-weight: bold; font-size: 14px;">
    WARNING (Testing):<br>Pay attention to dot (.) at the end of this command, which helps to set docker build context.
  </span>
</div>

## Running
`docker run --privileged -it --network host --volume=$PWD:/android-cpp --name minimal minimal:latest`