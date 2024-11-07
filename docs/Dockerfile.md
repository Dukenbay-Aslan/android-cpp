# Docker image

Docker file for building a docker base image is placed here:
```
android-cpp/dockerfiles/Dockerfile
```

This Docker file builds a base image `android-cpp-base-image` upon [Ubuntu 24.04.1 LTS](https://ubuntu.com/blog/ubuntu-desktop-24-04-noble-numbat-deep-dive).

## Main packages
- [GCC 13](https://gcc.gnu.org/gcc-13/): GNU Compiler Collection with C++ 23 support.
- [Python 3](https://devguide.python.org/): Programming language to work quickly and integrate systems.
- [Android NDK](https://developer.android.com/ndk/guides): The Native Development Kit that allows C/C++ code usage with Android.
- [OpenSSL](https://openssl-library.org/): Full-featured toolkit for general-purpose cryptography and secure communication.
- [GNU Wget](https://www.gnu.org/software/wget/): Free software package for retrieving files using Internet protocols. 
- [Git](https://git-scm.com/): Distributed version control system.
- [CMake](https://cmake.org/cmake/help/latest/): Cross-Platform Makefile Generator.
- [unzip](https://linux.die.net/man/1/unzip): List, test, or extract files from a ZIP archive.

## Secondary packages
- [Boost](https://www.boost.org/doc/libs/1_86_0/more/getting_started/unix-variants.html): Peer-reviewed portable C++ source libraries.
- [Crow](https://github.com/CrowCpp/Crow): A Fast and Easy to use microframework for the web.
- [cpr](https://github.com/libcpr/cpr): Wrapper around libcurl.
- [nlohmann/json](https://github.com/nlohmann/json): JSON for Modern C++.