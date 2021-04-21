/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Solutions component.
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
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
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

#include "finddialog.h"
#include <QPushButton>
#include <QtGui>
#include <qx11info_x11.h>

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

#include <qtbrowserplugin.h>

class Trivial : public QWidget {
  Q_OBJECT
  Q_PROPERTY(QString text READ text WRITE setText)

  Q_CLASSINFO("ClassID", "{5a22176d-118f-4185-9653-9f98958a6df8}")
  Q_CLASSINFO("InterfaceID", "{2df735ba-da4f-4fb7-8f35-b8dfbf8cfd9a}")
  Q_CLASSINFO("EventsID", "{449de213-f8bd-4d2e-a2cf-eab407c03245}")

  Q_CLASSINFO("MIME", "trivial/very:xxx:Trivial and useless")
  Q_CLASSINFO("ToSuperClass", "Trivial")
  Q_CLASSINFO("DefaultProperty", "text")

public:
  Trivial(QWidget *parent = 0)
      : QWidget(parent), m_input(NULL), m_button(NULL), m_dialog(NULL) {
    MY_LOG("");
    m_text = QString::fromLatin1("hello qt");

    m_input = new QTextEdit("hello qt", this);
    m_input->setGeometry(10, 10, 150, 100);

    m_button = new QPushButton("test", this);
    m_button->setGeometry(QRect(QPoint(50, 170), QSize(100, 20)));
    connect(m_button, SIGNAL(clicked()), this, SLOT(handleButton()));
  }

  void mousePressEvent(QMouseEvent *) { mouseDown(); }
  void mouseMoveEvent(QMouseEvent *e) { mouseMove(e->x(), e->y()); }
  void mouseReleaseEvent(QMouseEvent *) { mouseUp(); }
  QString text() const { return m_text; }

#ifdef DIRTY_TRICK_X11FOCUS
  bool event(QEvent *evt) {
    if (evt->type() == QEvent::WindowActivate) {
      MY_LOG("%d, WindowActivate", evt->type());
      trickX11Focus();
      // QApplication::postEvent(this, new QEvent(MY_TRICK_FOCUS_EVENT));
    } else if (evt->type() == QEvent::WindowBlocked) {
      MY_LOG("%d, WindowBlocked", evt->type());
      //            QWidget *w =
      //            QApplicationPrivate::active_window->focusWidget();
      //            this->setFocus(Qt::MouseFocusReason);

    } else if (evt->type() == MY_TRICK_FOCUS_EVENT) {
      trickX11Focus();
    } else if (evt->type() == QEvent::MouseButtonPress) {
      MY_LOG("%d, MouseButtonPress", evt->type());
      this->setFocus(Qt::MouseFocusReason);
    } else {
      MY_LOG("%d", evt->type());
    }

    return QWidget::event(evt);
  }
#endif

public slots:
  void about() { QMessageBox::aboutQt(this); }
  void setText(const QString &text) {
    m_text = text;
    update();
  }
  void handleButton() {
    MY_LOG("");

//    if (QMessageBox::Yes == QMessageBox::question(this,
//                                                  tr("Question"),
//                                                  tr("Are you OK?"),
//                                                  QMessageBox::Yes | QMessageBox::No,
//                                                  QMessageBox::Yes)) {
//        QMessageBox::information(this, tr("Yes"), tr("Yes"));
//    } else {
//        QMessageBox::information(this, tr("No"), tr("No"));
//    }

    if (!m_dialog) {
        m_dialog = new SimpleInputDialog(this);
        m_dialog->setModal(true);
    }
    m_dialog->show();
  }

signals:
  void mouseDown();
  void mouseMove(int x, int y);
  void mouseUp();

protected:
  //    void paintEvent(QPaintEvent*)
  //    {
  //        QPainter p(this);
  //        QRect r(rect());
  //        r.adjust(0, 0, -1, -1);

  //        p.drawRect(r);
  //        p.drawText(r, Qt::AlignBottom | Qt::AlignHCenter, m_text);
  //    }

private:
#ifdef DIRTY_TRICK_X11FOCUS
  void trickX11Focus() {
    MY_LOG("trickX11Focus");

    //        Display * display = x11Info().diaplay();
    Display *display = QX11Info::display();
    XSelectInput(display, this->winId(),
                 KeyPressMask | KeyReleaseMask | ButtonPressMask |
                     ButtonReleaseMask | KeymapStateMask | ButtonMotionMask |
                     PointerMotionHintMask | EnterWindowMask | LeaveWindowMask |
                     FocusChangeMask | ExposureMask | StructureNotifyMask |
                     SubstructureNotifyMask | PropertyChangeMask);
    XFlush(display);

    QApplication::setActiveWindow(this);

    QTimer timer(this);
    timer.setSingleShot(true);
    QDialog dialog(this);
    dialog.setWindowFlags(dialog.windowFlags() |
                          Qt::X11BypassWindowManagerHint);
    dialog.setGeometry(-65536, -65536, 5, 5);
    connect(&timer, SIGNAL(timeout()), &dialog, SLOT(close()));
    timer.start(0);
    dialog.exec();
  }
#endif

private:
  QString m_text;
  QTextEdit *m_input;
  QPushButton *m_button;
  QDialog *m_dialog;
};

#include "trivial.moc"

QTNPFACTORY_BEGIN("Trivial",
                  "A Qt-based LiveConnected plug-in that does nothing")
QTNPCLASS(Trivial)
QTNPFACTORY_END()

#ifdef QAXSERVER
#include <ActiveQt/QAxFactory>
QAXFACTORY_BEGIN("{aa3216bf-7e20-482c-84c6-06167bacb616}",
                 "{08538ca5-eb7a-4f24-a3c4-a120c6e04dc4}")
QAXCLASS(Trivial)
QAXFACTORY_END()
#endif
