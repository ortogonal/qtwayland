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

#ifndef QWAYLANDCOMPOSITOR_H
#define QWAYLANDCOMPOSITOR_H

#include <QtWaylandCompositor/qwaylandexport.h>
#include <QtWaylandCompositor/qwaylandextension.h>
#include <QtWaylandCompositor/QWaylandOutput>

#include <QObject>
#include <QImage>
#include <QRect>

struct wl_display;

QT_BEGIN_NAMESPACE

class QInputEvent;

class QMimeData;
class QUrl;
class QOpenGLContext;
class QWaylandCompositorPrivate;
class QWaylandClient;
class QWaylandSurface;
class QWaylandInputDevice;
class QWaylandGlobalInterface;
class QWaylandView;
class QWaylandPointer;
class QWaylandKeyboard;
class QWaylandTouch;

class Q_COMPOSITOR_EXPORT QWaylandCompositor : public QWaylandObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QWaylandCompositor)
    Q_PROPERTY(QByteArray socketName READ socketName WRITE setSocketName)
    Q_PROPERTY(bool retainedSelection READ retainedSelectionEnabled WRITE setRetainedSelectionEnabled)
    Q_PROPERTY(QWaylandOutput *defaultOutput READ defaultOutput WRITE setDefaultOutput NOTIFY defaultOutputChanged)
    Q_PROPERTY(bool useHardwareIntegrationExtension READ useHardwareIntegrationExtension WRITE setUseHardwareIntegrationExtension NOTIFY useHardwareIntegrationExtensionChanged)
    Q_PROPERTY(QWaylandInputDevice *defaultInputDevice READ defaultInputDevice NOTIFY defaultInputDeviceChanged)

public:
    QWaylandCompositor(QObject *parent = 0);
    virtual ~QWaylandCompositor();

    virtual void create();
    bool isCreated() const;

    void setSocketName(const QByteArray &name);
    QByteArray socketName() const;

    ::wl_display *display() const;
    uint32_t nextSerial();

    QList<QWaylandClient *>clients() const;
    Q_INVOKABLE void destroyClientForSurface(QWaylandSurface *surface);
    Q_INVOKABLE void destroyClient(QWaylandClient *client);

    QList<QWaylandSurface *> surfaces() const;
    QList<QWaylandSurface *> surfacesForClient(QWaylandClient* client) const;

    Q_INVOKABLE QWaylandOutput *outputFor(QWindow *window) const;

    QWaylandOutput *defaultOutput() const;
    void setDefaultOutput(QWaylandOutput *output);
    QList<QWaylandOutput *> outputs() const;

    uint currentTimeMsecs() const;

    void setRetainedSelectionEnabled(bool enabled);
    bool retainedSelectionEnabled() const;
    void overrideSelection(const QMimeData *data);

    QWaylandInputDevice *defaultInputDevice() const;

    QWaylandView *createSurfaceView(QWaylandSurface *surface);

    QWaylandInputDevice *inputDeviceFor(QInputEvent *inputEvent);

    bool useHardwareIntegrationExtension() const;
    void setUseHardwareIntegrationExtension(bool use);

public Q_SLOTS:
    void processWaylandEvents();

Q_SIGNALS:
    void createSurface(QWaylandClient *client, uint id, int version);
    void surfaceCreated(QWaylandSurface *surface);
    void surfaceAboutToBeDestroyed(QWaylandSurface *surface);

    void defaultOutputChanged();
    void defaultInputDeviceChanged();

    void useHardwareIntegrationExtensionChanged();
protected:
    virtual void retainedSelectionReceived(QMimeData *mimeData);
    virtual QWaylandInputDevice *createInputDevice();
    virtual QWaylandPointer *createPointerDevice(QWaylandInputDevice *inputDevice);
    virtual QWaylandKeyboard *createKeyboardDevice(QWaylandInputDevice *inputDevice);
    virtual QWaylandTouch *createTouchDevice(QWaylandInputDevice *inputDevice);

    QWaylandCompositor(QWaylandCompositorPrivate &dptr, QObject *parent = 0);
};

QT_END_NAMESPACE

#endif // QWAYLANDCOMPOSITOR_H
