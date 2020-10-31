#ifndef WIDGET_H
#define WIDGET_H

#include <qdialog.h>
#include <qlist.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include "abxtester.h"
#include "trackselectdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    ABXTester *tester;

private slots:
    void updateTrialCount();
};
#endif // WIDGET_H
