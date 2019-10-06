/*!
 * \file
 * \brief Заголовочный файл.
 *
 * Данный файл содержит объявление класса, обеспечивающего
 * работу графического пользовательского интерфейса.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> ///< Включение определения класса QMainWindow.

class QAction;
class QAction;
class QActionGroup;
class QLabel;
class QTranslator;
class FindDialog;
class Spreadsheet;

/*!
 * \brief The MainWindow class
 *
 * Класс главного окна приложения, который создается в виде подкласса QMainWindow.
 * Этот класс реализует интерфейс программы с помощью методов:
 *  - отображающих и редактирующих данные в объекте класса Spreadsheet, унаследованного от QTableWidget;
 *  - формирующих главное меню и реализуюoих его функции;
 *  - читающих и сохраняющих файлы, настройки программы;
 *  - реализующих переключение интерфейса программы на другой язык.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief MainWindow
     *
     * Конструктор вызывается при создании объекта.
     */
    MainWindow();

protected:
    /*!
     * \brief closeEvent
     *
     * Виртуальная функция closeEvent() класса QMainWindow
     * вызывается при закрытии окна пользователем.
     * Она переопределяется в MainWindow для того,
     * чтобы сохранить на диске пользовательские настройки.
     * \param event Указатель на изменение состояния в этом событии
     */
    void closeEvent(QCloseEvent *event);
    /*!
     * \brief changeEvent
     *
     * Переопределение базовой функции changeEvent класса QMainWindow
     * для вызова retranslateStrings() при генерации события
     * LanguageChange.
     * \param event Указатель на изменение состояния в этом событии
     */
    void changeEvent(QEvent *event);

signals:
    /*!
     * \brief groupLanguage
     *
     * Сигнал, подаваемый при активации действия языка из меню Language.
     */
    void groupLanguage(QAction *);

private slots:
    /*!
     * \brief newFile
     *
     * Слот newFile() вызывается при выборе пользователем пункта меню File | New
     * или при нажатии кнопки New на панели инструментов.
     */
    void newFile();
    /*!
     * \brief open
     *
     * Функция open() соответствует пункту меню File | Open
     * или при нажатии кнопки Open на панели инструментов.
     * Она вызывает статическую функцию QFileDialog::getOpenFileName(),
     * отображающую диалоговое окно для выбора пользователем имени файла.
     */
    void open();
    /*!
     * \brief save
     *
     * Слот save() соответствует пункту меню File | Save
     * или при нажатии кнопки Save на панели инструментов.
     * \return Если файл уже имеет имя, потому что уже открывался до этого
     * или уже сохранялся, слот save() передает управление функциям
     * saveFile(), задавая это имя; в противном случае вызывается saveAs().
     */
    bool save();
    /*!
     * \brief saveAs
     *
     * Слот saveAs() соответствует пункту меню File | Save As.
     * Вызывает статическую функцию QFileDialog::getSaveFileName()
     * для получения имени файла от пользователя
     * \return возвращает false при нажатии кнопки Cancel
     * \return предает управление функции saveFile(), задавая имя файла.
     */
    bool saveAs();
    /*!
     * \brief find
     *
     * Слот find() вызывается при выборе пользоваетелем пункта меню Edit | Find
     * для вывода на экран диалогового окна Find.
     */
    void find();
    /*!
     * \brief about
     *
     * Диалоговое окно About получается путем вызова удобной статической
     * функции QMessageBox::about().
     */
    void about();
    /*!
     * \brief retranslateStrings
     *
     * В функции MainWindow::retranslateStrings() выполняются почти все вызовы tr()
     * для класса MainWindow. Она вызывается в конце конструктора MainWindow
     * и при каждом изменении пользователем языка приложения в
     * при помощи пункта меню Language.
     */
    void retranslateStrings();
    /*!
     * \brief setMyFont
     *
     * Слот setMyFont() соответствует пункту меню Options | Font.
     */
    void setMyFont();
    /*!
     * \brief setMyColor
     *
     * Слот setMyColor() соответствует пункту меню Options | Color.
     */
    void setMyColor();
    /*!
     * \brief drawPlot
     *
     * Слот drawPlot() вызывается при выборе пользователем пункта меню Plot.
     * Подготавливает данные для осей координат и выводит
     * диалоговое окно графика на экран.
     */
    void drawPlot();
    /*!
     * \brief openRecentFile
     *
     * Слот openRecentFile() вызывается при выборе пользователем какого-нибудь
     * недавно используемого файла.
     */
    void openRecentFile();
    /*!
     * \brief updateStatusBar
     *
     * Слот updateStatusBar() обновляет индикаторы расположения ячейки и формулы ячейки.
     * Он вызывается при любом перемещении пользователем курсора ячейки на новую ячейку.
     */
    void updateStatusBar();
    /*!
     * \brief spreadsheetModified
     *
     * Слот spreadsheetModified() устанавливает свойство windowModified в значение true,
     * обновляя строку заголовка. Эта функция обновляет также индикаторы расположения
     * и формулы ячейки, чтобы они отражали текущее состояние.
     */
    void spreadsheetModified();
    /*!
     * \brief setLanguage
     *
     * Слот, вызываемый при выборе пользователем действия смены языка.
     * \param language Переменная, принимающая действие, устанавливающее язык.
     */
    void setLanguage(QAction *language);

private:
    /*!
     * \brief createActions
     *
     * Закрытая функция создает объекты QAction для элементов меню.
     * Текст пунктов меню устанавливается в retranslateStrings().
     */
    void createActions();
    /*!
     * \brief createMenus
     *
     * Закрытая функция создает пункты меню.
     */
    void createMenus();
    /*!
     * \brief createContextMenu
     *
     * Для обеспечения в приложении контекстного меню добавляются
     * действия Cut, Copy, Paste, Add Row, Remove Row в виджет Spreadsheet и устанавливается
     * политика контекстного меню виджета на отображение контекстного меню
     * с этими действиями.
     */
    void createContextMenu();
    /*!
     * \brief createToolBars
     *
     * Закрытая функция создает панель инструментов.
     */
    void createToolBars();
    /*!
     * \brief createStatusBar
     *
     * Функция createStatusBar() вызывается для создания строки состояния:
     * - положение текущей ячейки;
     * - следом формула текущей ячейки;
     * - вывод временных сообщений.
     */
    void createStatusBar();
    /*!
     * \brief setLanguage
     *
     * Слот, вызываемый при выборе пользователем действия смены языка.
     * \param language Переменная, принимающая действие, устанавливающее язык.
     */
    void createLanguageMenu();
    /*!
     * \brief readSettings
     *
     * Функция readSettings() загружает настройки,
     * которые были сохранены функцией writeSettings().
     */
    void readSettings();
    /*!
     * \brief writeSettings
     *
     * Функция writeSettings() используется для сохранения настроек.
     */
    void writeSettings();
    /*!
     * \brief okToContinue
     *
     * Закрытая функция okToContinue() проверяется свойство windowModified.
     * Если оно имеет значение true, выводится диалоговое окно сохранения
     * изменений.
     * \return возвращает true, если пользователь отвечает Yes или No.
     * \retrun возвращает false, если пользователь отвечает Cancel.
     */
    bool okToContinue();
    /*!
     * \brief loadFile
     *
     * Закрытая функция loadFile() вызывается в open() для загрузки файла.
     * Функция независима, поскольку требуется выполнить те же действия для
     * загрузки файлов, которые открывались недавно.
     * \param fileName Ссылка на имя файла
     * \return возвращает true, если файл не загрузился
     * \return возвращает false, если файл успешно загрузился
     */
    bool loadFile(const QString &fileName);
    /*!
     * \brief saveFile
     *
     * Закрытая функция saveFile() вызывается в save() или saveAs()
     * для записи файла на диск.
     * \param fileName Ссылка на имя файла
     * \return возвращает true, если файл не загрузился
     * \return возвращает false, если файл успешно загрузился
     */
    bool saveFile(const QString &fileName);
    /*!
     * \brief setCurrentFile
     *
     * Закрытая функция setCurrentFile() устанавливает закрытую переменную curFile,
     * обновляет список недавно используемых файлов, изменяет заголовок окна.
     * \param fileName Ссылка на имя файла
     */
    void setCurrentFile(const QString &fileName);
    /*!
     * \brief updateRecentFileActions
     *
     * Закрытая функция updateRecentFileActions() вызывается для обновления
     * пунктов меню File.
     */
    void updateRecentFileActions();
    /*!
     * \brief strippedName
     *
     * Закрытая функция strippedName() вызывается в setCurrentFile()
     * перед отображением имени файла в заголовке.
     * Функция убирает путь к файлу, чтобы имя файла выглядело более привлекательно.
     * \param fullFileName Ссылка на имя файла
     * \return возвращает имя файла без пути к нему
     */
    QString strippedName(const QString &fullFileName);

    Spreadsheet *spreadsheet; ///< Указатель на объект Spreadsheet
    FindDialog *findDialog; ///< Указатель на объект FindDialog
    QLabel *locationLabel;
    QLabel *formuLabel;

    QString curFile; ///< Содержит имя редактируемого файла
    QString qmPath; ///< Путь к подготовленым файлам перевода
    //! Объекты обеспечивающие перевод при отображении текста
    QTranslator *appTranslator; ///< appTranslator хранит текущий перевод предложения
    QTranslator *qtTranslator; ///< qtTranslator хранит перевод библиотеки Qt


    enum { MaxRecentFiles = 5 };
    QAction *recentFileActions[MaxRecentFiles]; ///< Массив пунктов меню недавних файлов
    QAction *separatorAction;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *plotMenu;
    QMenu *selectSubMenu;
    QMenu *languageMenu;
    QMenu *toolsMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *closeAction;

    QActionGroup *actionGroupLanguage; ///< Указатель на объект QActionGroup (группы меню)
    QAction *actionRussian;
    QAction *actionEnglish;

    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;
    QAction *addRowAction;
    QAction *removeRowAction;
    QAction *selectRowAction;
    QAction *selectColumnAction;
    QAction *selectAllAction;
    QAction *findAction;
    QAction *recalculateAction;
    QAction *showGridAction;
    QAction *autoRecalcAction;
    QAction *colorAction;
    QAction *fontAction;
    QAction *drawPlotAction;
    QAction *aboutAction;
    /*! \details Для действия About Qt используется слот aboutQt() объекта
     * QApplication, который доступен через глобальную переменную qApp.
     */
    QAction *aboutQtAction;
};

#endif // MAINWINDOW_H
