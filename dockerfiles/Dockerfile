# Installing Ubuntu 20.04 LTS as the base image
FROM ubuntu:20.04

# Setting environment variables
# Instructing apt to operate in non-interactive mode
ENV DEBIAN_FRONTEND=noninteractive
# Android NDK
ENV ANDROID_NDK_HOME=/android-ndk-r25
ENV PATH=$PATH:$ANDROID_NDK_HOME

RUN \
# Updating the package list
    apt update \
&&  apt upgrade -y \
# Installing following packages:
&&  apt install -y --no-install-recommends \
# ca-certificates: deb package that contains certificates provided by the Certificate Authorities
    ca-certificates \
# OpenJDK: The place to collaborate on an open-source implementation of the Java Platform, Standard Edition, and related projects.
    openjdk-11-jdk \
# cURL: Client-side URL transfer library
    libcurl4-openssl-dev \
# OpenSSL: Full-featured toolkit for general-purpose cryptography and secure communication.
    libssl-dev \
# GNU Wget: Free software package for retrieving files using Internet protocols.
    wget \
# List, test, or extract files from a ZIP archive.
    unzip \
# Collection of essential tools for compiling software.
    build-essential \
# CMake: Cross-Platform Makefile Generator.
    cmake \
# Git: Distributed version control system.
    git \
# Python 3: Programming language to work quickly and integrate systems.
    python3 \
# GCC 9: GNU Compiler Collection with C++ 17 support.
    gcc-9 \
    g++-9 \
# Asio: cross-platform C++ library for asynchronous network and low-level I/O
    libasio-dev \
# nlohmann/json: JSON for Modern C++.
    nlohmann-json3-dev \
# Clean up unnecessary files after package installation.
&&  apt clean \
&&  rm -rf /var/lib/apt/lists/*

# Setting GCC 9 as the default
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 100 \
&&  update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 100 \
# Update the list of trusted certificate authorities
&&  update-ca-certificates \
# Configure Git to Trust Certificates
&&  git config --global http.sslCAInfo /etc/ssl/certs/ca-certificates.crt

# Downloading and installing Android NDK.
# Android NDK: The Native Development Kit that allows C/C++ code usage with Android.
RUN wget https://dl.google.com/android/repository/android-ndk-r25-linux.zip \
&&  unzip android-ndk-r25-linux.zip -d / \
&&  rm android-ndk-r25-linux.zip

# Setting up CMake with Android NDK toolchain
ENV CMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake

# Installing packages
RUN \
# Crow: A Fast and Easy to use microframework for the web.
    cd /tmp \
&&  git clone https://github.com/CrowCpp/Crow.git \
&&  cd Crow \
&&  mkdir build \
&&  cd build \
&&  cmake -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF -DCROW_ENABLE_SSL=ON .. \
&&  make install \
&&  cd /tmp \
&&  rm -rf Crow \
# cpr: C++ wrapper around libcurl.
&&  cd /tmp \
&&  git clone https://github.com/libcpr/cpr.git \
&&  cd cpr \
&&  mkdir build \
&&  cd build \
&&  cmake .. -DCPR_USE_SYSTEM_CURL=ON -DBUILD_SHARED_LIBS=ON \
&&  cmake --build . --parallel \
&&  cmake --install . \
&&  cd /tmp \
&&  rm -rf cpr

# Mount code in docker container:
# --volume=$PWD:/android-cpp

# Setting the working directory
WORKDIR /android-cpp

# Running a command when the container starts
CMD ["/bin/bash"]