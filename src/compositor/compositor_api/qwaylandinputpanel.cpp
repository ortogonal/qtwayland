/****************************************************************************
**
** Copyright (C) 2013 Klarälvdalens Datakonsult AB (KDAB).
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

#include "qwaylandinputpanel.h"

#include <QtWaylandCompositor/QWaylandCompositor>

#include <private/qobject_p.h>

#include "qwlinputpanel_p.h"

QT_BEGIN_NAMESPACE

QWaylandInputPanel::QWaylandInputPanel()
    : QWaylandExtensionTemplate(*new QWaylandInputPanelPrivate())
{
}

QWaylandInputPanel::QWaylandInputPanel(QWaylandCompositor *compositor)
    : QWaylandExtensionTemplate(compositor, *new QWaylandInputPanelPrivate(compositor))
{
}

void QWaylandInputPanel::initialize()
{
    Q_D(QWaylandInputPanel);
    QWaylandExtensionTemplate::initialize();
    QWaylandCompositor* compositor = static_cast<QWaylandCompositor*>(extensionContainer());
    if (!compositor) {
        return;
    }
    d->init(compositor->display(), 1);
}

QWaylandSurface *QWaylandInputPanel::focus() const
{
   Q_D(const QWaylandInputPanel);

    return d->focus();
}

bool QWaylandInputPanel::visible() const
{
    Q_D(const QWaylandInputPanel);

    return d->inputPanelVisible();
}

QRect QWaylandInputPanel::cursorRectangle() const
{
    Q_D(const QWaylandInputPanel);

    return d->cursorRectangle();
}

const struct wl_interface *QWaylandInputPanel::interface()
{
    return QWaylandInputPanelPrivate::interface();
}

QByteArray QWaylandInputPanel::interfaceName()
{
    return QWaylandInputPanelPrivate::interfaceName();
}

QT_END_NAMESPACE
