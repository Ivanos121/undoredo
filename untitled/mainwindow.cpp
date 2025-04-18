#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include "QDebug"
#include "QShortcut"
#include "QKeyEvent"
#include "QLineEdit"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , undoOperation(false)
    , cleanState(true)
{
    ui->setupUi(this);

    undoStack = new QUndoStack(this);
    undoStack->setUndoLimit(100);

    ui->actionundo->setEnabled(false);
    ui->actionredo->setEnabled(false);

    createUndoView();

    ui->tableWidget->setRowCount(25);
    ui->tableWidget->setColumnCount(4);
    QStringList name_2;
    name_2 << "Величина" << "Обозначение" << "Значение" << "Размерность";
    ui->tableWidget->setHorizontalHeaderLabels(name_2);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(true);
    //ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
   // ui->tableWidget->setSelectionBehavior(QAbstractItemView :: SelectRows);
    //ui->tableWidget->setSelectionMode(QAbstractItemView :: SingleSelection);

    for(int row = 0; row<ui->tableWidget->rowCount(); row++)
    {
        for(int column = 0; column<ui->tableWidget->columnCount(); column++)
        {
            ui->tableWidget->setItem(row, column, new QTableWidgetItem());
        }
    }
    connect(ui->lineEdit,&QLineEdit::textEdited, this, &MainWindow::edit);
    connect(ui->lineEdit_2,&QLineEdit::textEdited, this, &MainWindow::edit_2);
    connect(ui->lineEdit_3,&QLineEdit::textEdited, this, &MainWindow::edit_3);
    connect(ui->tableWidget,&QTableWidget::itemChanged, this, &MainWindow::edit_4);
    connect(ui->actionundo, &QAction::triggered, this, &MainWindow::actionundo_triggered);
    connect(ui->actionredo, &QAction::triggered, this, &MainWindow::actionredo_triggered);
    //connect(ui->actionredo, &QAction::triggered, this, &MainWindow::actionredo_triggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actioncopy_triggered()
{
    if(ui->lineEdit->text().isEmpty())
    {
        return;
    }
    else
    {
        if(ui->lineEdit->hasFocus())
        {
            QUndoCommand *copyCommand = new QUndoCommand();
            QClipboard *clipboard = QApplication::clipboard();
            QString s1 = ui->lineEdit->text();
            clipboard->setText(s1);
            copyCommand->setText("lineEdit");
            ui->lineEdit->paste();
            undoStack->push(copyCommand);
        }
    }
    if(ui->lineEdit_2->text().isEmpty())
    {
    return;
    }
    else
    {
        if(ui->lineEdit_2->hasFocus())
        {
            QUndoCommand *copyCommand = new QUndoCommand();
            QClipboard *clipboard = QApplication::clipboard();
            QString s1 = ui->lineEdit_2->text();
            clipboard->setText(s1);
            copyCommand->setText("lineEdit_2");
            ui->lineEdit_2->paste();
            undoStack->push(copyCommand);
        }
    }
    if(ui->lineEdit_3->text().isEmpty())
    {
    return;
    }
    else
    {
        if(ui->lineEdit_3->hasFocus())
        {
            QUndoCommand *copyCommand = new QUndoCommand();
            QClipboard *clipboard = QApplication::clipboard();
            QString s1 = ui->lineEdit_3->text();
            clipboard->setText(s1);
            copyCommand->setText("lineEdit_3");
            ui->lineEdit_3->paste();
            undoStack->push(copyCommand);
        }
    }
    if(ui->tableWidget->hasFocus())
    {
        ui->tableWidget->copy();
    }
}


void MainWindow::on_actionpaste_triggered()
{
        if(ui->lineEdit->hasFocus())
        {
            QUndoCommand *addCommand = new QUndoCommand();
            addCommand->setText("lineEdit");
            ui->lineEdit->paste();
            undoStack->push(addCommand);

        }
   // }
    if(ui->lineEdit->text().isEmpty())
    {
    return;
    }
    else
    {
        if(ui->lineEdit_2->hasFocus())
        {
            QUndoCommand *addCommand = new QUndoCommand();
            addCommand->setText("lineEdit_2");
            ui->lineEdit_2->paste();
            undoStack->push(addCommand);
        }
    }
    if(ui->lineEdit->text().isEmpty())
    {
    return;
    }
    else
    {
        if(ui->lineEdit_3->hasFocus())
        {
            QUndoCommand *addCommand = new QUndoCommand();
            addCommand->setText("lineEdit_3");
            ui->lineEdit_3->paste();
            undoStack->push(addCommand);
        }
    }
    if(ui->tableWidget->hasFocus())
    {
        ui->tableWidget->paste();
    }
}


void MainWindow::on_actioncut_triggered()
{
    if(ui->lineEdit->text().isEmpty())
    {
        return;
    }
    else
    {
        if(ui->lineEdit->hasFocus())
        {
            QUndoCommand *cutCommand = new QUndoCommand();
            cutCommand->setText("lineEdit");
            ui->lineEdit->cut();
            undoStack->push(cutCommand);

        }
    }
    if(ui->lineEdit_2->text().isEmpty())
    {
        return;
    }
    else
    {
        if(ui->lineEdit_2->hasFocus())
        {
            QUndoCommand *cutCommand = new QUndoCommand();
            cutCommand->setText("lineEdit_2");
            ui->lineEdit_2->cut();
            undoStack->push(cutCommand);
        }
    }
    if(ui->lineEdit_3->text().isEmpty())
    {
        return;
    }
    else
    {
        if(ui->lineEdit_3->hasFocus())
        {
            QUndoCommand *cutCommand = new QUndoCommand();
            cutCommand->setText("lineEdit_3");
            ui->lineEdit_3->cut();
            undoStack->push(cutCommand);
        }
    }
    if(ui->tableWidget->hasFocus())
    {
        ui->tableWidget->cut();
    }
}


void MainWindow::actionundo_triggered()
{
    QString undoText = undoStack->undoText();
    ui->actionredo->setEnabled(true);

    if(undoText == "lineEdit")
    {
        undoOperation = true;
        ui->lineEdit->undo();
        undoOperation = false;

    }
    if(undoText == "lineEdit_2")
    {
        undoOperation = true;
        ui->lineEdit_2->undo();
        undoOperation = false;
    }
    if(undoText == "lineEdit_3")
    {
        undoOperation = true;
        ui->lineEdit_3->undo();
        undoOperation = false;
    }
    if(undoText == "tablewidget")
    {
        undoOperation = true;
        ui->tableWidget->undo();
        undoOperation = false;
    }
    undoStack->undo();
    if (undoStack->index() == 0)
    {
        ui->actionundo->setEnabled(false);
    }
    int currentIndex = undoStack->index();
    if (currentIndex > 0)
    { // Проверяем, есть ли предыдущая команда
        const QUndoCommand *lastCommand = undoStack->command(currentIndex-1);

        if (lastCommand)
        {
            // QString w = lastCommand->text();
            // QMessageBox::information(this, "Last Command",
            //                          "Last command: " + lastCommand->text());
            QLineEdit *lineEdit = findChild<QLineEdit*>(lastCommand->text());

            if (lineEdit)
            {
                lineEdit->setFocus();
            }

        }
    }
}


void MainWindow::actionredo_triggered()
{
    QString redoText = undoStack->redoText();
    ui->actionundo->setEnabled(true);

    if(redoText == "lineEdit")
    {
        undoOperation = true;
        ui->lineEdit->redo();
        undoOperation = false;
    }
    if(redoText == "lineEdit_2")
    {
        undoOperation = true;
        ui->lineEdit_2->redo();
        undoOperation = false;
    }
    if(redoText == "lineEdit_3")
    {
        undoOperation = true;
        ui->lineEdit_3->redo();
        undoOperation = false;
    }
    if(redoText == "tablewidget")
    {
        undoOperation = true;
        ui->tableWidget->redo();
        undoOperation = false;
    }
    undoStack->redo();
    if (undoStack->index() == undoStack->count())
    {
        ui->actionredo->setEnabled(false);
    }
    int currentIndex = undoStack->index();
    if (currentIndex > 0)
    { // Проверяем, есть ли предыдущая команда
        const QUndoCommand *lastCommand = undoStack->command(currentIndex - 1);

        if (lastCommand)
        {
            QString w = lastCommand->text();
            QLineEdit *lineEdit = findChild<QLineEdit*>(lastCommand->text());

            if (lineEdit)
            {
                lineEdit->setFocus();
            }
        }
    }
}

void MainWindow::createUndoView()
{
    undoView = new QUndoView(undoStack);
    undoView->setWindowTitle(tr("Command List"));
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}

void MainWindow::edit()
{
    cleanState = false;
    if (undoOperation)
        return;
    QUndoCommand *cutCommand = new QUndoCommand();
    cutCommand->setText("lineEdit");
    undoStack->push(cutCommand);

    ui->actionundo->setEnabled(true);
    ui->actionredo->setEnabled(true);
}

void MainWindow::edit_2()
{
    cleanState = false;
    if (undoOperation)
        return;
    QUndoCommand *cutCommand = new QUndoCommand();
    cutCommand->setText("lineEdit_2");
    undoStack->push(cutCommand);

    ui->actionundo->setEnabled(true);
    ui->actionredo->setEnabled(true);
}
void MainWindow::edit_3()
{
    cleanState = false;
    if (undoOperation)
        return;
    QUndoCommand *cutCommand = new QUndoCommand();
    cutCommand->setText("lineEdit_3");
    undoStack->push(cutCommand);

    ui->actionundo->setEnabled(true);
    ui->actionredo->setEnabled(true);
}
void MainWindow::edit_4()
{
    cleanState = false;
    if (undoOperation)
        return;
    QUndoCommand *cutCommand = new QUndoCommand();
    cutCommand->setText("tablewidget");
    undoStack->push(cutCommand);

    ui->actionundo->setEnabled(true);
    ui->actionredo->setEnabled(true);
}

void MainWindow::on_actionclear_triggered()
{
    undoStack->clear();
}
void MainWindow::on_actionsave_triggered()
{
    ui->actionundo->setEnabled(false);
    ui->actionredo->setEnabled(false);
    QString filter = "Файл конфигурации проекта (*.imview);;Все файлы (*.*)";
    //QString filter = "Файл конфигурации проекта (*.xml);;Все файлы (*.*)";
    QString str = QFileDialog::getSaveFileName(this, "Выбрать имя, под которым сохранить данные", "../Output", filter);
    cleanState = true;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    if(!cleanState)
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Программа еще работает", "Закрыть программу?", QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
        switch (resBtn)
        {
        case QMessageBox::Yes:
            on_actionsave_triggered();
            event->accept();
            break;
        case QMessageBox::No:
            event->accept();
            break;
        default:
        case QMessageBox::Cancel:
            event->ignore();
        }
    }
}

