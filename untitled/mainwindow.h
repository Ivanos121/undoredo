#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qundostack.h"
#include "qundoview.h"
#include <QMainWindow>

#include <QUndoStack>

QT_BEGIN_NAMESPACE
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

private:
    Ui::MainWindow *ui;
    QUndoStack *undoStack;
    QUndoView *undoView;
    QString *document;

    bool cleanState;

    bool undoOperation;
};
#endif // MAINWINDOW_H
