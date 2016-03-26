FROM ericfont/armv7hf-debian-qemu:jessie

RUN [ "cross-build-start" ]

# maybe in future build will want to get backports so can use latest Qt version
#RUN echo "deb http://http.debian.net/debian jessie-backports main" >> /etc/apt/sources.list

RUN apt-get update

# need to be able to use https for wget
RUN apt-get install ca-certificates wget #git
#RUN git config --global http.sslVerify true

# get musescore non-qt dev dependencies
RUN apt-get install alsa-base libsndfile1 libasound2-dev portaudio19-dev libsndfile1-dev zlib1g-dev libfreetype6-dev libfreetype6 lame libmp3lame-dev libssl-dev libdrm-dev libgl1-mesa-dev libpulse-dev 

# get qt5
RUN apt-get install qtbase5-dev qttools5-dev qttools5-dev-tools qtquick1-5-dev qtscript5-dev libqt5xmlpatterns5-dev libqt5svg5-dev libqt5webkit5-dev

# get other dependencies needed by gcc
RUN wget https://raw.githubusercontent.com/ericfont/MuseScore/compile-armhf/build/Linux%2BBSD/portable/RecipeArm && chmod +x RecipeArm && ./RecipeArm --fetch-dependencies-only

# get prebuilt AppImageKit
RUN wget https://bintray.com/artifact/download/ericfont/prebuilt-AppImageKit/AppImageKit-5_built-in-armv7hf-jessie.tar.gz && tar -xvzf AppImageKit-5_built-in-armv7hf-jessie.tar.gz 

RUN [ "cross-build-end" ]  
