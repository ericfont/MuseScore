FROM debian:jessie

ADD https://raw.githubusercontent.com/ericfont/MuseScore/compile-armhf/build/Linux%2BBSD/portable/RecipeDebian /RecipeDebian

RUN "bash RecipeDebian --fetch-build-dependencies-only armhf"
RUN "bash RecipeDebian --fetch-package-dependencies-only armhf"
