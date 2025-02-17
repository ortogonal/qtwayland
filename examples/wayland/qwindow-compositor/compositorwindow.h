/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Wayland module
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef COMPOSITORWINDOW_H
#define COMPOSITORWINDOW_H

#include <QOpenGLWindow>
#include <QPointer>
#include <QtGui/private/qopengltextureblitter_p.h>

QT_BEGIN_NAMESPACE

class WindowCompositor;
class WindowCompositorView;
class QOpenGLTexture;

class CompositorWindow : public QOpenGLWindow
{
public:
    CompositorWindow();

    void setCompositor(WindowCompositor *comp);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

private slots:
    void startMove();
    void startResize(int edge);
    void startDrag(WindowCompositorView *dragIcon);
    void setFrameOffset(const QPoint &offset);

private:
    enum GrabState { NoGrab, MoveGrab, ResizeGrab, DragGrab };

    WindowCompositorView *viewAt(const QPointF &point);
    bool mouseGrab() const { return m_grabState != NoGrab ;}
    void drawBackground();
    void sendMouseEvent(QMouseEvent *e, WindowCompositorView *target);

    QOpenGLTextureBlitter m_textureBlitter;
    QSize m_backgroundImageSize;
    QOpenGLTexture *m_backgroundTexture;
    WindowCompositor *m_compositor;
    QPointer<WindowCompositorView> m_mouseView;
    GrabState m_grabState;
    QSize m_initialSize;
    int m_resizeEdge;
    QPointF m_mouseOffset;
    QPointF m_initialMousePos;
    WindowCompositorView *m_dragIconView;
};

QT_END_NAMESPACE

#endif // COMPOSITORWINDOW_H
