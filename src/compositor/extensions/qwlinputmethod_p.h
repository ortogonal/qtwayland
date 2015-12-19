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

#ifndef QTWAYLAND_QWLINPUTMETHOD_H
#define QTWAYLAND_QWLINPUTMETHOD_H

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

#include <QtWaylandCompositor/private/qwayland-server-input-method.h>
#include <QtWaylandCompositor/QWaylandExtension>
#include <QtWaylandCompositor/QWaylandInputDevice>

#include <QObject>
#include <QScopedPointer>

#include <QtWaylandCompositor/QWaylandSurface>

QT_BEGIN_NAMESPACE

class QWaylandInputDevice;
class QWaylandCompositor;

namespace QtWayland {

class InputMethodContext;
class TextInput;

class Q_COMPOSITOR_EXPORT InputMethod : public QWaylandExtensionTemplate<InputMethod> , public QtWaylandServer::wl_input_method
{
    Q_OBJECT
public:
    explicit InputMethod(QWaylandInputDevice *seat);
    ~InputMethod();

    QWaylandCompositor *compositor() const { return m_seat->compositor(); }

    void activate(TextInput *textInput);
    void deactivate();

    bool isBound() const;

    InputMethodContext *context() const;
    TextInput *textInput() const;

protected:
    void input_method_bind_resource(Resource *resource);
    void input_method_destroy_resource(Resource *resource);

private Q_SLOTS:
    void focusChanged(QWaylandSurface *surface);

private:
    QWaylandInputDevice *m_seat;
    Resource *m_resource;
    TextInput *m_textInput;
    InputMethodContext *m_context;
};

} // namespace QtWayland

QT_END_NAMESPACE

#endif // QTWAYLAND_QWLINPUTMETHOD_H
