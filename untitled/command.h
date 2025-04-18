#ifndef COMMAND_H
#define COMMAND_H

#include "qlabel.h"
#include <QUndoCommand>

class CustomTableWidget;

class AppendText : public QUndoCommand
{
public:
    AppendText(QLabel *doc, QString text);
    void undo();
    void redo();

private:
    QLabel *label;
    QString text;
};

#endif // COMMAND_H
