# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /usr/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH /lib/arm-linux-gnueabihf /usr/lib/arm-linux-gnueabihf /usr/include/arm-linux-gnueabihf)

#NOTE TO ERIC...I NEED TO SET THESES>>>>
SET(QT_BINARY_DIR   /usr/lib/x86_64-linux-gnu/qt5/bin)
#SET(QT_BINARY_DIR   /usr/lib/arm-linux-gnueabihf/qt5)
set(QT_LIBRARY_DIR  /usr/lib/arm-linux-gnueabihf)
#set(QT_QTCORE_LIBRARY   ${KDE_PREFIX}/lib/libQtCore4.a)
#set(QT_QTCORE_INCLUDE_DIR ${KDE_PREFIX}/include/QtCore)
#set(QT_MKSPECS_DIR  ${KDE_PREFIX}/mkspecs)
set(QT_MOC_EXECUTABLE  ${QT_BINARY_DIR}/moc)
set(QT_QMAKE_EXECUTABLE  ${QT_BINARY_DIR}/qmake)
set(QT_UIC_EXECUTABLE  ${QT_BINARY_DIR}/uic)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
