QT -= gui

TEMPLATE = lib
DEFINES += GAZ_CAM_LIB_DLL_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gaz_cam_lib_dll.c \
    queue.c \
    snap_shot.c

HEADERS += \
    gaz_cam_lib_dll.h \
    queue.h \
    snap_shot.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
