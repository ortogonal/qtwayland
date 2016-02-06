/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mockclient.h"
#include "testcompositor.h"
#include "testkeyboardgrabber.h"
#include "testinputdevice.h"

#include "qwaylandview.h"
#include "qwaylandbufferref.h"
#include "qwaylandinput.h"

#include <QtTest/QtTest>

class tst_WaylandCompositor : public QObject
{
    Q_OBJECT

public:
    tst_WaylandCompositor() {
        setenv("XDG_RUNTIME_DIR", ".", 1);
    }

private slots:
    void inputDeviceCapabilities();
    void keyboardGrab();
    void inputDeviceCreation();
    void inputDeviceKeyboardFocus();
    void singleClient();
    void multipleClients();
    void geometry();
    void mapSurface();
    void frameCallback();
};

void tst_WaylandCompositor::singleClient()
{
    TestCompositor compositor;
    compositor.create();

    MockClient client;

    wl_surface *sa = client.createSurface();
    QTRY_COMPARE(compositor.surfaces.size(), 1);

    wl_surface *sb = client.createSurface();
    QTRY_COMPARE(compositor.surfaces.size(), 2);

    QWaylandClient *ca = compositor.surfaces.at(0)->client();
    QWaylandClient *cb = compositor.surfaces.at(1)->client();

    QCOMPARE(ca, cb);
    QVERIFY(ca != 0);

    QList<QWaylandSurface *> surfaces = compositor.surfacesForClient(ca);
    QCOMPARE(surfaces.size(), 2);
    QVERIFY((surfaces.at(0) == compositor.surfaces.at(0) && surfaces.at(1) == compositor.surfaces.at(1))
            || (surfaces.at(0) == compositor.surfaces.at(1) && surfaces.at(1) == compositor.surfaces.at(0)));

    wl_surface_destroy(sa);
    QTRY_COMPARE(compositor.surfaces.size(), 1);

    wl_surface_destroy(sb);
    QTRY_COMPARE(compositor.surfaces.size(), 0);
}

void tst_WaylandCompositor::multipleClients()
{
    TestCompositor compositor;
    compositor.create();

    MockClient a;
    MockClient b;
    MockClient c;

    wl_surface *sa = a.createSurface();
    wl_surface *sb = b.createSurface();
    wl_surface *sc = c.createSurface();

    QTRY_COMPARE(compositor.surfaces.size(), 3);

    QWaylandClient *ca = compositor.surfaces.at(0)->client();
    QWaylandClient *cb = compositor.surfaces.at(1)->client();
    QWaylandClient *cc = compositor.surfaces.at(2)->client();

    QVERIFY(ca != cb);
    QVERIFY(ca != cc);
    QVERIFY(cb != cc);
    QVERIFY(ca != 0);

    QCOMPARE(compositor.surfacesForClient(ca).size(), 1);
    QCOMPARE(compositor.surfacesForClient(ca).at(0), compositor.surfaces.at(0));

    QCOMPARE(compositor.surfacesForClient(cb).size(), 1);
    QCOMPARE(compositor.surfacesForClient(cb).at(0), compositor.surfaces.at(1));

    QCOMPARE(compositor.surfacesForClient(cc).size(), 1);
    QCOMPARE(compositor.surfacesForClient(cc).at(0), compositor.surfaces.at(2));

    wl_surface_destroy(sa);
    wl_surface_destroy(sb);
    wl_surface_destroy(sc);

    QTRY_COMPARE(compositor.surfaces.size(), 0);
}

void tst_WaylandCompositor::keyboardGrab()
{
    TestCompositor compositor;
    compositor.create();
    MockClient mc;

    mc.createSurface();
    // This is needed for timing purposes, otherwise the query for the
    // compositor surfaces will return null
    QTRY_COMPARE(compositor.surfaces.size(), 1);

    // Set the focused surface so that key event will flow through
    QWaylandSurface *waylandSurface = compositor.surfaces.at(0);
    QWaylandInputDevice* inputDevice = compositor.defaultInputDevice();

    TestKeyboardGrabber* grab = static_cast<TestKeyboardGrabber *>(inputDevice->keyboard());
    QTRY_COMPARE(grab, inputDevice->keyboard());
    QSignalSpy grabFocusSpy(grab, SIGNAL(focusedCalled()));
    QSignalSpy grabKeyPressSpy(grab, SIGNAL(keyPressCalled()));
    QSignalSpy grabKeyReleaseSpy(grab, SIGNAL(keyReleaseCalled()));
    //QSignalSpy grabModifierSpy(grab, SIGNAL(modifiersCalled()));

    inputDevice->setKeyboardFocus(waylandSurface);
    QTRY_COMPARE(grabFocusSpy.count(), 1);

    QKeyEvent ke(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier, 30, 0, 0);
    QKeyEvent ke1(QEvent::KeyRelease, Qt::Key_A, Qt::NoModifier, 30, 0, 0);
    inputDevice->sendFullKeyEvent(&ke);
    inputDevice->sendFullKeyEvent(&ke1);
    QTRY_COMPARE(grabKeyPressSpy.count(), 1);
    QTRY_COMPARE(grabKeyReleaseSpy.count(), 1);

    QKeyEvent ke2(QEvent::KeyPress, Qt::Key_Shift, Qt::NoModifier, 50, 0, 0);
    QKeyEvent ke3(QEvent::KeyRelease, Qt::Key_Shift, Qt::NoModifier, 50, 0, 0);
    inputDevice->sendFullKeyEvent(&ke2);
    inputDevice->sendFullKeyEvent(&ke3);
    //QTRY_COMPARE(grabModifierSpy.count(), 2);
    // Modifiers are also keys
    QTRY_COMPARE(grabKeyPressSpy.count(), 2);
    QTRY_COMPARE(grabKeyReleaseSpy.count(), 2);

    // Stop grabbing
    inputDevice->setKeyboardFocus(Q_NULLPTR);
    inputDevice->sendFullKeyEvent(&ke);
    inputDevice->sendFullKeyEvent(&ke1);
    QTRY_COMPARE(grabKeyPressSpy.count(), 2);
}

void tst_WaylandCompositor::geometry()
{
    TestCompositor compositor;
    compositor.create();

    QRect geometry(0, 0, 4096, 3072);
    compositor.defaultOutput()->setGeometry(geometry);

    MockClient client;

    QTRY_COMPARE(client.geometry, geometry);
}

void tst_WaylandCompositor::mapSurface()
{
    TestCompositor compositor;
    compositor.create();

    MockClient client;

    wl_surface *surface = client.createSurface();
    QTRY_COMPARE(compositor.surfaces.size(), 1);

    QWaylandSurface *waylandSurface = compositor.surfaces.at(0);

    QSignalSpy mappedSpy(waylandSurface, SIGNAL(mappedChanged()));

    QCOMPARE(waylandSurface->size(), QSize());
    QCOMPARE(waylandSurface->isMapped(), false);

    QSize size(256, 256);
    ShmBuffer buffer(size, client.shm);

    // we need to create a shell surface here or the surface won't be mapped
    client.createShellSurface(surface);
    wl_surface_attach(surface, buffer.handle, 0, 0);
    wl_surface_damage(surface, 0, 0, size.width(), size.height());
    wl_surface_commit(surface);

    QTRY_COMPARE(waylandSurface->size(), size);
    QTRY_COMPARE(waylandSurface->isMapped(), true);
    QTRY_COMPARE(mappedSpy.count(), 1);

    wl_surface_destroy(surface);
}

static void frameCallbackFunc(void *data, wl_callback *callback, uint32_t)
{
    ++*static_cast<int *>(data);
    wl_callback_destroy(callback);
}

static void registerFrameCallback(wl_surface *surface, int *counter)
{
    static const wl_callback_listener frameCallbackListener = {
        frameCallbackFunc
    };

    wl_callback_add_listener(wl_surface_frame(surface), &frameCallbackListener, counter);
}

void tst_WaylandCompositor::frameCallback()
{
    class BufferView : public QWaylandView
    {
    public:
        void attach(const QWaylandBufferRef &ref, const QRegion &damage) Q_DECL_OVERRIDE
        {
            Q_UNUSED(damage);
            bufferRef = ref;
        }

        QImage image() const
        {
            if (bufferRef.isNull() || !bufferRef.isShm())
                return QImage();
            return bufferRef.image();
        }

        QWaylandBufferRef bufferRef;
    };

    TestCompositor compositor;
    compositor.create();

    MockClient client;

    wl_surface *surface = client.createSurface();

    int frameCounter = 0;

    QTRY_COMPARE(compositor.surfaces.size(), 1);
    QWaylandSurface *waylandSurface = compositor.surfaces.at(0);
    BufferView* view = new BufferView;
    view->setSurface(waylandSurface);
    view->setOutput(compositor.defaultOutput());

    QSignalSpy damagedSpy(waylandSurface, SIGNAL(damaged(const QRegion &)));

    for (int i = 0; i < 10; ++i) {
        QSize size(i * 8 + 2, i * 8 + 2);
        ShmBuffer buffer(size, client.shm);

        // attach a new buffer every frame, else the damage signal won't be fired
        wl_surface_attach(surface, buffer.handle, 0, 0);
        registerFrameCallback(surface, &frameCounter);
        wl_surface_damage(surface, 0, 0, size.width(), size.height());
        wl_surface_commit(surface);

        QTRY_COMPARE(waylandSurface->isMapped(), true);
        QTRY_COMPARE(damagedSpy.count(), i + 1);

        QCOMPARE(static_cast<BufferView*>(waylandSurface->views().first())->image(), buffer.image);
        compositor.defaultOutput()->frameStarted();
        compositor.defaultOutput()->sendFrameCallbacks();

        QTRY_COMPARE(frameCounter, i + 1);
    }

    wl_surface_destroy(surface);
}

void tst_WaylandCompositor::inputDeviceCapabilities()
{
    TestCompositor compositor;
    compositor.create();

    MockClient client;
    Q_UNUSED(client);

    QWaylandInputDevice dev(&compositor, QWaylandInputDevice::Pointer);

    QTRY_VERIFY(dev.pointer());
    QTRY_VERIFY(!dev.keyboard());
    QTRY_VERIFY(!dev.touch());

    QWaylandInputDevice dev2(&compositor, QWaylandInputDevice::Keyboard | QWaylandInputDevice::Touch);
    QTRY_VERIFY(!dev2.pointer());
    QTRY_VERIFY(dev2.keyboard());
    QTRY_VERIFY(dev2.touch());
}

void tst_WaylandCompositor::inputDeviceCreation()
{
    TestCompositor compositor(true);
    compositor.create();

    MockClient client;
    Q_UNUSED(client);

    TestInputDevice* dev = static_cast<TestInputDevice*>(compositor.defaultInputDevice());

    // The compositor will create the default input device
    QTRY_COMPARE(compositor.defaultInputDevice(), dev);

    QList<QMouseEvent *> allEvents;
    allEvents += dev->createMouseEvents(5);
    foreach (QMouseEvent *me, allEvents) {
        compositor.inputDeviceFor(me);
    }

    // The default input device will get called exatly the number of times it has created
    // the events
    QTRY_COMPARE(dev->queryCount(), 5);
}

void tst_WaylandCompositor::inputDeviceKeyboardFocus()
{
    TestCompositor compositor(true);
    compositor.create();

    // Create client after all the input devices have been set up as the mock client
    // does not dynamically listen to new seats
    MockClient client;
    wl_surface *surface = client.createSurface();
    QTRY_COMPARE(compositor.surfaces.size(), 1);

    QWaylandSurface *waylandSurface = compositor.surfaces.at(0);
    QWaylandInputDevice* dev = compositor.defaultInputDevice();
    dev->setKeyboardFocus(waylandSurface);
    QTRY_COMPARE(compositor.defaultInputDevice()->keyboardFocus(), waylandSurface);

    wl_surface_destroy(surface);
    QTRY_VERIFY(compositor.surfaces.size() == 0);

    QTRY_VERIFY(!compositor.defaultInputDevice()->keyboardFocus());
}

#include <tst_compositor.moc>
QTEST_MAIN(tst_WaylandCompositor);
