

FROM baseimages/ubuntu_1604_amd64



# ensure commands are printed and errors propagated
# create build user (and group)
RUN set -xe; \
    addgroup --gid 1000 build; \
    adduser --home /build --shell /bin/bash --uid 1000 --ingroup build --disabled-password --gecos Build build


# include toolchains in path
ENV PATH /usr/local/lib/utilities:/opt/arm-buildroot-linux-gnueabihf/usr/bin:/opt/ae-toolchain/usr/bin:/usr/local/lib/ecog1x-cyan-elf-toolchain/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

RUN mkdir /BuildArea
WORKDIR "/BuildArea"

RUN apt-get update
RUN apt-get install -y build-essential git python


RUN apt-get install -y binutils-arm-none-eabi
RUN apt-get install -y gcc-arm-none-eabi
RUN apt-get install -y gdb-arm-none-eabi


#ENTRYPOINT ["/bin/bash"]
#CMD ["ChildBuild.sh"]




