/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtWaylandCompositor module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWAYLANDINPUT_H
#define QWAYLANDINPUT_H

#include <QtCore/qnamespace.h>
#include <QtCore/QPoint>
#include <QtCore/QString>

#include <QtWaylandCompositor/qwaylandexport.h>
#include <QtWaylandCompositor/qwaylandextension.h>
#include <QtWaylandCompositor/qwaylandkeyboard.h>

QT_BEGIN_NAMESPACE

class QWaylandCompositor;
class QWaylandSurface;
class QKeyEvent;
class QTouchEvent;
class QWaylandView;
class QInputEvent;
class QWaylandInputDevicePrivate;
class QWaylandDrag;
class QWaylandKeyboard;
class QWaylandPointer;
class QWaylandTouch;

namespace QtWayland {
class InputDevice;
}

class Q_COMPOSITOR_EXPORT QWaylandInputDevice : public QWaylandObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QWaylandInputDevice)
public:
    enum CapabilityFlag {
        // The order should match the enum WL_SEAT_CAPABILITY_*
        Pointer = 0x01,
        Keyboard = 0x02,
        Touch = 0x04,

        DefaultCapabilities = Pointer | Keyboard | Touch
    };
    Q_DECLARE_FLAGS(CapabilityFlags, CapabilityFlag)
    Q_ENUM(CapabilityFlags)

    QWaylandInputDevice(QWaylandCompositor *compositor, CapabilityFlags capabilityFlags = DefaultCapabilities);
    virtual ~QWaylandInputDevice();

    void sendMousePressEvent(Qt::MouseButton button);
    void sendMouseReleaseEvent(Qt::MouseButton button);
    void sendMouseMoveEvent(QWaylandView *surface , const QPointF &localPos, const QPointF &outputSpacePos = QPointF());
    void sendMouseWheelEvent(Qt::Orientation orientation, int delta);

    void sendKeyPressEvent(uint code);
    void sendKeyReleaseEvent(uint code);

    void sendFullKeyEvent(QKeyEvent *event);
    void sendFullKeyEvent(QWaylandSurface *surface, QKeyEvent *event);

    void sendTouchPointEvent(int id, const QPointF &point, Qt::TouchPointState state);
    void sendTouchFrameEvent();
    void sendTouchCancelEvent();

    void sendFullTouchEvent(QTouchEvent *event);

    QWaylandPointer *pointer() const;
    //Normally set by the mouse device,
    //But can be set manually for use with touch or can reset unset the current mouse focus;
    QWaylandView *mouseFocus() const;
    void setMouseFocus(QWaylandView *view);

    QWaylandKeyboard *keyboard() const;
    QWaylandSurface *keyboardFocus() const;
    bool setKeyboardFocus(QWaylandSurface *surface);
    void setKeymap(const QWaylandKeymap &keymap);

    QWaylandTouch *touch() const;

    QWaylandCompositor *compositor() const;

    QWaylandDrag *drag() const;

    QWaylandInputDevice::CapabilityFlags capabilities() const;

    virtual bool isOwner(QInputEvent *inputEvent) const;

    static QWaylandInputDevice *fromSeatResource(struct ::wl_resource *resource);

Q_SIGNALS:
    void mouseFocusChanged(QWaylandView *newFocus, QWaylandView *oldFocus);
    void cursorSurfaceRequest(QWaylandSurface *surface, int hotspotX, int hotspotY);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QWaylandInputDevice::CapabilityFlags)

QT_END_NAMESPACE

#endif // QWAYLANDINPUT_H
