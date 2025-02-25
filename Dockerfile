FROM ubuntu:22.04

RUN apt-get update
RUN apt-get install -y clang-12 libreadline-dev
RUN apt-get install -y make cmake valgrind

WORKDIR /app
ENV USER=root

ENTRYPOINT ["bash"]
