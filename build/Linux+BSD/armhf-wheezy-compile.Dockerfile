FROM debian:wheezy

RUN dpkg --add-architecture armhf
RUN apt-get update

RUN apt-get install -y gcc-4.7:armhf

RUN apt-get install -y libsndfile1:armhf libsndfile1-dev:armhf libasound2-dev:armhf

