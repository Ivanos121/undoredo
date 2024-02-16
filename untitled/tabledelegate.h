#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H

#include "qstyleditemdelegate.h"

class TableDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TableDelegate(QObject *parent = nullptr);
    TableDelegate();
};

#endif // TABLEDELEGATE_H
