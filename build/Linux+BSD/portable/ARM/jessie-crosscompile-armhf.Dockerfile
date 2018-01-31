FROM debian:jessie

ADD https://raw.githubusercontent.com/ericfont/MuseScore/compile-armhf-jessie-with-buster-Qt/build/Linux%2BBSD/portable/RecipeDebian /
ADD https://raw.githubusercontent.com/ericfont/MuseScore/compile-armhf-jessie-with-buster-Qt/build/Linux%2BBSD/portable/ARM/preferences /etc/apt/
ADD https://raw.githubusercontent.com/ericfont/MuseScore/compile-armhf-jessie-with-buster-Qt/build/Linux%2BBSD/portable/ARM/sources.list /etc/apt/

RUN chmod +x RecipeDebian
RUN ./RecipeDebian --fetch-build-dependencies-only armhf
RUN ./RecipeDebian --fetch-package-dependencies-only armhf
