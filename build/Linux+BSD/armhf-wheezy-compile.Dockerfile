FROM debian:wheezy-backports

RUN dpkg --add-architecture armhf
RUN apt-get update

RUN apt-get install -y --force-yes \
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
 libglib2.0-dev:armhf \
 \
 qtbase5-dev:armhf \
 qttools5-dev:armhf \
 qttools5-dev-tools \
 qtquick1-5-dev:armhf \
 qtscript5-dev:armhf \
 libqt5xmlpatterns5-dev:armhf \
 libqt5svg5-dev:armhf \
 libqt5webkit5-dev:armhf
# libsndfile1:armhf \
# lame:armhf \ 
# \
# qtbase5-dev-tools #\
# perl:armhf perl-base:armhf
# libpulse0:armhf \
# libpulse-mainloop-glib0:armhf \
