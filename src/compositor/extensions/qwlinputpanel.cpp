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

#include "qwlinputpanel_p.h"

#include <QtWaylandCompositor/qwaylandinputpanel.h>
#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/qwaylandexport.h>

#include <QtWaylandCompositor/QWaylandSurface>
#include <QtWaylandCompositor>

#include "qwlinputmethod_p.h"
#include "qwlinputpanelsurface_p.h"
#include "qwltextinput_p.h"
#include "compositor_api/qwaylandclient.h"
#include "compositor_api/qwaylandsurface.h"
#include <QDebug>
QT_BEGIN_NAMESPACE

QWaylandInputPanelPrivate::QWaylandInputPanelPrivate()
    : QWaylandExtensionTemplatePrivate()
    , QtWaylandServer::wl_input_panel()
    , m_compositor(NULL)
    , m_focus()
    , m_inputPanelVisible(false)
    , m_cursorRectangle()
{
}

QWaylandInputPanelPrivate::QWaylandInputPanelPrivate(QWaylandCompositor *compositor)
    : QWaylandExtensionTemplatePrivate()
    , QtWaylandServer::wl_input_panel()
    , m_compositor(compositor)
    , m_focus()
    , m_inputPanelVisible(false)
    , m_cursorRectangle()
{
    init(compositor->display(), 1);
}

QWaylandInputPanelPrivate::~QWaylandInputPanelPrivate()
{
}

QWaylandInputPanel *QWaylandInputPanelPrivate::waylandInputPanel() const
{
    QWaylandInputPanel *panel = const_cast<QWaylandInputPanel *>(q_func());
    return panel;
}

QWaylandSurface *QWaylandInputPanelPrivate::focus() const
{
    return m_focus;
}

void QWaylandInputPanelPrivate::setFocus(QWaylandSurface *focus)
{
    Q_Q(QWaylandInputPanel);
    if (m_focus == focus)
        return;

    m_focus = focus;

    Q_EMIT q->focusChanged();
}

bool QWaylandInputPanelPrivate::inputPanelVisible() const
{
    return m_inputPanelVisible;
}

void QWaylandInputPanelPrivate::setInputPanelVisible(bool inputPanelVisible)
{
    Q_Q(QWaylandInputPanel);
    if (m_inputPanelVisible == inputPanelVisible)
        return;

    m_inputPanelVisible = inputPanelVisible;

    q->visibleChanged();
}

QRect QWaylandInputPanelPrivate::cursorRectangle() const
{
    return m_cursorRectangle;
}

void QWaylandInputPanelPrivate::setCursorRectangle(const QRect &cursorRectangle)
{
    Q_Q(QWaylandInputPanel);
    if (m_cursorRectangle == cursorRectangle)
        return;

    m_cursorRectangle = cursorRectangle;

    Q_EMIT q->cursorRectangleChanged();
}

QWaylandInputPanelPrivate *QWaylandInputPanelPrivate::findIn(QWaylandObject *container)
{
    QWaylandInputPanel *panel = QWaylandInputPanel::findIn(container);
    if (!panel)
        return Q_NULLPTR;
    return panel->d_func();
}

void QWaylandInputPanelPrivate::input_panel_get_input_panel_surface(Resource *resource, uint32_t id, wl_resource *surface_res)
{
    qDebug() << Q_FUNC_INFO;

    Q_Q(QWaylandInputPanel);
    QWaylandSurface* surface = QWaylandSurface::fromResource(surface_res);
    new QtWayland::InputPanelSurface(resource->client(), id, surface);
    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(q->extensionContainer());
    emit q_func()->createShellSurface(surface, QWaylandClient::fromWlClient(compositor, resource->client()), id);

}

QT_END_NAMESPACE
