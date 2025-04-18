#include "customtablewidget.h"

CustomTableWidget::CustomTableWidget(QWidget *parent) : QTableWidget(parent)
    , undoOperation(false)
{
    undoStack = new QUndoStack();

    connect(this, &CustomTableWidget::cellActivated, this, &CustomTableWidget::customTableWidget_cellActivated_Slot);
    connect(this, &CustomTableWidget::cellDoubleClicked, this, &CustomTableWidget::customTableWidget_cellActivated_Slot);

    connect(this, &CustomTableWidget::cellChanged, this, &CustomTableWidget::on_customTableWidget_cellChanged_Slot);

    QUndoView *undoView = new QUndoView(undoStack);
    undoView->setWindowTitle(tr("Table Command List"));
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}

void CustomTableWidget::paste()
{
    QModelIndexList selectedModel=selectedIndexes(); //cell selected
    std::sort(selectedModel.begin(),selectedModel.end()); //ASC sort

    if(selectedModel.isEmpty())
    {
        return;
    }

    QClipboard *clipboard = QApplication::clipboard();
    QString pasteText = clipboard->text();

    QStringList rowList = pasteText.split('\n');

    if (rowList.count() == 1)
    {
        int startRow=selectedModel.first().row();
        int startColumn=selectedModel.first().column();

        setItem(startRow,startColumn,new QTableWidgetItem(pasteText));
    }
    else
    {
        int startRow=selectedModel.first().row();
        int startColumn=selectedModel.first().column();
        int row = startRow;
        int column = startColumn;

        foreach (QString pasteRow, rowList)
        {
            QStringList columnList = pasteRow.split(' ');
            foreach (QString pasteCell, columnList)
            {
                setItem(row, column, new QTableWidgetItem(pasteCell));
                column++;
            }
            row++;
            column = startColumn;
        }
    }
}

void CustomTableWidget::copy()
{
    QModelIndexList selectedModel=selectedIndexes(); //cell selected
    std::sort(selectedModel.begin(),selectedModel.end()); //ASC sort

    if(selectedModel.isEmpty())
    {
        return;
    }

    if (selectedModel.count() == 1)
    {
        int startRow=selectedModel.first().row();
        int startColumn=selectedModel.first().column();

        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(item(startRow, startColumn)->text());
    }
    else
    {
        int startRow=selectedModel.first().row();

        QString copyCellsCSV;
        int currentRow = startRow;
        foreach (QModelIndex index, selectedModel)
        {
            if (index.row() != currentRow)
            {
                copyCellsCSV += "\n";
                currentRow = index.row();
            }
            copyCellsCSV += item(index.row(), index.column())->text() + " ";
        }
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(copyCellsCSV);
    }
}

void CustomTableWidget::cut()
{
    QModelIndexList selectedModel=selectedIndexes(); //cell selected
    std::sort(selectedModel.begin(),selectedModel.end()); //ASC sort

    if(selectedModel.isEmpty())
    {
        return;
    }

    int startRow=selectedModel.first().row();
    int startColumn=selectedModel.first().column();

    QClipboard *clipboard = QApplication::clipboard();

    clipboard->setText(item(startRow, startColumn)->text());
    setItem(startRow,startColumn,new QTableWidgetItem(""));
}

void CustomTableWidget::undo()
{
    QString undoText = undoStack->undoText();
    QStringList undoTextList = undoText.split(" ");

    QString rowString = undoTextList[0];
    QString columnString = undoTextList[1];
    QString prevString = undoTextList[2];

    int row = rowString.toInt();
    int column = columnString.toInt();

    undoOperation = true;
    item(row, column)->setText(prevString);
    undoOperation = false;

    undoStack->undo();
}

void CustomTableWidget::redo()
{
    QString redoText = undoStack->redoText();
    QStringList redoTextList = redoText.split(" ");

    QString rowString = redoTextList[0];
    QString columnString = redoTextList[1];
    QString curString = redoTextList[3];

    int row = rowString.toInt();
    int column = columnString.toInt();

    undoOperation = true;
    item(row, column)->setText(curString);
    undoOperation = false;

    undoStack->redo();
}

void CustomTableWidget::customTableWidget_cellActivated_Slot(int row, int column)
{
    prevCellText = item(row, column)->text();
}

void CustomTableWidget::on_customTableWidget_cellChanged_Slot(int row, int column)
{
    if (!undoOperation)
    {
        QUndoCommand *addCommand = new QUndoCommand();
        QString curCellText = item(row, column)->text();
        addCommand->setText(QString("%1 %2 %3 %4").arg(row).arg(column).arg(prevCellText).arg(curCellText));
        undoStack->push(addCommand);
    }
}

void CustomTableWidget::showEvent( QShowEvent*)
{
    undoStack->clear();
}
