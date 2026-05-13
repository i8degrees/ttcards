# ttcards.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/ttcards.git
# docker build templates/ttcards -t ttcards:0.13.1
# docker run --rm -it \
#   -v ~/Projects/nomlib.git/third-party:/ttcards/third-party \
#   -v $(pwd)/.build:/ttcards/build \
# ttcards:0.13.1
#

ARG VERSION=0.13.1
FROM local:0.13.1
#LABEL version="${VERSION}" \
LABEL version="0.13.1" \
      tag="i8degrees/ttcards:0.13.1" \
      description="" \
      maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/ttcards:latest

USER root
WORKDIR /ttcards
RUN apt-get update; \
    apt-get install -y --no-install-recommends \
    bzip2 \
    cmake \
    curl \
    doxygen \
    file \
    git \
    git-lfs \
    graphviz \
    pandoc \
    xdg-utils \
    zip \
    zstd

FROM i8degrees/nomlib:0.13.1
RUN ls -lhas /nomlib/build
WORKDIR /ttcards

# NOTE(JEFF): Any conveniences we wish to have within the container's shell
# should be handled here if we wish for it to be permanent.
RUN git clone \
  https://github.com/i8degrees/ttcards.git \
/ttcards && git checkout feature/LinuxPort

WORKDIR /ttcards/build
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;
#RUN cmake \
  #-DDEBUG=on -DDEBUG_ASSERT=on \
#-DEXAMPLES=off -DNOM_BUILD_TESTS=off \
#..

