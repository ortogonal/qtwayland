INCLUDEPATH += compositor_api

HEADERS += \
    compositor_api/qwaylandcompositor.h \
    compositor_api/qwaylandcompositor_p.h \
    compositor_api/qwaylandclient.h \
    compositor_api/qwaylandsurface.h \
    compositor_api/qwaylandsurface_p.h \
    compositor_api/qwaylandinput.h \
    compositor_api/qwaylandinput_p.h \
    compositor_api/qwaylandkeyboard.h \
    compositor_api/qwaylandkeyboard_p.h \
    compositor_api/qwaylandpointer.h \
    compositor_api/qwaylandpointer_p.h \
    compositor_api/qwaylandtouch.h \
    compositor_api/qwaylandtouch_p.h \
    compositor_api/qwaylandinputpanel.h \
    compositor_api/qwaylandoutput.h \
    compositor_api/qwaylanddrag.h \
    compositor_api/qwaylandbufferref.h \
    compositor_api/qwaylanddestroylistener.h \
    compositor_api/qwaylanddestroylistener_p.h \
    compositor_api/qwaylandview.h \
    compositor_api/qwaylandview_p.h \

SOURCES += \
    compositor_api/qwaylandcompositor.cpp \
    compositor_api/qwaylandclient.cpp \
    compositor_api/qwaylandsurface.cpp \
    compositor_api/qwaylandinput.cpp \
    compositor_api/qwaylandkeyboard.cpp \
    compositor_api/qwaylandpointer.cpp \
    compositor_api/qwaylandtouch.cpp \
    compositor_api/qwaylandinputpanel.cpp \
    compositor_api/qwaylandoutput.cpp \
    compositor_api/qwaylanddrag.cpp \
    compositor_api/qwaylandbufferref.cpp \
    compositor_api/qwaylanddestroylistener.cpp \
    compositor_api/qwaylandview.cpp \

QT += core-private

qtHaveModule(quick) {
    SOURCES += \
        compositor_api/qwaylandquickcompositor.cpp \
        compositor_api/qwaylandquicksurface.cpp \
        compositor_api/qwaylandquickoutput.cpp \
        compositor_api/qwaylandquickitem.cpp

    HEADERS += \
        compositor_api/qwaylandquickcompositor.h \
        compositor_api/qwaylandquicksurface.h \
        compositor_api/qwaylandquickoutput.h \
        compositor_api/qwaylandquickitem.h \
        compositor_api/qwaylandquickitem_p.h

    QT += qml qml-private quick quick-private
}
