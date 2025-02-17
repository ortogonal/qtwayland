/****************************************************************************
**
** Copyright (C) 2016 LG Electronics, Inc., author: <mikko.levonmaa@lge.com>
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

#include "testinputdevice.h"

#include <QMouseEvent>

TestInputDevice::TestInputDevice(QWaylandCompositor *compositor, QWaylandInputDevice::CapabilityFlags caps)
    : QWaylandInputDevice(compositor, caps)
{
    m_queryCount = 0;
}

TestInputDevice::~TestInputDevice()
{
}

bool TestInputDevice::isOwner(QInputEvent *event)
{
    m_queryCount++;
    QMouseEvent *me = dynamic_cast<QMouseEvent *>(event);
    return m_events.contains(me);
}

QList<QMouseEvent *> TestInputDevice::createMouseEvents(int count)
{
    for (int i = 0; i < count; i++) {
        m_events.append(new QMouseEvent(QEvent::MouseMove, QPointF(10 + i, 10 + i), Qt::NoButton, Qt::NoButton, Qt::NoModifier));
    }
    return m_events;
}

