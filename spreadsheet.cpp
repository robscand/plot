/*!
 * \file
 * \brief Исполнительный файл.
 *
 * Исходный файл класса Spreadsheet, в котором создается основной
 * функционал программы: обработка и представление данных из файлов,
 * реализация технологии drag-and-drop.
 */
#include <QtGui>
#include <QApplication>
#include <QMessageBox>
#include <QHeaderView>
#include <QAbstractSlider>

#include "cell.h"
#include "spreadsheet.h"
#include "delegates/combobox.h"
#include "delegates/spinbox.h"

Spreadsheet::Spreadsheet(QWidget *parent)
    : QTableWidget(parent)
{
    autoRecalc = true;

    setItemPrototype(new Cell); //При вводе текста в ячейку QTableWidget создает элемент QTableWidgetItem для хранения текста. С помощью этой функции создаются элементы Cell. Когда требуется новый элемент, QTableWidget дублирует элемент, переданный в качестве прототипа.
    setAcceptDrops(true); //Включение возможности опускания переносимых объектов в области отображения Spreadsheet.
    setSelectionMode(ContiguousSelection); //Установка режима выделения области на значение QAbstractItemView::ContiguousSelection, чтобы могла быть выделена только одна прямоугольная область.
    setDefaultDropAction(Qt::CopyAction);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //Заголовоки и вслед за ними колонки растягиваются под размер данных в ячейках.

    setItemDelegateForColumn(2, new SpinBoxDelegate(this));
    setItemDelegateForColumn(3, new ComboBoxDelegate(this));
    setItemDelegateForColumn(4, new SpinBoxDelegate(this));
    setItemDelegateForColumn(5, new SpinBoxDelegate(this));
    setItemDelegateForColumn(7, new SpinBoxDelegate(this));

    connect(this, SIGNAL(itemChanged(QTableWidgetItem *)), //Соединение сигнала itemChanged() виджета таблицы с закрытым слотом somethingChanged().
                this, SLOT(somethingChanged()));           //Это гарантирует вызова слота somethingChanged() при редактировании ячейки пользователем.

    clear();
    retranslateStrings();
}

void Spreadsheet::clear() //Служит для инициализации электронной таблицы; изменяет ее размер и задание заголовков столбцов.
{                         //Вызывается из конструктора Spreadsheet и из MainWindow::newFile()
    setRowCount(0);               //В начале мы уменьшаем размер таблицы до 0x0. Это приводит к очистке таблицы, включая заголовки.
    setColumnCount(0);
    setRowCount(RowCount);        //Затем устанавливаем ее размер на ColumnCount x RowCount(8x25)
    setColumnCount(ColumnCount);

    item0 = new QTableWidgetItem;
    setHorizontalHeaderItem(0 , item0);

    item1 = new QTableWidgetItem;
    setHorizontalHeaderItem(1 , item1);

    item2 = new QTableWidgetItem;
    setHorizontalHeaderItem(2 , item2);

    item3 = new QTableWidgetItem;
    setHorizontalHeaderItem(3 , item3);

    item4 = new QTableWidgetItem;
    setHorizontalHeaderItem(4 , item4);

    item5 = new QTableWidgetItem;
    setHorizontalHeaderItem(5 , item5);

    item6 = new QTableWidgetItem;
    setHorizontalHeaderItem(6 , item6);

    item7 = new QTableWidgetItem;
    setHorizontalHeaderItem(7, item7);

    setCurrentCell(0, 0); //Перемещение курсора на ячейку A1
}

void Spreadsheet::retranslateStrings()
{
    item0->setText(tr("Manufacturer"));
    item1->setText(tr("Model"));
    item2->setText(tr("Price"));
    item3->setText(tr("Method of printing"));
    item4->setText(tr("Width of printing, mm"));
    item5->setText(tr("Number of colors"));
    item6->setText(tr("Resolution, dpi"));
    item7->setText(tr("Power consumption, W"));
}

void Spreadsheet::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateStrings();
    QTableWidget::changeEvent(event);
}

Cell *Spreadsheet::cell(int row, int column) const //Закрытая функция cell() возвращает для заданной строки и столбца указатель на объект Cell.
{
    return static_cast<Cell *>(item(row, column));
}

QString Spreadsheet::text(int row, int column) const //Закрытая функция text() возвращает текст или формулу (если текст совпадает с формулой) заданной ячейки.
{
    Cell *c = cell(row, column);
    if (c) {
        return c->text(); //Функция QTableWidgetItem::text() эквивалентна вызову data(Qt::DisplayRole).toString().
    } else {              //По этой причине в Cell нет определения функции text().
        return "";
    }
}

QString Spreadsheet::formula(int row, int column) const //Закрытая функция formula() возвращает формулу ячейки.
{
    Cell *c = cell(row, column);
    if (c) {
        return c->formula();
    } else {
        return "";
    }
}

void Spreadsheet::setFormula(int row, int column, const QString &formula)// Закрытая функция setFormula задает формулу для указанной ячейки
{
    Cell *c = cell(row, column); //Если ячейка уже имеет объект Cell, мы его повторно используем
    if (!c) {                    //В противном случае создаем новый объект Cell и вызываем QTableWidget::setItem() для вставки его в таблицу
        c = new Cell;
        setItem(row, column, c);
    }
    c->setFormula(formula); //В конце мы вызываем для этой ячейки функцию setFormula(), что приводит к перерисовке ячейки, если она отображается на экране
}

QString Spreadsheet::currentLocation() const //Функция currentLocation() возвращает текущее положение ячейки
{
    return QChar('A' + currentColumn()) + QString::number(currentRow() + 1); //Используется обычная форма представления координат ячейки в таблице. Буквой обозначается положение столбца, зв которой идет номер строки.
}

QString Spreadsheet::currentFormula() const //Функция currentFormula() возвращает формулу текущей ячейки.
{
    return formula(currentRow(), currentColumn());
}

void Spreadsheet::somethingChanged()//Закрытый слот somethingChanged() делает пересчет всей электронной таблицы, если включен режим Auto-Recalcualate
{                                   //Также он генерирует сигнал modified()
    if (autoRecalc)
        recalculate();
    emit modified();
}

bool Spreadsheet::writeFile(const QString &fileName) //Функция writeFile() вызывается из MainWindow::saveFile() для записи файла на диск. При успешной записи возвращает true, false при ошибке.
{
    QFile file(fileName); //Создаем объект QFile, задавая имя файла
    if (!file.open(QIODevice::WriteOnly)) { //Вызываем функцию open() для открытия файла для записи данных
        QMessageBox::warning(this, tr("Spreadsheet"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file); //Объект QDataStream предназначен для работы с QFile и использует его для записи данных
    out.setVersion(QDataStream::Qt_5_6); //Установка версии двоичного формата данных
    out << quint32(MagicNumber); //Запись в поток значения переменной MagicNumber, c которой начинается таблица. Тип значения преобразован в двоичный формат quint32,
                                 //так как элементарные целочисленные типы C++ имеют на различных платформах разный размер. Это гарантирует использование объявленного размера.
    QApplication::setOverrideCursor(Qt::WaitCursor); //Перед записью мы изменяем курсор приложения на курсор ожидания (анимированный Уроборос)
    for (int row = 0; row < RowCount; row++) {
        for (int column = 0; column < ColumnCount; ++column) {
            QString str = formula(row, column);
            if (!str.isEmpty())
                out << quint16(row) << quint16(column) << str; //После MagicNumber идет последовательность блоков, содержащих строку, столбец и формулу одной ячейки.
        }                                                       //Пустые ячейки не записываются.
    }
    QApplication::restoreOverrideCursor(); //Восстановление курсора после записи данных
    return true;
} //В конце функции файл автоматически закрывается деструктором QFile

bool Spreadsheet::readFile(const QString &fileName)//Функция readFile() вызывается из MainWindow::loadFile() для чтения данных из файла. При успешном чтении возвращает true, false при ошибке.
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) { //Вызываем функцию open() для открытия файла для чтения данных.
        QMessageBox::warning(this, tr("Spreadsheet"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream in(&file); //Объект QDataStream предназначен для работы с QFile и использует его для чтения данных.
    in.setVersion(QDataStream::Qt_5_6); //Установка версии двоичного формата данных. Формат чтения всегда должен совпадать с форматом записи.

    quint32 magic;
    in >> magic; //Считывание из потока значения переменной magic - "волшебного числа".
    if (magic != MagicNumber) {
        QMessageBox::warning(this, tr("Spreadsheet"),
                             tr("The file is not a Spreadsheet file."));
        return false;
    }

    clear(); //Если в начале файла содержится правильное "волшебное число", вызывается функция clear() для очистки ячеек таблицы.
    retranslateStrings();

    quint16 row;
    quint16 column;
    QString str;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    while (!in.atEnd()) { //Считывание данных ячеек из потока.
        in >> row >> column >> str;
        setFormula(row, column, str);
    }
    resizeColumnsToContents();
    QApplication::restoreOverrideCursor();
    return true;
}

void Spreadsheet::cut() //Слот cut() соответствует пункту меню Edit | Cut.
{
    copy();
    del();
}

void Spreadsheet::addRow() //Слот addRow() соответствует пункту меню Edit | Add Row.
{
    int row(rowCount());
    setRowCount(++row);
}

void Spreadsheet::removeRow() //Слот removeRow() соответствует пункту меню Edit | Remove Row.
{
    int row(rowCount());
    setRowCount(--row);
}

void Spreadsheet::copy() //Слот copy() соответствует пункту меню Edit | Copy.
{
    QTableWidgetSelectionRange range = selectedRange(); //Возвращение выделенного диапазона.
    QString str;

    for (int i = 0; i < range.rowCount(); ++i) { //В цикле обрабатывается вся выделенная область ячеек.
        if (i > 0)                               //Если нет явно выделенной области, то ею будет просто текущая ячейка.
            str += "\n"; //Строки отделяются символом новой строки.
        for (int j = 0; j < range.columnCount(); ++j) {
            if (j > 0)
                str += "\t"; //Столбцы разделяются символом табуляции.
            str += formula(range.topRow() + i, range.leftColumn() + j); //Формула каждой выделенной ячейки добавляется в QString.
        }
    }
    QApplication::clipboard()->setText(str); // QApplication::clipboard() дает доступ к буферу обмена. Вызывая функцию QClipboard::setText() мы делаем текст доступным через буфер обмена.
}

QTableWidgetSelectionRange Spreadsheet::selectedRange() const //Функция selectedRange() возвращает выделенный диапазон.
{                                                       //Выделяется не более одного диапазона, так как в конструкторе указан режим выделения ContiguousSelection
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    if (ranges.isEmpty())                    //Какая-нибудь ячейка всегда должна быть выбрана. Однако такая ситуация обрабатывается,
        return QTableWidgetSelectionRange(); //чтобы защититься от ошибке в программе, приводящей к отсутствию ячейки.
    return ranges.first(); //Возвращение ссылки на первый элемент выделенного диапазона.
}

void Spreadsheet::paste() //Слот paste() соответствует пункту меню Edit | Paste.
{
    QTableWidgetSelectionRange range = selectedRange();
    QString str = QApplication::clipboard()->text(); //Считывание текста из буфера обмена.
    QStringList rows = str.split('\n'); //Статическая функция QString::split() разбивает строку и представляет ее в виде списка QStringList. Каждая строка таблицы представлена в этом списке одной строкой.
    //Определение размеров области копирования.
    int numRows = rows.count(); // Номер строки таблицы является номером строки QStringList.
    int numColumns = rows.first().count('\t') + 1; // Номер столбца является номером символа табуляции в первой строке плюс 1.

    if (range.rowCount() * range.columnCount() != 1
            && (range.rowCount() != numRows
                || range.columnCount() != numColumns)) {
        QMessageBox::information(this, tr("Spreadsheet"),
                                 tr("The information cannot be pasted because the copy "
                                    "and paste areas aren't the same size. "));
        return;
    }

    for (int i = 0; i < numRows; ++i) {                 //При выполнении операции вставки в цикле
        QStringList columns = rows[i].split('\t');      //проходим по строкам и разбиваем каждую строку на значения ячеек, используя QStringList::split().
        for (int j = 0; j < numColumns; ++j) {          //В качестве разделителя используем знак табуляции.
            int row = range.topRow() + i;
            int column = range.leftColumn() + j;
            if (row < RowCount && column < ColumnCount)
                setFormula(row, column, columns[j]);
        }
    }
    somethingChanged();
}

void Spreadsheet::del() //Слот del() соответствует пункту меню Edit | Delete.
{
    QList<QTableWidgetItem *> items = selectedItems(); //Если есть выделенные ячейки, они удаляются.
    if (!items.isEmpty()) {
        foreach (QTableWidgetItem *item, items)
            delete item; //Для очистки ячеек достаточно использовать оператор delete для каждого объекта Cell.
        somethingChanged();
    }
}

void Spreadsheet::selectCurrentRow() //Слот selectCurrentRow() соответствует пункту меню Edit | Select | Row.
{
    selectRow(currentRow()); //Здесь используется реализация функции selectRow() класса QTableWidget.
}

void Spreadsheet::selectCurrentColumn() //Слот selectCurrentColumn() соответствует пункту меню Edit | Select | Column.
{
    selectColumn(currentColumn()); //Здесь используется реализация функции selectColumn() класса QTableWidget.
}

void Spreadsheet::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    int row = currentRow();           //Определение позиции ячейки,
    int column = currentColumn() + 1; //которая находится правее от текущей выделенной ячейки.
                                                        //В цикле просматриваются ячейки, начиная с ячейки, расположенной правее курсора,
    while (row < RowCount) {                            //и двигается вправо до достижения последнего столбца.
        while (column < ColumnCount) {                  //Затем процесс идет с первого столбца строки, расположенной ниже
            if (text(row, column).contains(str, cs)) {  //и так продолжается, пока не найдется текст или не будет достигнута последняя ячейка.
                clearSelection();               //Если значение найдено, мы сбрасываем текущее выделение
                setCurrentCell(row, column);    //Перемещаем курсор на ячейку, в которой оно находится
                activateWindow();               //Делаем активным окно, содержащим эту электронную таблицу Spreadsheet
                return;
            }
            ++column;
        }
        column = 0;
        ++row;
    }
    QApplication::beep(); //При неудачном завершении поиска приложение выдает звуковой сигнал.
}

void Spreadsheet::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{                                       //Слот findPrevious() очень похож на findNex, но здесь
    int row = currentRow();
    int column = currentColumn() - 1;

    while (row >= 0) {                  //цикл выполняется в обратном порядке и заканчивается в ячейке A1.
        while (column >= 0) {
            if (text(row, column).contains(str, cs)) {
                clearSelection();
                setCurrentCell(row, column);
                activateWindow();
                return;
            }
            --column;
        }
        column = ColumnCount - 1;
        --row;
    }
    QApplication::beep();
}

void Spreadsheet::recalculate() //Слот recalculate() соответствует пункту меню Tools | Recalculate. Также вызывается в Spreadsheet по мере необходимости.
{
    for (int row = 0; row < RowCount; ++row) {                  //Мы выполняем цикл по всем ячейкам и вызываем функцию Cell::setDirty(),
        for (int column = 0; column < ColumnCount; ++column) {  //которая помечает каждую из них для пересчета значения.
            if (cell(row, column))
                cell(row, column)->setDirty(); //В следующий раз, когда для отображения значения ячейки Сell из таблицы будет вызвана функция text(),
        }                                      //значение этой ячейки будет пересчитано.
    }
    viewport()->update(); //Затем мы вызываем для области отображения функцию update() для перерисовки всей электронной таблицы.
}                         //При этом программный код по перерисовке вызовет функцию text() для каждой видимой ячейки для получения значения.
//Т.к. функция setDirty() вызывалась для каждой ячейки, в вызовах text() будет использовано новое расчитанное значение.
//В этом случае может потребоваться расчет невидимых ячеек. Этот расчет выполняется в классе Cell.

void Spreadsheet::setAutoRecalculate(bool recalc) //Слот setAutoRecalculate() соответствует пунку меню Options | Auto-Recalculate.
{                                                 //Впоследствии функция будет автоматически вызываться из somethingChanged().
    autoRecalc = recalc;
    if (autoRecalc)
        recalculate();
}

/* Реализация технологии "drag and drop" */
void Spreadsheet::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        startPos = event->pos(); //Когда пользователь нажимает левую кнопку мыши, мы сохраняем позицию мыши в закрытой переменной startPos
    QTableWidget::mousePressEvent(event); //Вызываем определенную в классе QTableWidget функцию mousePressEvent()
}                                        // для обработки в QTableWidget обычным образом события нажатия кнопки мыши.
void Spreadsheet::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton) { //Если пользователь перемещает курсор и одновременно держит нажатой левую кнопку, то это начало перетаскивания объекта
        int distance = (event->pos() - startPos).manhattanLength(); //Вычисление позици между текущей позицией и позицией нажатия левой кнопки. manhattanLength() используется для регистрации начала перетаскивания.
        if (distance >= QApplication::startDragDistance()) //Если это расстояние равно или превышает рекомендованное для начала перетаскивания(4 пикселя)
            performDrag(); //Вызывается функция performDrag() для запуска процесса перетаскивания объекта.
    }
    QTableWidget::mouseMoveEvent(event);
}

void Spreadsheet::dragEnterEvent(QDragEnterEvent *event) //Вызывается всякий раз, когда пользователь переносит объект на какой-нибудь виджет
{
    Spreadsheet *source =                                 // QDragEnterEvent::source() возвращает указатель на виджет, который инициирует перенос,
            qobject_cast<Spreadsheet *>(event->source()); // если этот виджет принадлежит тому же самому приложению, в противном случае нулевой указатель
    if (source)                                           // qobject_cast<T>() используется, чтобы убедиться в инициировании переноса виджетом Spreadsheet.
        event->acceptProposedAction();                    // Если все верно, указываем Qt, что пользователь может оптпустить переносимый объект в данном виджете
}

void Spreadsheet::dragMoveEvent(QDragMoveEvent *event) //Код dragMoveEvent() идентичен dragEnterEvent(). Код этой функции необходим,
{                                                      //потому что приходится переопределять реализацию этой функции в классе QTableWidget
    Spreadsheet *source =
            qobject_cast<Spreadsheet *>(event->source());
    if (source)
        event->acceptProposedAction();
}

void Spreadsheet::performDrag()
{
    QString plainText = selectionAsPlainText();
    if (plainText.isEmpty())
        return;

   QMimeData *mimeData = new QMimeData;
   mimeData->setText(plainText); //Обеспечение данных типа text/plain, используя функцию setText()
   mimeData->setHtml(toHtml(plainText)); //Обеспечение данных типа text/plain при помощи setHtml()
   mimeData->setData("text/csv", toCsv(plainText).toUtf8()); //Установка данных типа text/csv при помощи setData(), которая принимает произвольный тип MIME и массив QByteArray

   QDrag *drag = new QDrag(this); //Создание объекта QDrag с указанием this в качестве родительского элемента.
   drag->setMimeData(mimeData); //Объекта QDrag хранит данные в объекте QMimeData
   if (drag->exec(Qt::CopyAction | Qt::MoveAction) == Qt::MoveAction)  //Вызов функции QDrag::exec() запускает операцию перетаскивания объектов и ждет, пока пользователь не отпустит перетаскиваемые объекты или не отменит перетаскивание.
        del();                                                         //В аргументе задается перечень поддерживаемых "операций перетаскивания"; ф-ция возвращает ту операцию, которая была выполнена.
//После вызова этой функции Qt становится владельцем объектов и удалит их, когда они станут не нужны.
}

/* Функции toCsv() и toHtml() преобразуют строку со знаками табуляции и конца строки в форматы CSV и HTML. */
QString Spreadsheet::toCsv(const QString &plainText)
{
    QString result = plainText;
    result.replace("\\", "\\\\");
    result.replace("\"", "\\\"");
    result.replace("\t", "\", \"");
    result.replace("\n", "\"\n\"");
    result.prepend("\"");
    result.append("\"");
    return result;
}

QString Spreadsheet::toHtml(const QString &plainText)
{
    QString result = plainText.toHtmlEscaped();
    result.replace("\t","<td>");
    result.replace("\n", "\n<tr><td>");
    result.prepend("<table>\n<tr><td>");
    result.append("\n</table>");
    return result;
}

void Spreadsheet::dropEvent(QDropEvent *event) //Вызывается, когда пользователь отпускает объект на виджете
{
    Spreadsheet *source =
            qobject_cast<Spreadsheet *>(event->source());
    if (source) {
        event->setDropAction(Qt::CopyAction); //Также необходимо, чтобы данное событие воспринималось как "операция перетаскивания": виджет удалит первоначальную версию элемента.
        event->accept();

        //Определение цели вставки
        QPoint targetPos = event->pos();
        setCurrentCell(rowAt(targetPos.y()), columnAt(targetPos.x()));

        QString plainText = event->mimeData()->text();
        QTableWidgetSelectionRange range = selectedRange();
        QStringList rows = plainText.split('\n'); //Статическая функция QString::split() разбивает строку и представляет ее в виде списка QStringList. Каждая строка таблицы представлена в этом списке одной строкой.
        //Определение размеров области копирования.
        int numRows = rows.count(); // Номер строки таблицы является номером строки QStringList.
        int numColumns = rows.first().count('\t') + 1; // Номер столбца является номером символа табуляции в первой строке плюс 1.

        for (int i = 0; i < numRows; ++i) {                 //При выполнении операции вставки в цикле
            QStringList columns = rows[i].split('\t');      //проходим по строкам и разбиваем каждую строку на значения ячеек, используя QStringList::split().
            for (int j = 0; j < numColumns; ++j) {          //В качестве разделителя используем знак табуляции.
                int row = range.topRow() + i;
                int column = range.leftColumn() + j;
                if (row < RowCount && column < ColumnCount)
                    setFormula(row, column, columns[j]);
            }
        }
        somethingChanged();

    } else
        event->ignore();
}

QString Spreadsheet::selectionAsPlainText()
{
    QTableWidgetSelectionRange range = selectedRange(); //Возвращение выделенного диапазона.
    QString str;

    for (int i = 0; i < range.rowCount(); ++i) { //В цикле обрабатывается вся выделенная область ячеек.
        if (i > 0)                               //Если нет явно выделенной области, то ею будет просто текущая ячейка.
            str += "\n"; //Строки отделяются символом новой строки.
        for (int j = 0; j < range.columnCount(); ++j) {
            if (j > 0)
                str += "\t"; //Столбцы разделяются символом табуляции.
            str += formula(range.topRow() + i, range.leftColumn() + j); //Формула каждой выделенной ячейки добавляется в QString.
        }
    }
    return str;
}
