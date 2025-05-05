#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qundostack.h"
#include "qundoview.h"
#include <QMainWindow>

#include <QSystemTrayIcon>
#include <QUndoStack>

QT_BEGIN_NAMESPACE

class QStandardItem;

class QStandardItemModel;
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actioncopy_triggered();

    void on_actionpaste_triggered();

    void on_actioncut_triggered();

    void actionundo_triggered();

    void actionredo_triggered();

    void createUndoView();

    void edit();

    void edit_2();

    void edit_3();

    void edit_4();

    void on_actionclear_triggered();

    void on_actionsave_triggered();

    void closeEvent (QCloseEvent *event);

    void message_action();
private:
    Ui::MainWindow *ui;
    QUndoStack *undoStack;
    QUndoView *undoView;
    QString *document;

    bool cleanState;

    bool undoOperation;

public:
    QStandardItemModel* model2;
    QSystemTrayIcon *trayIcon;

    int rowHeight;

    QStandardItem *item1,  *item2,  *item3,  *item4,  *item5,  *item6,  *item7,  *item8,  *item9,  *item10;
    QStandardItem *item11, *item12, *item13, *item14, *item15, *item16, *item17, *item18, *item19, *item20;
    QStandardItem *item21, *item22, *item23, *item24, *item25, *item26, *item27, *item28, *item29, *item30;
    QStandardItem *item31, *item32, *item33, *item34, *item35, *item36, *item37, *item38, *item39, *item40;
    QStandardItem *item41, *item42, *item43, *item44, *item45, *item46, *item47, *item48, *item49, *item50;
    QStandardItem *item51, *item52, *item53, *item54, *item55, *item56, *item57, *item58, *item59, *item60;
    QStandardItem *item61, *item62, *item63, *item64, *item65, *item66, *item67, *item68, *item69, *item70;
    QStandardItem *item71, *item72, *item73, *item74, *item75, *item76, *item77, *item78, *item79, *item80;
    QStandardItem *item81, *item82, *item83, *item84, *item85, *item86, *item87, *item88, *item89, *item90;
    QStandardItem *item91, *item92, *item93, *item94, *item95, *item96, *item97, *item98, *item99, *item100;
    QStandardItem *item101, *item102, *item103, *item104, *item105,*item106, *item107, *item108, *item109, *item110;
    QStandardItem *item111, *item112, *item113, *item114, *item115,*item116, *item117, *item118, *item119, *item120;
    QStandardItem *item121, *item122, *item123, *item124, *item125,*item126, *item127, *item128, *item129, *item130;
    QStandardItem *item131, *item132, *item133, *item134, *item135,*item136, *item137, *item138, *item139, *item140;
    QStandardItem *item141, *item142, *item143, *item144, *item145,*item146, *item147, *item148, *item149, *item150;
    QStandardItem *item151, *item152, *item153, *item154, *item155,*item156, *item157, *item158, *item159, *item160;
    QStandardItem *item161, *item162, *item163, *item164, *item165,*item166, *item167, *item168, *item169, *item170;
    QStandardItem *item171, *item172, *item173, *item174, *item175,*item176, *item177, *item178, *item179, *item180;
};
#endif // MAINWINDOW_H
