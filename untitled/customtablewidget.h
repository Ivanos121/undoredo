#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QUndoStack>

class CustomTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit CustomTableWidget(QWidget *parent = nullptr);
    void paste();
    void copy();
    void cut();
    void undo();
    void redo();

    void showEvent( QShowEvent* event );


public slots:
    void customTableWidget_cellActivated_Slot(int row, int column);
    void on_customTableWidget_cellChanged_Slot(int row, int column);

private:
    QModelIndexList copyIndexList;
    QUndoStack *undoStack;
    QString prevCellText;

    bool undoOperation;
};

#endif // CUSTOMTABLEWIDGET_H
