FROM debian:jessie

# add cross compiler tool chain for armhf to aptitude
ADD http://emdebian.org/tools/debian/emdebian-toolchain-archive.key key
RUN apt-key add key && \
 echo "deb http://emdebian.org/tools/debian/ jessie main" > /etc/apt/sources.list.d/crosstools.list && \
 dpkg --add-architecture armhf

# note: install musescore deps & qt5 for armhf arch, since cross compiling
RUN apt-get clean && apt-get update && apt-get install -y \
 libqt4-dev:armhf \
 libsndfile1-dev:armhf \
 libasound2-dev:armhf \
 portaudio19-dev:armhf \
 zlib1g-dev:armhf \
 libfreetype6-dev:armhf \
 libmp3lame-dev:armhf \
 libssl-dev:armhf \
 libdrm-dev:armhf \
 libgl1-mesa-dev:armhf \
 libpulse-dev:armhf \
 \
 qtbase5-dev:armhf \
 qttools5-dev:armhf \
 qtquick1-5-dev:armhf \
 qtscript5-dev:armhf \
 libqt5xmlpatterns5-dev:armhf \
 libqt5svg5-dev:armhf \
 libqt5webkit5-dev:armhf \
 \
 qttools5-dev-tools:armhf \
 \
 cross-gcc-dev \
 binutils-arm-linux-gnueabihf \
 crossbuild-essential-armhf \
 qemu-user-static \
 cmake \
 git \
 \
 curl \
 wget

RUN apt-get download qtbase5-dev-tools:armhf && dpkg --force-all -i qtbase5-dev-tools_5.3.2+dfsg-4+deb8u1_armhf.deb && \
 apt-get download alsa-tools:armhf && dpkg --force-all -i alsa-tools_1.0.28-1_armhf.deb && \
 apt-get download alsa-utils:armhf && dpkg --force-all -i alsa-utils_1.0.28-1_armhf.deb && \
 apt-get download alsa-base        && dpkg --force-all -i alsa-base_1.0.27+1_all.deb && \
 apt-get download pkg-config:armhf && dpkg --force-all -i pkg-config_0.28-1.1_armhf.deb && \
 apt-get download pulseaudio:armhf && dpkg --force-all -i pulseaudio_5.0-13_armhf.deb

