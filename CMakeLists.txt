cmake_minimum_required(VERSION 3.5)

project(VVChat LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(VVChat SHARED
    ./thirdParty/material/lib/src
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    signup.cpp
    signup.h
    signup.ui
    signin.cpp
    signin.h
    signin.ui
    ListViewData.h
    ListViewData.cpp
  )
else()
  add_executable(VVChat
    ./thirdParty/material/lib/src
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    signup.cpp
    signup.h
    signup.ui
    signin.cpp
    signin.h
    signin.ui
    res.qrc
    ListViewData.h
    ListViewData.cpp
    ${RESOURCES}
  )
endif()

include_directories(./thirdParty/material/lib/include)

qt5_add_resources(RESOURCES ${RESOURCES_DIR}/res.qrc)
source_group("Resource Files" FILES res.qrc)

add_library(
    libcomponents
    STATIC
    IMPORTED
)
 set_target_properties(
    libcomponents
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/thirdParty/material/lib/libcomponents.a
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
    ListViewData.cpp
)


target_link_libraries(VVChat PRIVATE Qt5::Widgets PRIVATE libcomponents)


