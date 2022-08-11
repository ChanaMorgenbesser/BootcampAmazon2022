QT -= gui

TEMPLATE = lib
DEFINES += GAZ_CAM_LIB_DLL_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#LIBS += /usr/lib/x86_64-linux-gnu$ -lgpeg
LIBS += -L /usr/lib/x86_64-linux-gnu  -lavcodec -lavformat -lavutil -lavcodec -ljpeg

#LIBS += /snap/gnome-3-38-2004/99/usr/lib/x86_64-linux-gnu/libjpeg.so
SOURCES += \
    gaz_cam_lib_dll.c \
    gaz_encoded.c \
    queue.c \
    snap_shot.c

HEADERS += \
    gaz_cam_lib_dll.h \
    gaz_encoded.h \
    queue.h \
    snap_shot.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
