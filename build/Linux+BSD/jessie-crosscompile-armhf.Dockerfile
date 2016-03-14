FROM debian:jessie

RUN echo "deb http://emdebian.org/tools/debian/ jessie main" > /etc/apt/sources.list.d/crosstools.list
RUN apt-get update
RUN apt-get install -y curl
RUN curl http://emdebian.org/tools/debian/emdebian-toolchain-archive.key | apt-key add -
RUN dpkg --add-architecture armhf
RUN apt-get update
RUN apt-get install -y \
 crossbuild-essential-armhf \
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
 qttools5-dev-tools \
 qtquick1-5-dev:armhf \
 qtscript5-dev:armhf \
 libqt5xmlpatterns5-dev:armhf \
 libqt5svg5-dev:armhf \
 libqt5webkit5-dev:armhf \
 qemu


