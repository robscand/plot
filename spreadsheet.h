/*!
 * \file
 * \brief Заголовочный файл.
 *
 * Данный файл содержит объявление класса центрального виджета
 * главного окна приложений Spreadsheet - электронная таблица.
 */
#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QTableWidget> ///< Включение определения класса QTableWidget.

class Cell;
/*!
 * \brief The Spreadsheet class
 *
 * Класс Spreadsheet является подклассом QTableWidget, который поддерживает формулы электронной таблицы,
 * а также является основным инструментом обработки и представления информации в программе.
 */
class Spreadsheet : public QTableWidget
{
    Q_OBJECT

public:
    /*!
     * \brief FindDialog
     *
     * Конструктор с параметром. Вызывается при создании объекта.
     * \param parent Указатель на родительский класс типа QWidget.
     */
    Spreadsheet(QWidget *parent = 0);

    QTableWidgetItem *item0,*item1, *item2, *item3, *item4, *item5, *item6, *item7; ///< Указатели на элементы заголовков электронной таблицы
    /*!
     * \brief autoRecalculate
     *
     * Реализуется как встроенная - inline, поскольку показывает,
     * задействован режим перерасчета или нет
     * \return Режим пересчета.
     */
    bool autoRecalculate() const { return autoRecalc; }
    /*!
     * \brief currentLocation
     *
     * //Функция currentLocation() возвращает текущее положение ячейки.
     * \return Текущее положение ячейки.
     */
    QString currentLocation() const;
    /*!
     * \brief currentFormula
     *
     * //Функция currentFormula() возвращает формулу текущей ячейки.
     * \return Управление передается функции formula() для перобразования
     * положения ячейки в формулу.
     */
    QString currentFormula() const;
    /*!
     * \brief selectedRange
     *
     * Функция selectedRange() возвращает выделенный диапазон.
     * Выделяется не более одного диапазона, так как в конструкторе
     * указан режим выделения ContiguousSelection.
     * \return Ссылка на первый элемент выделенного диапазона.
     */
    QTableWidgetSelectionRange selectedRange() const;
    /*!
     * \brief selectionAsPlainText
     *
     * Функция selectionAsPlainText() обрабатывает выделенный диапазон,
     * преобразуя его в простую строку.
     * \return Преобразованная строка.
     */
    QString selectionAsPlainText();
    /*!
     * \brief clear
     *
     * Функция clear() служит для инициализации электронной таблицы, задания заголовков столбцов.
     * Вызывается из конструктора Spreadsheet и из MainWindow::newFile().
     */
    void clear();
    /*!
     * \brief retranslateStrings
     *
     * Функция Spreadsheet::retranslateStrings() выполняет перевод заголовков столбцов.
     * Она вызывается в конце конструктора Spreadsheet
     * и при каждом изменении пользователем языка приложения в MainWindow
     * при помощи пункта меню Language.
     */
    void retranslateStrings();
    /*!
     * \brief readFile
     *
     * Функция readFile() вызывается из MainWindow::loadFile() для чтения данных из файла.
     * \param fileName Имя файла, из которого выполняется чтение.
     * \return true при успешном чтении.
     * \return false при ошибке.
     */
    bool readFile(const QString &fileName);
    /*!
     * \brief writeFile
     *
     *  //Функция writeFile() вызывается из MainWindow::saveFile() для записи файла на диск.
     * \param fileName Имя сохраняемого файла.
     * \return true при успешной записи.
     * \return false при ошибке.
     */
    bool writeFile(const QString &fileName);

/// Класс Spreadsheet содержит множество слотов, которые реализуют действия пунктов меню Edit, Tools и Options
public slots:
    /*!
     * \brief cut
     *
     * Слот cut() соответствует пункту меню Edit | Cut.
     */
    void cut();
    /*!
     * \brief copy
     *
     * Слот copy() соответствует пункту меню Edit | Copy.
     */
    void copy();
    /*!
     * \brief paste
     *
     * Слот paste() соответствует пункту меню Edit | Paste.
     */
    void paste();
    /*!
     * \brief del
     *
     * Слот del() соответствует пункту меню Edit | Delete.
     */
    void del();
    /*!
     * \brief addRow
     *
     * Слот addRow() соответствует пункту меню Edit | Add Row.
     */
    void addRow();
    /*!
     * \brief removeRow
     *
     * Слот removeRow() соответствует пункту меню Edit | Remove Row.
     */
    void removeRow();
    /*!
     * \brief selectCurrentRow
     *
     * Слот selectCurrentRow() соответствует пункту меню Edit | Select | Row.
     */
    void selectCurrentRow();
    /*!
     * \brief selectCurrentColumn
     *
     * Слот selectCurrentColumn() соответствует пункту меню Edit | Select | Column.
     */
    void selectCurrentColumn();
    /*!
     * \brief recalculate
     *
     * Слот recalculate() соответствует пункту меню Tools | Recalculate.
     * Также вызывается в Spreadsheet по мере необходимости.
     * Выполняет перерисовку электронной таблицы с данными.
     */
    void recalculate();
    /*!
     * \brief setAutoRecalculate
     *
     * Слот setAutoRecalculate() соответствует пунку меню Options | Auto-Recalculate.
     * Впоследствии функция будет автоматически вызываться из somethingChanged().
     * \param recalc Переменная, отвечающая за установку автоматического пересчета.
     */
    void setAutoRecalculate(bool recalc);
    /*!
     * \brief findNext
     *
     * Слот findNext() в цикле просматривает ячейки, начиная с ячейки, расположенной правее курсора,
     * и двигается вправо до достижения последнего столбца; затем процесс идет с первого столбца строки,
     * расположенной ниже, и так продолжается, пока не будет найден требуемый текст или пока не будет
     * достигнута самая последняя ячейка.
     * \param str текст, считываемый из строки редактирования.
     * \param cs переменная перечисления Qt::CaseSensitivity
     * и принимает значение Qt::CaseSensitive или Qt::CaseInsensitive.
     */
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    /*!
     * \brief findPrevious
     *
     * Слот findPrevious() в цикле просматривает ячейки, начиная с ячейки, расположенной левее курсора,
     * и двигается влево до достижения первого столбца; затем процесс идет с первого столбца строки,
     * расположенной выше, и так продолжается, пока не будет найден требуемый текст или пока не будет
     * достигнута самая первая ячейка.
     * \param str текст, считываемый из строки редактирования.
     * \param cs переменная перечисления Qt::CaseSensitivity
     * и принимает значение Qt::CaseSensitive или Qt::CaseInsensitive.
     */
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);

signals:
    void modified(); ///< Сигнал для уведомления о возникновении любого изменения

private slots:
    /*!
     * \brief somethingChanged
     *
     * Закрытый слот somethingChanged() делает пересчет всей электронной таблицы,
     * если включен режим Auto-Recalcualate. Также он генерирует сигнал modified().
     */
    void somethingChanged();

protected:
    /// Spreadsheet переопределяет 6 обработчиков событий, которые объявлены в QWidget
    /*!
     * \brief mousePressEvent
     *
     * Переопределение обработчика события нажатия указателя мыши.
     * Функция сохраняет позицию мыши в закрытой переменной startPos.
     * \param event Указатель на изменение состояния в этом событии.
     */
    void mousePressEvent(QMouseEvent *event);
    /*!
     * \brief mouseMoveEvent
     *
     * Переопределение обработчика события перемещения указателя мыши.
     * Функция вычисляет расстояние между текущей позицией и позицией нажатия левой кнопки.
     * \param event Указатель на изменение состояния в этом событии.
     */
    void mouseMoveEvent(QMouseEvent *event);
    /*!
     * \brief dragEnterEvent
     *
     * Переопределение обработчика события переноса объекта на какой-нибудь виджет.
     * Осуществляет проверку на инициирование переноса виджетом Spreadsheet.
     * \param event Указатель на изменение состояния в этом событии.
     */
    void dragEnterEvent(QDragEnterEvent *event);
    /*!
     * \brief dragMoveEvent
     *
     * Переопределение обработчика события перемещения объекта над каким-либо виджетом.
     * Осуществляет проверку на инициирование переноса виджетом Spreadsheet.
     * \param event Указатель на изменение состояния в этом событии.
     */
    void dragMoveEvent(QDragMoveEvent *event);
    /*!
     * \brief dropEvent
     *
     * Переопределение обработчика события отпускания пользователем объекта на виджете.
     * Функция обеспечивает правильный прием объекта после его опускания.
     * \param event
     */
    void dropEvent(QDropEvent *event);
    /*!
     * \brief changeEvent
     *
     * Переопределение базовой виртуальной функции changeEvent класса QTableWidget
     * для вызова retranslateStrings() при генерации события
     * LanguageChange.
     * \param event Указатель на изменение состояния в этом событии
     */
    void changeEvent(QEvent *event);

private:
    /*!
     * Перечислимое значение для задания первоначальных размеров таблицы,
     * создания типового номера.
     */
    enum { MagicNumber = 0x7F51C883, ///< "Волшебное число" определено в spreadsheet.h как 0x7F51C833 - произвольное случайное число.
           RowCount = 25, ///< Число строк в таблице.
           ColumnCount = 8 ///< Число колонок в таблице.
         };
    /*!
     * \brief cell
     *
     * Закрытая функция cell() возвращает для заданной строки и столбца указатель на объект Cell.
     * \param row Номер строки.
     * \param column Номер столбца.
     * \return Указатель на объект Cell.
     */
    Cell *cell(int row, int column) const;
    /*!
     * \brief text
     *
     * Закрытая функция text() возвращает текст или формулу (если текст совпадает с формулой) заданной ячейки.
     * \param row Номер строки.
     * \param column Номер столбца.
     * \return Текст или формула.
     */
    QString text(int row, int column) const;
    /*!
     * \brief formula
     *
     * Закрытая функция formula() возвращает формулу ячейки.
     * \param row Номер стркои.
     * \param column Номер столбца.
     * \return Формула ячейки.
     */
    QString formula(int row, int column) const;
    /*!
     * \brief setFormula
     *
     * Закрытая функция setFormula задает формулу для указанной ячейки.
     * \param row Номер строки.
     * \param column Номер столбца.
     * \param formula Формула ячейки.
     */
    void setFormula(int row, int column, const QString &formula);

    QPoint startPos; ///< Стартовая позиция кнопки мыши при перетаскивании.
    /*!
     * \brief toCsv
     *
     * Функция toCsv() преобразует строку со знаками табуляции и конца строки в формат CSV.
     * \param plainText простой текст для форматирования.
     * \return Отформатированный текст.
     */
    static QString toCsv(const QString &plainText);
    /*!
     * \brief toHtml
     *
     * Функции toHtml() преобразует строку со знаками табуляции и конца строки в формат HTML.
     * \param plainText простой текст для форматирования.
     * \return Отформатированный текст.
     */
    static QString toHtml(const QString &plainText);
    /*!
     * \brief performDrag
     *
     * Закрытая функция performDrag() вызывается из mouseMoveEvent() для инициирования
     * переноса выделенной прямоугольной области, а также для установки типов MIME.
     */
    void performDrag();

    bool autoRecalc;
};

#endif // SPREADSHEET_H
