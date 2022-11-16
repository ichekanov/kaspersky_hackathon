# syntax=docker/dockerfile:1

FROM debian:buster
LABEL maintainer="user"

# install dependencies
RUN apt-get update \
    && apt-get -y install \
        sudo \
        systemd \
        systemd-sysv \
        build-essential \
        wget \
        libffi-dev \
        libssl-dev \
        mc \
        git \
        cmake \
        autoconf \
        automake \
        libtool \
        gawk \
        parted \
        udev \
        python \
        make \
        qemu-system-arm \
        curl \
        wget \
        git \
        nano \
        vim \
        cmake-curses-gui \
        gdebi-core \
        device-tree-compiler \
    && apt-get autoremove --yes \
    && apt-get clean --yes

# install KasperskyOS
# COPY ./KasperskyOS-Community-Edition_1.1.0.356_amd64.deb /tmp/KOS.deb
ADD https://ds.mai307.ru/s/CrM38T9X2LtABnZ/download/KasperskyOS-Community-Edition_1.1.0.356_amd64.deb /tmp/KOS.deb
RUN gdebi --non-interactive /tmp/KOS.deb

# попытка заменить sh на bash
# RUN echo "dash dash/sh boolean false" | debconf-set-selections
# RUN DEBIAN_FRONTEND=noninteractive dpkg-reconfigure dash

# add user 'docker'
RUN useradd -r -u 1000 -U -m docker-KOS

# prepare working directory
RUN mkdir /workspace && chown -R docker-KOS /workspace
WORKDIR /workspace

ENV LC_ALL C.UTF-8
ENV LANG C.UTF-8
ENV LANGUAGE C.UTF-8
ENV PATH="${PATH}:/opt/KasperskyOS-Community-Edition-1.1.0.356/toolchain/bin"

RUN echo "export PS1='\u@\h \W $ '\n" >> /etc/profile
RUN echo "export PS1='\u@\h \W $ '\n" >> /root/.bashrc

# USER docker  
ENTRYPOINT [ "/bin/bash" , "-l" , "-c" ]
