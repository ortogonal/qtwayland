/****************************************************************************
**
** Copyright (C) 2016 Erik Larsson.
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

#include "qwaylandclientextension.h"
#include "qwaylandclientextension_p.h"
#include <QtWaylandClient/private/qwaylanddisplay_p.h>
#include <QtWaylandClient/private/qwaylandintegration_p.h>

QT_BEGIN_NAMESPACE

namespace QtWaylandClient {

QWaylandClientExtensionPrivate::QWaylandClientExtensionPrivate()
    : QObjectPrivate()
    , waylandIntegration(new QWaylandIntegration())
    , version(-1)
{
    QtWaylandClient::QWaylandDisplay *wayland_display = waylandIntegration->display();
    struct ::wl_registry *registry = wl_display_get_registry(wayland_display->wl_display());
    QtWayland::wl_registry::init(registry);
}

void QWaylandClientExtensionPrivate::registry_global(uint32_t id, const QString &interfaceName, uint32_t ver)
{
    Q_Q(QWaylandClientExtension);
    if (interfaceName == QLatin1String(q->extensionInterface()->name)) {
        struct ::wl_registry *registry = static_cast<struct ::wl_registry *>(QtWayland::wl_registry::object());
        q->bind(registry, id, ver);
    }
}

QWaylandClientExtension::QWaylandClientExtension(const int ver)
    : QObject(*new QWaylandClientExtensionPrivate())
{
    Q_D(QWaylandClientExtension);
    d->version = ver;
}

QWaylandIntegration *QWaylandClientExtension::integration() const
{
    Q_D(const QWaylandClientExtension);
    return d->waylandIntegration;
}

int QWaylandClientExtension::version() const
{
    Q_D(const QWaylandClientExtension);
    return d->version;
}

void QWaylandClientExtension::setVersion(const int ver)
{
    Q_D(QWaylandClientExtension);
    if (d->version != ver) {
        d->version = ver;
        emit versionChanged();
    }
}

}

QT_END_NAMESPACE
