#===- llvm/utils/docker/example/build/Dockerfile -------------------------===//
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===----------------------------------------------------------------------===//
# This is an example Dockerfile to build an image that compiles clang.
# Replace FIXMEs to prepare your own image.

# Stage 1. Check out LLVM source code and run the build.

FROM ubuntu:18.04 as builder

LABEL maintainer "James Tuck <jtuck@ncsu.edu>"

# FIXME: Install llvm/clang build dependencies here. Including compiler to
# build stage1, cmake, subversion, ninja, etc.

RUN apt-get update && apt-get install -y \
        wget unzip ninja-build cmake zlib1g python subversion g++

ADD checksums /tmp/checksums
ADD scripts /tmp/scripts

# Checkout the source code.
ARG checkout_args
RUN /tmp/scripts/checkout.sh ${checkout_args}
# Run the build. Results of the build will be available at /tmp/clang-install/.
ARG buildscript_args
RUN /tmp/scripts/build_install_llvm.sh --to /tmp/clang-install ${buildscript_args}


# Stage 2. Produce a minimal release image with build results.
# FIXME: Replace 'ubuntu' with your base image.
FROM ubuntu:18.04
# FIXME: Change maintainer name.
LABEL maintainer "James Tuck <jtuck@ncsu.edu>"

# FIXME: Install all packages you want to have in your release container.
# A minimal useful installation should include at least libstdc++ and binutils.

RUN apt-get update && \
    apt-get install -y --no-install-recommends binutils python wget git unzip ninja-build cmake doxygen emacs graphviz flex bison libbison-dev libtool make flex libfl-dev libstdc++-7-dev && \
    rm -rf /var/lib/apt/lists/*

# Copy build results of stage 1 to /usr/local.
COPY --from=builder /tmp/clang-install/ /usr/local/
