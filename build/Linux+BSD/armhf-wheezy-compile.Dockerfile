FROM debian:wheezy-backports

RUN dpkg --add-architecture armhf
RUN apt-get update

RUN apt-get install -y \
 gcc-4.7:armhf \
 libsndfile1:armhf \
 libsndfile1-dev:armhf \
 libasound2-dev:armhf \
 portaudio19-dev:armhf \
 zlib1g-dev:armhf \
 libfreetype6-dev:armhf \
 lame:armhf \ 
 libmp3lame-dev:armhf \
 libssl-dev:armhf \
 libdrm-dev:armhf \
 libgl1-mesa-dev:armhf \
 libpulse-dev:armhf \
 libglib2.0-dev:armhf

