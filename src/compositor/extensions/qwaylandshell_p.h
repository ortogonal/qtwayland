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

#ifndef QWAYLANDSHELL_P_H
#define QWAYLANDSHELL_P_H

#include <QtWaylandCompositor/qwaylandexport.h>
#include <QtWaylandCompositor/qwaylandsurface.h>
#include <QtWaylandCompositor/private/qwaylandextension_p.h>
#include <QtWaylandCompositor/QWaylandShellSurface>
#include <QtWaylandCompositor/QWaylandInputDevice>

#include <wayland-server.h>
#include <QHash>
#include <QPoint>
#include <QSet>

#include <QtWaylandCompositor/private/qwayland-server-wayland.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

QT_BEGIN_NAMESPACE

class Q_COMPOSITOR_EXPORT QWaylandShellPrivate
                                        : public QWaylandExtensionTemplatePrivate
                                        , public QtWaylandServer::wl_shell
{
    Q_DECLARE_PUBLIC(QWaylandShell)
public:
    QWaylandShellPrivate();
    static QWaylandShellPrivate *get(QWaylandShell *shell) { return shell->d_func(); }

protected:
    void shell_get_shell_surface(Resource *resource, uint32_t id, struct ::wl_resource *surface) Q_DECL_OVERRIDE;
};

class Q_COMPOSITOR_EXPORT QWaylandShellSurfacePrivate
                                        : public QWaylandExtensionTemplatePrivate
                                        , public QtWaylandServer::wl_shell_surface
{
    Q_DECLARE_PUBLIC(QWaylandShellSurface)
public:
    QWaylandShellSurfacePrivate();
    ~QWaylandShellSurfacePrivate();

    static QWaylandShellSurfacePrivate *get(QWaylandShellSurface *surface) { return surface->d_func(); }

    void ping();
    void ping(uint32_t serial);

    void setFocusPolicy(QWaylandShellSurface::FocusPolicy focusPolicy)
    {
        if (focusPolicy == m_focusPolicy)
            return;
        Q_Q(QWaylandShellSurface);
        m_focusPolicy = focusPolicy;
        emit q->focusPolicyChanged();
    }
private:
    QWaylandShell *m_shell;
    QWaylandSurface *m_surface;

    QSet<uint32_t> m_pings;

    QString m_title;
    QString m_className;
    QWaylandShellSurface::FocusPolicy m_focusPolicy;

    void shell_surface_destroy_resource(Resource *resource) Q_DECL_OVERRIDE;

    void shell_surface_move(Resource *resource,
                            struct wl_resource *input_device_super,
                            uint32_t time) Q_DECL_OVERRIDE;
    void shell_surface_resize(Resource *resource,
                              struct wl_resource *input_device,
                              uint32_t time,
                              uint32_t edges) Q_DECL_OVERRIDE;
    void shell_surface_set_toplevel(Resource *resource) Q_DECL_OVERRIDE;
    void shell_surface_set_transient(Resource *resource,
                                     struct wl_resource *parent_surface_resource,
                                     int x,
                                     int y,
                                     uint32_t flags) Q_DECL_OVERRIDE;
    void shell_surface_set_fullscreen(Resource *resource,
                                      uint32_t method,
                                      uint32_t framerate,
                                      struct wl_resource *output_resource) Q_DECL_OVERRIDE;
    void shell_surface_set_popup(Resource *resource,
                                 struct wl_resource *input_device,
                                 uint32_t time,
                                 struct wl_resource *parent,
                                 int32_t x,
                                 int32_t y,
                                 uint32_t flags) Q_DECL_OVERRIDE;
    void shell_surface_set_maximized(Resource *resource,
                                     struct wl_resource *output_resource) Q_DECL_OVERRIDE;
    void shell_surface_pong(Resource *resource,
                            uint32_t serial) Q_DECL_OVERRIDE;
    void shell_surface_set_title(Resource *resource,
                                 const QString &title) Q_DECL_OVERRIDE;
    void shell_surface_set_class(Resource *resource,
                                 const QString &class_) Q_DECL_OVERRIDE;

};

QT_END_NAMESPACE

#endif // QWAYLANDSHELL_P_H
