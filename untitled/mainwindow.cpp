#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include "QDebug"
#include "QShortcut"
#include "QKeyEvent"
#include "QLineEdit"
#include "pushbuttondelegate.h"
#include <QSystemTrayIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cleanState(true)
    , undoOperation(false)
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

    ui->treeView->setSelectionBehavior(QTreeView :: SelectRows); // Выбираем всю строку за раз
    ui->treeView->setSelectionMode(QTreeView :: SingleSelection); // Одиночный выбор, при этом вся строка над ним является одной строкой меню
    ui->treeView->setAlternatingRowColors(true); // Цвет каждой строки интервала разный, при наличии qss этот атрибут недействителен
    ui->treeView->setFocusPolicy(Qt :: NoFocus);
    ui->treeView->setExpandsOnDoubleClick(true);
    //ui->treeView->setRootIsDecorated(true);
    QFont newFontt("DroidSans", 10, QFont::Normal, false);
    ui->treeView->setFont(newFontt);
    ui->treeView->setBackgroundRole(QPalette :: Dark);
    ui->treeView->setExpandsOnDoubleClick(false);

    QObject::connect(ui->treeView, &QTreeView::clicked,this, [this]()
                     {
                         if (ui->treeView->isExpanded(ui->treeView->currentIndex()))
                         {
                             ui->treeView->collapse(ui->treeView->currentIndex());
                         }else{
                             ui->treeView->expand(ui->treeView->currentIndex());
                         }
                     });

    //Создание модели QTreeView
    model2=new QStandardItemModel(ui->treeView);
    model2->setHorizontalHeaderLabels (QStringList () << tr("Наименование") << tr("Свойство")); // Установить заголовок столбца
    ui->treeView->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->treeView->setAlternatingRowColors(true);



    //настройка итемов QTreeView
    QList<QStandardItem*> items1;
    item1 = new QStandardItem(tr("Общее настройки сессии"));
    QString w0=item1->text();
    item1->setToolTip(w0);
    item2 = new QStandardItem();
    items1.append(item1);
    items1.append(item2);
    model2->appendRow(items1);
    item1->setSelectable(false);
    item1->setEditable(false);
    item2->setSelectable(false);
    item2->setEditable(false);
    QFont newFont("DroidSans", 10, QFont::Bold,false);
    item1->setFont(newFont);

    QList<QStandardItem*> items2;
    item3 = new QStandardItem(tr("Название сессии"));
    item3->setEditable(false);
    QString w1=item3->text();
    item3->setToolTip(w1);
    item4 = new QStandardItem(tr("Имя сеанса"));
    QString w2=item4->text();
    item4->setToolTip(w2);
    items2.append(item3);
    items2.append(item4);
    item1->appendRow(items2);
    items2.clear();

    item7 = new QStandardItem(tr("Тип эксперимента"));
    item7->setEditable(false);
    QString w9=item7->text();
    item7->setToolTip(w9);
    QFont newFont10("DroidSans", 10, QFont::Bold,false);
    item7->setFont(newFont10);
    item8 = new QStandardItem();
    item8->setEditable(false);
    items2.append(item7);
    items2.append(item8);
    item1->appendRow(items2);
    items2.clear();

    item87 = new QStandardItem(tr("Идентификация данных схемы замещения"));
    item87->setEditable(false);
    QString w10=item87->text();
    item87->setToolTip(w10);
    item88 = new QStandardItem(tr("Выбрать тип эксперимента"));
    QString w11=item88->text();
    item88->setToolTip(w11);
    items2.append(item87);
    items2.append(item88);
    item7->appendRow(items2);
    items2.clear();

    item105 = new QStandardItem(tr("Загрузка данных ручной идентификации"));
    item105->setEditable(false);
    item105->setEnabled(false);
    QString w12=item105->text();
    item105->setToolTip(w12);
    item106 = new QStandardItem(tr("Указать каталог"));
    item106->setEnabled(false);
    QString w13=item106->text();
    item106->setToolTip(w13);
    items2.append(item105);
    items2.append(item106);
    item7->appendRow(items2);
    items2.clear();

    item79 = new QStandardItem(tr("Наблюдатель состояния"));
    item79->setEditable(false);
    QString w14=item79->text();
    item79->setToolTip(w14);
    item80 = new QStandardItem(tr("Выбрать тип эксперимента"));
    QString w15=item80->text();
    item80->setToolTip(w15);
    items2.append(item79);
    items2.append(item80);
    item7->appendRow(items2);
    items2.clear();

    item81 = new QStandardItem(tr("Чтение данных для наблюдателя скорости"));
    item81->setEditable(false);
    item81->setEnabled(false);
    QString w16=item81->text();
    item81->setToolTip(w16);
    item82 = new QStandardItem(tr("Указать каталог"));
    item82->setEnabled(false);
    QString w17=item82->text();
    item82->setToolTip(w17);
    items2.append(item81);
    items2.append(item82);
    item7->appendRow(items2);
    items2.clear();

    item116 = new QStandardItem(tr("Настройка каналов"));
    item116->setEditable(false);
    item117 = new QStandardItem(tr("Выбрать каналы"));
    QString w447=item117->text();
    item117->setToolTip(w447);
    items2.append(item116);
    items2.append(item117);
    item7->appendRow(items2);
    items2.clear();

    item65 = new QStandardItem(tr("Сохранение данных"));
    item65->setEditable(false);
    QFont newFont11("DroidSans", 10, QFont::Bold,false);
    item65->setFont(newFont11);
    QString w18=item65->text();
    item65->setToolTip(w18);
    item66 = new QStandardItem();
    item66->setEditable(false);
    items2.append(item65);
    items2.append(item66);
    item1->appendRow(items2);
    items2.clear();

    item67 = new QStandardItem(tr("Данные идентификации"));
    item67->setEditable(false);
    QString w19=item67->text();
    item67->setToolTip(w19);
    item68 = new QStandardItem(tr("Выбрать режим"));
    items2.append(item67);
    items2.append(item68);
    item65->appendRow(items2);
    items2.clear();

    item69 = new QStandardItem(tr("Данные электромагнитных процессов"));
    item69->setEditable(false);
    QString w21=item69->text();
    item69->setToolTip(w21);
    item70 = new QStandardItem(tr("Выбрать режим"));
    items2.append(item69);
    items2.append(item70);
    item65->appendRow(items2);
    items2.clear();

    item71 = new QStandardItem(tr("Данные тепловых процессов"));
    item71->setEditable(false);
    QString w23=item71->text();
    item71->setToolTip(w23);
    item72 = new QStandardItem(tr("Выбрать режим"));
    items2.append(item71);
    items2.append(item72);
    item65->appendRow(items2);
    items2.clear();

    item73 = new QStandardItem(tr("Данные вентиляционных процессов"));
    item73->setEditable(false);
    QString w25=item73->text();
    item73->setToolTip(w25);
    item74 = new QStandardItem(tr("Выбрать режим"));
    items2.append(item73);
    items2.append(item74);
    item65->appendRow(items2);
    items2.clear();

    item75 = new QStandardItem(tr("Данные прогноза температур"));
    item75->setEditable(false);
    QString w27=item75->text();
    item75->setToolTip(w27);
    item76 = new QStandardItem(tr("Выбрать режим"));
    items2.append(item75);
    items2.append(item76);
    item65->appendRow(items2);
    items2.clear();

    item77 = new QStandardItem(tr("Данные остаточного теплового ресурса"));
    item77->setEditable(false);
    QString w29=item77->text();
    item77->setToolTip(w29);
    item78 = new QStandardItem(tr("Выбрать режим"));
    items2.append(item77);
    items2.append(item78);
    item65->appendRow(items2);
    items2.clear();

    QList<QStandardItem*> items3;
    item9 = new QStandardItem(tr("Идентификация параметров схемы замещения"));
    item10 = new QStandardItem();
    items3.append(item9);
    items3.append(item10);
    model2->appendRow(items3);
    items3.clear();
    item9->setSelectable(false);
    item9->setEditable(false);
    QString w31=item9->text();
    item9->setToolTip(w31);
    item10->setSelectable(false);
    item10->setEditable(false);
    QString w32=item10->text();
    item10->setToolTip(w32);
    QFont newFont2("SansSerif", 10, QFont::Bold,false);
    item9->setFont(newFont2);

    QList<QStandardItem*> items4;
    item13 = new QStandardItem(tr("Режим расчета"));
    item13->setEditable(false);
    QString w35=item13->text();
    item13->setToolTip(w35);
    item14 = new QStandardItem(tr("Выберите режим"));
    QString w73=item14->text();
    item14->setToolTip(w73);
    items4.append(item13);
    items4.append(item14);
    item9->appendRow(items4);
    items4.clear();

    item93 = new QStandardItem(tr("Настроечный коэффициент gd="));
    item93->setEditable(false);
    item93->setEnabled(false);
    QString w37=item93->text();
    item93->setToolTip(w37);
    item94 = new QStandardItem(tr("0"));
    item94->setEnabled(false);
    item94->setCheckable(false);
    QString w38=item94->text();
    item94->setToolTip(w38);
    items4.append(item93);
    items4.append(item94);
    item9->appendRow(items4);
    items4.clear();

    item95 = new QStandardItem(tr("Настроечный коэффициент ki="));
    item95->setEditable(false);
    item95->setEnabled(false);
    QString w39=item95->text();
    item95->setToolTip(w39);
    item96 = new QStandardItem(tr("0"));
    item96->setEnabled(false);
    item96->setCheckable(false);
    QString w40=item96->text();
    item96->setToolTip(w40);
    items4.append(item95);
    items4.append(item96);
    item9->appendRow(items4);
    items4.clear();

    item97 = new QStandardItem(tr("Настроечный коэффициент gb="));
    item97->setEditable(false);
    item97->setEnabled(false);
    QString w41=item97->text();
    item97->setToolTip(w41);
    item98 = new QStandardItem(tr("0"));
    item98->setEnabled(false);
    item98->setCheckable(false);
    QString w42=item98->text();
    item98->setToolTip(w42);
    items4.append(item97);
    items4.append(item98);
    item9->appendRow(items4);
    items4.clear();

    item99 = new QStandardItem(tr("Настроечный коэффициент kpsi="));
    item99->setEditable(false);
    item99->setEnabled(false);
    QString w43=item99->text();
    item99->setToolTip(w43);
    item100 = new QStandardItem(tr("0"));
    item100->setEnabled(false);
    item100->setCheckable(false);
    QString w44=item100->text();
    item100->setToolTip(w44);
    items4.append(item99);
    items4.append(item100);
    item9->appendRow(items4);
    items4.clear();

    item101 = new QStandardItem(tr("Настроечный коэффициент gp="));
    item101->setEditable(false);
    item101->setEnabled(false);
    QString w45=item101->text();
    item101->setToolTip(w45);
    item102 = new QStandardItem(tr("0"));
    item102->setEnabled(false);
    item102->setCheckable(false);
    QString w46=item102->text();
    item102->setToolTip(w46);
    items4.append(item101);
    items4.append(item102);
    item9->appendRow(items4);
    items4.clear();

    item103 = new QStandardItem(tr("Настроечный коэффициент gpsi="));
    item103->setEditable(false);
    item103->setEnabled(false);
    QString w47=item103->text();
    item103->setToolTip(w47);
    item104 = new QStandardItem(tr("0"));
    item104->setEnabled(false);
    item104->setCheckable(false);
    QString w48=item104->text();
    item104->setToolTip(w48);
    items4.append(item103);
    items4.append(item104);
    item9->appendRow(items4);
    items4.clear();

    QList<QStandardItem*> items17;
    item145 = new QStandardItem(tr("Тепловая идентификация"));
    item146 = new QStandardItem();
    items17.append(item145);
    items17.append(item146);
    model2->appendRow(items17);
    items17.clear();
    item145->setSelectable(false);
    item145->setEditable(false);
    QString w64=item145->text();
    item145->setToolTip(w64);
    item146->setSelectable(false);
    item146->setEditable(false);
    QFont newFont17("SansSerif", 10, QFont::Bold,false);
    item145->setFont(newFont17);

    item147 = new QStandardItem(tr("Вид тепловой модели"));
    item147->setEditable(false);
    QString w65=item147->text();
    item147->setToolTip(w65);
    item148 = new QStandardItem(tr("Выберите вид"));
    QString w66=item148->text();
    item148->setToolTip(w66);
    items17.append(item147);
    items17.append(item148);
    item145->appendRow(items17);
    items17.clear();

    item149 = new QStandardItem(tr("Вид тепловой модели"));
    item149->setEditable(false);
    QString w67=item149->text();
    item149->setToolTip(w67);
    item150 = new QStandardItem(tr("Выберите вид"));
    QString w68=item150->text();
    item150->setToolTip(w68);
    items17.append(item149);
    items17.append(item150);
    item145->appendRow(items17);
    items17.clear();

    item151 = new QStandardItem(tr("Вид тепловой модели"));
    item151->setEditable(false);
    QString w69=item151->text();
    item151->setToolTip(w69);
    item152 = new QStandardItem(tr("Выберите вид"));
    QString w70=item152->text();
    item152->setToolTip(w70);
    items17.append(item151);
    items17.append(item152);
    item145->appendRow(items17);
    items17.clear();

    QList<QStandardItem*> items18;
    item153 = new QStandardItem(tr("Вентиляционная идентификация"));
    item154 = new QStandardItem();
    items18.append(item153);
    items18.append(item154);
    model2->appendRow(items18);
    items18.clear();
    item153->setSelectable(false);
    item153->setEditable(false);
    QString w71=item153->text();
    item153->setToolTip(w71);
    item154->setSelectable(false);
    item154->setEditable(false);
    QFont newFont18("SansSerif", 10, QFont::Bold,false);
    item153->setFont(newFont18);

    item155 = new QStandardItem(tr("Вид вентиляции"));
    item155->setEditable(false);
    QString w72=item155->text();
    item155->setToolTip(w72);
    item156 = new QStandardItem(tr("Выберите вид"));
    QString w74=item156->text();
    item156->setToolTip(w74);
    items18.append(item155);
    items18.append(item156);
    item153->appendRow(items18);
    items18.clear();

    item157 = new QStandardItem(tr("Режим расчета"));
    item157->setEditable(false);
    QString w75=item155->text();
    item157->setToolTip(w75);
    item158 = new QStandardItem(tr("Выберите вид"));
    QString w76=item156->text();
    item158->setToolTip(w76);
    items18.append(item157);
    items18.append(item158);
    item153->appendRow(items18);
    items18.clear();

    item159 = new QStandardItem(tr("Геометрические параметры"));
    item159->setEditable(false);
    item159->setEnabled(false);
    QString w77=item159->text();
    item159->setToolTip(w77);
    item160 = new QStandardItem(tr("Введите данные"));
    item160->setEnabled(false);
    QString w78=item160->text();
    item160->setToolTip(w78);
    items18.append(item159);
    items18.append(item160);
    item153->appendRow(items18);
    items18.clear();

    item161 = new QStandardItem(tr("Учет тепловых процессов"));
    item161->setEditable(false);
    QString w79=item161->text();
    item161->setToolTip(w79);
    item162 = new QStandardItem(tr("Введите данные"));
    QString w80=item162->text();
    item162->setToolTip(w80);
    items18.append(item161);
    items18.append(item162);
    item153->appendRow(items18);
    items18.clear();

    QList<QStandardItem*> items5;
    item17 = new QStandardItem(tr("Электромагнитная модель"));
    item18 = new QStandardItem();
    items5.append(item17);
    items5.append(item18);
    model2->appendRow(items5);
    items5.clear();
    item17->setSelectable(false);
    item17->setEditable(false);
    QString w49=item17->text();
    item17->setToolTip(w49);
    item18->setSelectable(false);
    item18->setEditable(false);
    QFont newFont3("SansSerif", 10, QFont::Bold,false);
    item17->setFont(newFont3);

    QList<QStandardItem*> items6;
    item19 = new QStandardItem(tr("Pежим работы двигателя"));
    item19->setEditable(false);
    QString w50=item19->text();
    item19->setToolTip(w50);
    item20 = new QStandardItem(tr("Выбрать режим"));
    QString w51=item20->text();
    item20->setToolTip(w51);
    items6.append(item19);
    items6.append(item20);
    item17->appendRow(items6);
    items6.clear();

    item21 = new QStandardItem(tr("Время цикла, с:"));
    item21->setEditable(false);
    item21->setEnabled(false);
    QString w52=item21->text();
    item21->setToolTip(w52);
    item22 = new QStandardItem(tr("0"));
    item22->setEnabled(false);
    QString w53=item22->text();
    item22->setToolTip(w53);
    items6.append(item21);
    items6.append(item22);
    item17->appendRow(items6);
    items6.clear();

    item175 = new QStandardItem(tr("Время работы в цикле, с:"));
    item175->setEditable(false);
    item175->setEnabled(false);
    QString w83=item175->text();
    item175->setToolTip(w83);
    item176 = new QStandardItem(tr("0"));
    item176->setEnabled(false);
    QString w84=item176->text();
    item176->setToolTip(w84);
    items6.append(item175);
    items6.append(item176);
    item17->appendRow(items6);
    items6.clear();

    item177 = new QStandardItem(tr("Время пуска в цикле, с:"));
    item177->setEditable(false);
    item177->setEnabled(false);
    QString w87=item177->text();
    item177->setToolTip(w87);
    item178 = new QStandardItem(tr("0"));
    item178->setEnabled(false);
    QString w88=item178->text();
    item178->setToolTip(w88);
    items6.append(item177);
    items6.append(item178);
    item17->appendRow(items6);
    items6.clear();

    item23 = new QStandardItem(tr("Выбор системы отсчета времени"));
    item23->setEditable(false);
    QString w54=item23->text();
    item23->setToolTip(w54);
    item24 = new QStandardItem(tr("Выберите тип отсчета"));
    QString w55=item24->text();
    item24->setToolTip(w55);
    items6.append(item23);
    items6.append(item24);
    item17->appendRow(items6);
    items6.clear();

    item173 = new QStandardItem(tr("Время работы, с:"));
    item173->setEditable(false);
    item173->setEnabled(false);
    QString w81=item173->text();
    item173->setToolTip(w81);
    item174 = new QStandardItem(tr("0"));
    item174->setEnabled(false);
    QString w82=item174->text();
    item174->setToolTip(w82);
    items6.append(item173);
    items6.append(item174);
    item17->appendRow(items6);
    items6.clear();

    item91 = new QStandardItem(tr("Выбор системы электропривода"));
    item91->setEditable(false);
    QString w58=item91->text();
    item91->setToolTip(w58);
    item92 = new QStandardItem(tr("Выбрать режим"));
    QString w59=item92->text();
    item92->setToolTip(w59);
    items6.append(item91);
    items6.append(item92);
    item17->appendRow(items6);
    items6.clear();

    item129 = new QStandardItem(tr("Ввод напряжения питания двигателя"));
    item129->setEditable(false);
    QString w60=item129->text();
    item129->setToolTip(w60);
    item130 = new QStandardItem(tr("0"));
    QString w61=item130->text();
    item130->setToolTip(w61);
    items6.append(item129);
    items6.append(item130);
    item17->appendRow(items6);
    items6.clear();

    item131 = new QStandardItem(tr("Ввод значение момента нагрузки"));
    item131->setEditable(false);
    QString w62=item131->text();
    item131->setToolTip(w62);
    item132 = new QStandardItem(tr("0"));
    QString w63=item132->text();
    item132->setToolTip(w63);
    items6.append(item131);
    items6.append(item132);
    item17->appendRow(items6);
    items6.clear();

    //Настройка представления модели QTreeView
    ui->treeView->setModel(model2);
    ui->treeView->header()->resizeSection(0,270);
    ui->treeView->header()->setSectionResizeMode(0,QHeaderView::Interactive);
    ui->treeView->header()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->treeView->expandAll();

    ButtonColumnDelegate* buttonColumnDelegate = new ButtonColumnDelegate(this); //создание делегата для создания комбобоксов
    ui->treeView->setItemDelegateForColumn(1, buttonColumnDelegate);

    ui->treeView->setStyleSheet(
        "QScrollBar:vertical {border-width: 0px;border-style: solid;"
        "background-color: #FFFFFF; width: 18px;}"
        "QScrollBar::handle:vertical {background-color:#d0d2d2;min-height: 25px;"
        "margin-left:5px;margin-right:5px;border-radius:4px;margin-top:5px;margin-bottom:5px;}"
        "QScrollBar::handle:vertical:hover {background-color:#97d5f0;}"
        "QScrollBar::add-line:vertical {border: 0px solid black;"
        "height: 0px;subcontrol-position: bottom;subcontrol-origin: margin;}"
        "QScrollBar::sub-line:vertical {border: 0px solid black;"
        "height: 0px;subcontrol-position: top;subcontrol-origin: margin;}"
        "*{"
        "background: rgb(255, 255, 222);"
        "}"
        "*{"
        "alternate-background-color: rgb(255, 255, 191);"
        "}"
        "*::item{"
        "    border-top-width: 0px;"
        "    border-right-width: 1px;"
        "    border-bottom-width: 1px;"
        "    border-left-width: 0px;"
        "    border-style: solid;"
        "    border-color: silver;"
        "}"
        "*::item:selected{"
        "    background: palette(Highlight);"
        "}"
        //                            "*::item:has-children{"
        //                            "    background: rgb(128,128,128);"
        //                            "}"
        "::branch"
        "{"
        "border-bottom: 1px solid silver;"
        "border-image: none 0;"
        "image: none 0;"
        "}"
        "::branch:has-children:!has-siblings:closed,"
        "::branch:closed:has-children:has-siblings {"
        "        border-image: none;"
        "        image: url(:/img/data/branch-closed2.png);"
        "}"
        "::branch:open:has-children:!has-siblings,"
        "::branch:open:has-children:has-siblings  {"
        "        border-image: none;"
        "        image: url(:/img/data/branch-open2.png);"
        "}"
        "::branch:has-siblings:!adjoins-item {"
        "    border-image: url(:/img/data/vline1.png) 0;"
        "}"

        "::branch:has-siblings:adjoins-item {"
        "    border-image: url(:/img/data/branch-more1.png) 0;"
        "}"

        "::branch:!has-children:!has-siblings:adjoins-item {"
        "    border-image: url(:/img/data/branch-end1.png) 0;"
        "}"

        "::branch:has-children:!has-siblings:closed,"
        "::branch:closed:has-children:has-siblings {"
        "        border-image: none;"
        "        image: url(:/img/data/branch-closed2.png);"
        "}"

        "::branch:open:has-children:!has-siblings,"
        "::branch:open:has-children:has-siblings  {"
        "        border-image: none;"
        "        image: url(:/img/data/branch-open2.png);"
        "}"

        );

    ui->treeView->setRootIsDecorated(true);

    connect(ui->lineEdit,&QLineEdit::textEdited, this, &MainWindow::edit);
    connect(ui->lineEdit_2,&QLineEdit::textEdited, this, &MainWindow::edit_2);
    connect(ui->lineEdit_3,&QLineEdit::textEdited, this, &MainWindow::edit_3);
    connect(ui->tableWidget,&QTableWidget::itemChanged, this, &MainWindow::edit_4);
    connect(ui->actionundo, &QAction::triggered, this, &MainWindow::actionundo_triggered);
    connect(ui->actionredo, &QAction::triggered, this, &MainWindow::actionredo_triggered);
    connect(ui->message,&QAction::triggered, this,&MainWindow::message_action);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/img/data/IM_96x96.png"));
    trayIcon->show();
}

void MainWindow::message_action()
{
    // trayIcon->setIcon(QIcon(":/img/data/IM_96x96.png"));
    // QString appName = QCoreApplication::applicationName();
    // QString title = "Заголовок уведомления";
    // //QString title = appName;
    // QString message = "Это содержимое вашего системного сообщения.";
    // trayIcon->showMessage(title, message, QSystemTrayIcon::NoIcon);

    QIcon icon(":/img/data/branch-more.png");
    trayIcon->setIcon(icon);
    trayIcon->show();
    trayIcon->showMessage("Заголовок", "Текст сообщения", QSystemTrayIcon::NoIcon);
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
    rowHeight = ui->treeView->sizeHintForRow(2);
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

