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

#include "qwaylandquickshellsurfaceitem.h"
#include "qwaylandquickshellsurfaceitem_p.h"

#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandInputDevice>

QT_BEGIN_NAMESPACE

/*!
 * \qmltype ShellSurfaceItem
 * \inqmlmodule QtWayland.Compositor
 * \brief An item representing a ShellSurface.
 *
 * This type can be used to render shell surfaces as part of a Qt Quick scene.
 * It handles moving and resizing triggered by clicking on the window decorations.
 */

/*!
 * \class QWaylandQuickShellSurfaceItem
 * \inmodule QtWaylandCompositor
 * \brief A Qt Quick item for QWaylandShellSurface.
 *
 * This class can be used to create Qt Quick items representing shell surfaces.
 * It handles moving and resizing triggered by clicking on the window decorations.
 *
 * \sa QWaylandQuickItem
 */

/*!
 * Constructs a QWaylandQuickShellSurfaceItem with the given \a parent.
 */
QWaylandQuickShellSurfaceItem::QWaylandQuickShellSurfaceItem(QQuickItem *parent)
    : QWaylandQuickItem(*new QWaylandQuickShellSurfaceItemPrivate(), parent)
{
}

/*!
 * \internal
 */
QWaylandQuickShellSurfaceItem::QWaylandQuickShellSurfaceItem(QWaylandQuickShellSurfaceItemPrivate &dd, QQuickItem *parent)
    : QWaylandQuickItem(dd, parent)
{
}

QWaylandQuickShellSurfaceItem::~QWaylandQuickShellSurfaceItem() {
    Q_D(QWaylandQuickShellSurfaceItem);
    if (d->shellSurface) {
        d->shellSurface->destroy();
    }
}

/*!
 * \qmlproperty object QtWaylandCompositor::ShellSurfaceItem::shellSurface
 *
 * This property holds the shell surface rendered by this ShellSurfaceItem.
 */

/*!
 * \property QWaylandQuickShellSurfaceItem::shellSurface
 *
 * This property holds the shell surface rendered by this QWaylandQuickShellSurfaceItem.
 */
QWaylandShellSurface *QWaylandQuickShellSurfaceItem::shellSurface() const
{
    Q_D(const QWaylandQuickShellSurfaceItem);
    return d->shellSurface;
}

void QWaylandQuickShellSurfaceItem::setShellSurface(QWaylandShellSurface *shellSurface)
{
    Q_D(QWaylandQuickShellSurfaceItem);
    if (shellSurface == d->shellSurface)
        return;

    if (d->shellSurface) {
        disconnect(d->shellSurface, &QWaylandShellSurface::startMove, this, &QWaylandQuickShellSurfaceItem::handleStartMove);
        disconnect(d->shellSurface, &QWaylandShellSurface::startResize, this, &QWaylandQuickShellSurfaceItem::handleStartResize);
        disconnect(d->shellSurface, &QWaylandShellSurface::setPopup, this, &QWaylandQuickShellSurfaceItem::handleSetPopup);
        disconnect(d->shellSurface, &QWaylandShellSurface::destroyed, this, &QWaylandQuickShellSurfaceItem::handleShellSurfaceDestroyed);
    }
    d->shellSurface = shellSurface;
    if (d->shellSurface) {
        connect(d->shellSurface, &QWaylandShellSurface::startMove, this, &QWaylandQuickShellSurfaceItem::handleStartMove);
        connect(d->shellSurface, &QWaylandShellSurface::startResize, this, &QWaylandQuickShellSurfaceItem::handleStartResize);
        connect(d->shellSurface, &QWaylandShellSurface::setPopup, this, &QWaylandQuickShellSurfaceItem::handleSetPopup);
        connect(d->shellSurface, &QWaylandShellSurface::destroyed, this, &QWaylandQuickShellSurfaceItem::handleShellSurfaceDestroyed);
    }
    emit shellSurfaceChanged();
}

/*!
 * \internal
 * \property QWaylandQuickShellSurfaceItem::moveItem
 *
 * This property holds the move item for this QWaylandQuickShellSurfaceItem.
 */
QQuickItem *QWaylandQuickShellSurfaceItem::moveItem() const
{
    Q_D(const QWaylandQuickShellSurfaceItem);
    return d->moveItem;
}

void QWaylandQuickShellSurfaceItem::setMoveItem(QQuickItem *moveItem)
{
    Q_D(QWaylandQuickShellSurfaceItem);
    if (d->moveItem == moveItem)
        return;
    d->moveItem = moveItem;
    moveItemChanged();
}

/*!
 * \internal
 */
void QWaylandQuickShellSurfaceItem::handleStartMove(QWaylandInputDevice *inputDevice)
{
    Q_D(QWaylandQuickShellSurfaceItem);
    d->grabberState = QWaylandQuickShellSurfaceItemPrivate::MoveState;
    d->moveState.inputDevice = inputDevice;
    d->moveState.initialized = false;
}

/*!
 * \internal
 */
void QWaylandQuickShellSurfaceItem::handleStartResize(QWaylandInputDevice *inputDevice, QWaylandShellSurface::ResizeEdge edges)
{
    Q_D(QWaylandQuickShellSurfaceItem);
    d->grabberState = QWaylandQuickShellSurfaceItemPrivate::ResizeState;
    d->resizeState.inputDevice = inputDevice;
    d->resizeState.resizeEdges = edges;
    d->resizeState.initialSize = surface()->size();
    d->resizeState.initialized = false;
}

/*!
 * \internal
 */
void QWaylandQuickShellSurfaceItem::handleSetPopup(QWaylandInputDevice *inputDevice, QWaylandSurface *parent, const QPoint &relativeToParent)
{
    Q_UNUSED(inputDevice);
    Q_D(QWaylandQuickShellSurfaceItem);

    QWaylandQuickShellSurfaceItem* parentItem = qobject_cast<QWaylandQuickShellSurfaceItem*>(parent->views().first()->renderObject());
    if (parentItem) {
        // Clear all the transforms for this ShellSurfaceItem. They are not
        // applicable when the item becomes a child to a surface that has its
        // own transforms. Otherwise the transforms would be applied twice.
        QQmlListProperty<QQuickTransform> t = this->transform();
        t.clear(&t);
        this->setRotation(0);
        this->setScale(1.0);
        this->setX(relativeToParent.x());
        this->setY(relativeToParent.y());
        this->setParentItem(parentItem);

        if (Q_LIKELY(this->view()) && Q_LIKELY(this->view()->output()) &&
            Q_LIKELY(this->view()->output()->window()))
        {
            QWindow* wnd = this->view()->output()->window();
            wnd->installEventFilter(parentItem);
        }
    }

    if (!d->popupshellSurfaces.contains(this->shellSurface()))
        d->popupshellSurfaces.append(this->shellSurface());
}

/*!
 * \internal
 */
void QWaylandQuickShellSurfaceItem::handleShellSurfaceDestroyed() {
    Q_D(QWaylandQuickShellSurfaceItem);
    d->shellSurface = NULL;

    if (Q_LIKELY(this->view()) && Q_LIKELY(this->view()->output()) &&
        Q_LIKELY(this->view()->output()->window()))
    {
        QWindow* wnd = this->view()->output()->window();
        wnd->removeEventFilter(this->parentItem());
    }

}

/*!
 * \internal
 */
void QWaylandQuickShellSurfaceItem::adjustOffsetForNextFrame(const QPointF &offset)
{
    Q_D(QWaylandQuickShellSurfaceItem);
    QQuickItem *moveItem = d->moveItem ? d->moveItem : this;
    moveItem->setPosition(moveItem->position() + offset);
}

/*!
 * \internal
 */
void QWaylandQuickShellSurfaceItem::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QWaylandQuickShellSurfaceItem);
    if (d->grabberState == QWaylandQuickShellSurfaceItemPrivate::ResizeState) {
        Q_ASSERT(d->resizeState.inputDevice == compositor()->inputDeviceFor(event));
        if (!d->resizeState.initialized) {
            d->resizeState.initialMousePos = event->windowPos();
            d->resizeState.initialized = true;
            return;
        }
        QPointF delta = event->windowPos() - d->resizeState.initialMousePos;
        QSize newSize = shellSurface()->sizeForResize(d->resizeState.initialSize, delta, d->resizeState.resizeEdges);
        shellSurface()->sendConfigure(newSize, d->resizeState.resizeEdges);
    } else if (d->grabberState == QWaylandQuickShellSurfaceItemPrivate::MoveState) {
        Q_ASSERT(d->moveState.inputDevice == compositor()->inputDeviceFor(event));
        QQuickItem *moveItem = d->moveItem ? d->moveItem : this;
        if (!d->moveState.initialized) {
            d->moveState.initialOffset = moveItem->mapFromItem(Q_NULLPTR, event->windowPos());
            d->moveState.initialized = true;
            return;
        }
        if (!moveItem->parentItem())
            return;
        QPointF parentPos = moveItem->parentItem()->mapFromItem(Q_NULLPTR, event->windowPos());
        moveItem->setPosition(parentPos - d->moveState.initialOffset);
    } else {
        QWaylandQuickItem::mouseMoveEvent(event);
    }
}

/*!
 * \internal
 */
void QWaylandQuickShellSurfaceItem::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QWaylandQuickShellSurfaceItem);
    if (d->grabberState != QWaylandQuickShellSurfaceItemPrivate::DefaultState) {
        d->grabberState = QWaylandQuickShellSurfaceItemPrivate::DefaultState;
        return;
    }
    QWaylandQuickItem::mouseReleaseEvent(event);
}

/*!
 * \internal
 */
void QWaylandQuickShellSurfaceItem::surfaceChangedEvent(QWaylandSurface *newSurface, QWaylandSurface *oldSurface)
{
    if (oldSurface)
        disconnect(oldSurface, &QWaylandSurface::offsetForNextFrame, this, &QWaylandQuickShellSurfaceItem::adjustOffsetForNextFrame);

    if (newSurface)
        connect(newSurface, &QWaylandSurface::offsetForNextFrame, this, &QWaylandQuickShellSurfaceItem::adjustOffsetForNextFrame);
}

/*!
 * \internal
 */
void QWaylandQuickShellSurfaceItem::componentComplete()
{
    Q_D(QWaylandQuickShellSurfaceItem);
    if (!d->shellSurface)
        setShellSurface(new QWaylandShellSurface());

    QWaylandQuickItem::componentComplete();
}

QList<QWaylandShellSurface*> QWaylandQuickShellSurfaceItemPrivate::popupshellSurfaces;

/*!
 * \internal
 */
void QWaylandQuickShellSurfaceItemPrivate::closePopups()
{
    if (!popupshellSurfaces.isEmpty()) {
        Q_FOREACH (QWaylandShellSurface* shellSurface, popupshellSurfaces) {
            shellSurface->sendPopupDone();
        }
        popupshellSurfaces.clear();
    }
}

bool QWaylandQuickShellSurfaceItem::eventFilter(QObject *obj, QEvent *event)
{
    Q_D(QWaylandQuickShellSurfaceItem);
    if (event->type() == QEvent::MouseButtonPress && !d->popupsClosed) {
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        QPoint pnt = me->pos() - this->position().toPoint();

        if (this->childAt(pnt.x(), pnt.y())) {
            // If there is a child at this coordinate it means that we can
            // remove the event filter and let the normal event handler handle
            // this event.
            obj->removeEventFilter(this);
            return false;
        } else {
            // If there is no child at this coordina it means that the user
            // clicked outside all the active popup surfaces. The popups should
            // be closed, but the event filter will stay to chatch the release-
            // event before removing it self.
            d->popupsClosed = true;
            d->closePopups();
            return true;
        }
    } else if (event->type() == QEvent::MouseButtonRelease && d->popupsClosed) {
        d->popupsClosed = false;
        obj->removeEventFilter(this);
        return true;
    }
    return QObject::eventFilter(obj, event);
}

QT_END_NAMESPACE
