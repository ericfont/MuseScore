
# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)


# specify the cross compiler
SET(CMAKE_C_COMPILER   /usr/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)
SET(CMAKE_STRIP        /usr/bin/arm-linux-gnueabihf-strip)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH /lib/arm-linux-gnueabihf /usr/lib/arm-linux-gnueabihf /usr/include/arm-linux-gnueabihf)

set(QT_INCLUDE_DIR           /usr/include/arm-linux-gnueabihf)
set(QT_QT_INCLUDE_DIR        ${QT_INCLUDE_DIR}/qt5)
set(QT_QTCORE_INCLUDE_DIR    ${QT_QT_INCLUDE_DIR}/QtCore)
set(QT_QTXML_INCLUDE_DIR     ${QT_QT_INCLUDE_DIR}/QtXml)
set(QT_QTGUI_INCLUDE_DIR     ${QT_QT_INCLUDE_DIR}/QtGui)
set(QT_QTNETWORK_INCLUDE_DIR ${QT_QT_INCLUDE_DIR}/QtNetwork)
set(QT_QTUITOOLS_INCLUDE_DIR ${QT_QT_INCLUDE_DIR}/QtUiTools)
set(QT_QTSCRIPT_INCLUDE_DIR  ${QT_QT_INCLUDE_DIR}/QtScript)
set(QT_QTWEBKIT_INCLUDE_DIR  ${QT_QT_INCLUDE_DIR}/QtWebkit)
set(QT_LIBRARY_DIR           ${CROSSQT}/lib)

#NOTE TO ERIC...I NEED TO SET THESES>>>>
SET(QT_BINARY_DIR   /usr/lib/x86_64-linux-gnu/qt5/bin)
#SET(QT_BINARY_DIR   /usr/lib/arm-linux-gnueabihf/qt5)
set(QT_LIBRARY_DIR  /usr/lib/arm-linux-gnueabihf)
#set(QT_QTCORE_LIBRARY   ${KDE_PREFIX}/lib/libQtCore4.a)
#set(QT_QTCORE_INCLUDE_DIR ${KDE_PREFIX}/include/QtCore)
#set(QT_MKSPECS_DIR  ${KDE_PREFIX}/mkspecs)
set(QT_MOC_EXECUTABLE  ${QT_BINARY_DIR}/moc)
set(QT_UIC_EXECUTABLE  ${QT_BINARY_DIR}/uic)
set(QT_QMAKE_EXECUTABLE  ${QT_BINARY_DIR}/qmake)
set(QT_RCC_EXECUTABLE  /usr/bin/rcc)

set(QT_INCLUDES ${QT_INCLUDE_DIR} ${QT_QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTXML_INCLUDE_DIR} ${QT_GUI_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR} ${QT_QTWEBKIT_INCLUDE_DIR} )

set(QT_CROSS_LIBRARIES QtSvg5 QtGui5 QtCore5 QtXml5 QtNetwork5 QtWebKit5 QtXmlPatterns5 QtDeclarative5)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)