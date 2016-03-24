#!/bin/bash
# Run this script from MuseScore's root directory

# Build portable Linux AppImages and upload them to Bintray. AppImages will
# always be uploaded unless a list of specific branches is passed in. e.g.:
#    $   build.sh  --upload-branches  master  my-branch-1  my-branch-2
# Builds will be for the native architecture (64 bit) unless another is
# specified for cross-compiling. (e.g. build.sh --32bit)

set -e # exit on error
set -x # echo commands

date="$(date -u +%Y%m%d%H%M)"

branch="$TRAVIS_BRANCH"
[ "$branch" ] || branch="$(git rev-parse --abbrev-ref HEAD)"

revision="$(echo "$TRAVIS_COMMIT" | cut -c 1-7)"
[ "$revision" ] || revision="$(make revision && cat mscore/revision.h)"

if [ "$(grep '^[[:blank:]]*set( *MSCORE_UNSTABLE \+TRUE *)' CMakeLists.txt)" ]
then # Build is marked UNSTABLE inside CMakeLists.txt
  if [ "${BINTRAY_REPO_OWNER}" == "musescore" ]
  then # This is a nightly build
    makefile_overrides="PREFIX='MuseScoreNightly-$date-$branch-$revision' \
                        SUFFIX='-portable-nightly' \
                        LABEL='Portable Nightly Build'"
    cp -f build/travis/job2_AppImage/splash-nightly.png mscore/data/splash.png
  else
    # This is someone developing on their own fork
    makefile_overrides="PREFIX='MuseScoreDev-$date-$branch-$revision' \
                        SUFFIX='-portable-dev' \
                        LABEL='Unofficial Developer Build'"
  fi
else
  # Build is STABLE so create a stable release!
  makefile_overrides="" # use Makefile defaults
fi

# Build AppImage. Are we cross-compiling?
#case "$1" in
#  --32bit )
#    shift
#    # Build MuseScore AppImage inside 32-bit Docker image
#    docker run -i -v "${PWD}:/MuseScore" toopher/centos-i386:centos6 /bin/bash -c \
#      "linux32 --32bit i386 /MuseScore/build/Linux+BSD/portable/Recipe $makefile_overrides"
#    ;;
#  * )
#    [ "$1" == "--64bit" ] && shift || true
#    # Build MuseScore AppImage inside native (64-bit) Docker image
#    docker run -i -v "${PWD}:/MuseScore" library/centos:6 /bin/bash -c \
#      "/MuseScore/build/Linux+BSD/portable/Recipe $makefile_overrides"
#    ;;
#esac

# associate arm binaries with qemu-arm-static.  https://resin.io/blog/building-arm-containers-on-any-x86-machine-even-dockerhub/
sudo umount binfmt_misc
sudo mount binfmt_misc -t binfmt_misc /proc/sys/fs/binfmt_misc
echo ':arm:M::\x7fELF\x01\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x28\x00:\xff\xff\xff\xff\xff\xff\xff\x00\xff\xff\xff\xff\xff\xff\xff\xff\xfe\xff\xff\xff:/usr/bin/qemu-arm-static:' > sudo /proc/sys/fs/binfmt_misc/register  

# arm build docker script...first get prebuilt AppImageKit for armv7
tar -xvzf build/Linux+BSD/AppImageKit-5_built-in-armv7hf-jessie.tar.gz --directory ..
docker run -i -v "${PWD}:/MuseScore" -v "${PWD}/../AppImageKit-5:/AppImageKit" ericfont/musescore:compile-armhf /bin/bash -c "cd /MuseScore && CMAKE_TOOLCHAIN_FILE=/MuseScore/build/Linux+BSD/jessie-crosscompile-armhf.cmake make"


# Should the AppImage be uploaded?
if [ "$1" == "--upload-branches" ] && [ "$2" != "ALL" ]; then
  # User passed in a list of zero or more branches so only upload those listed
  shift
  for upload_branch in "$@" ; do
    [ "$branch" == "$upload_branch" ] && upload=true || true # bypass `set -e`
  done
else
  # No list passed in (or specified "ALL"), so upload on every branch
  upload=true
fi

if [ "${upload}" ]; then
  # Upload AppImage to Bintray
  ./build/travis/job2_AppImage/bintray.sh build.release/MuseScore*.AppImage
else
  echo "On branch '$branch' so AppImage will not be uploaded." >&2
fi
