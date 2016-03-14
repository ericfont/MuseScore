FROM debian:wheezy-backports

RUN dpkg --add-architecture armhf
RUN apt-get update

RUN apt-get install -y libsndfile1:armhf

