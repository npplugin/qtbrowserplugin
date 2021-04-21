#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QTextEdit;

class SimpleInputDialog : public QDialog {
public:
  Q_OBJECT

public:
  SimpleInputDialog(QWidget *parent = 0);

private:
  QTextEdit* textEdit;
};

#if 0
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class FindDialog : public QDialog {
public:
  Q_OBJECT

public:
  FindDialog(QWidget *parent = 0);

signals:
  void findNext(const QString &str, Qt::CaseSensitivity cs);
  void findPrevious(const QString &str, Qt::CaseSensitivity cs);

private slots:
  void findClicked();
  void enableFindButton(const QString &text);

private:
  QLabel *label;
  QLineEdit *lineEdit;
  QCheckBox *caseCheckBox;
  QCheckBox *backwardCheckBox;
  QPushButton *findButton;
  QPushButton *closeButton;
};
#endif


#endif // FINDDIALOG_H
