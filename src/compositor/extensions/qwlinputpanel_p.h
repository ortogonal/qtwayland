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

#ifndef QTWAYLAND_QWLINPUTPANEL_P_H
#define QTWAYLAND_QWLINPUTPANEL_P_H

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

#include <QtWaylandCompositor/qwaylandexport.h>
#include <QtWaylandCompositor/qwaylandinputpanel.h>
#include <QtWaylandCompositor/QWaylandSurface>
#include <QtWaylandCompositor/QWaylandInputPanel>

#include <QtWaylandCompositor/qwaylandclient.h>
#include <QtWaylandCompositor/private/qwaylandextension_p.h>
#include <QtWaylandCompositor/private/qwayland-server-input-method.h>

#include <QRect>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE

namespace QtWayland {
class TextInput;
}


class Q_COMPOSITOR_EXPORT QWaylandInputPanelPrivate : public QWaylandExtensionTemplatePrivate, public QtWaylandServer::wl_input_panel
{
    Q_DECLARE_PUBLIC(QWaylandInputPanel)
public:
    QWaylandInputPanelPrivate();
    QWaylandInputPanelPrivate(QWaylandCompositor *compositor);
    ~QWaylandInputPanelPrivate();

    QWaylandInputPanel *waylandInputPanel() const;

    QWaylandSurface *focus() const;
    void setFocus(QWaylandSurface *focus);

    bool inputPanelVisible() const;
    void setInputPanelVisible(bool inputPanelVisible);

    QRect cursorRectangle() const;
    void setCursorRectangle(const QRect &cursorRectangle);

    static QWaylandInputPanelPrivate *findIn(QWaylandObject *container);
protected:
    void input_panel_get_input_panel_surface(Resource *resource, uint32_t id, struct ::wl_resource *surface) Q_DECL_OVERRIDE;

private:
    QWaylandCompositor *m_compositor;

    QWaylandSurface *m_focus;
    bool m_inputPanelVisible;
    QRect m_cursorRectangle;
};

QT_END_NAMESPACE

#endif // QTWAYLAND_QWLINPUTPANEL_P_H
