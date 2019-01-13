# ECE 466/566 container for easy use on Windows, Linux, MacOS

FROM jamesmtuck/ubuntu-llvm7.0.1-release:latest

LABEL maintainer="jtuck@ncsu.edu"

# RUN apt-get update && apt-get install -y make flex libfl-dev libstdc++-7-dev

RUN apt-get update && apt-get install git-lfs

ADD . /projects
WORKDIR /projects

