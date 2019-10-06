/*!
 * \file
 * \brief Исполняемый файл.
 *
 * В файле строится каркас графического пользовательского интерфейса приложения,
 * который имеет меню, панели инструментов и строку состояния;
 * имеет необходимые диалоговые окна для обработки главного виджета - Spreadsheet.
 * Основная часть кода связана с созданием пользовательского интерфейса
 * и настройки соответствующих соединений сигнал-слот.
 */
#include <QtGui>
#include <QApplication>
#include <QLabel>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QActionGroup>
#include <QTranslator>
#include <QMdiArea>
#include <QColor>
#include <QColorDialog>
#include <QFont>
#include <QFontDialog>

#include "mainwindow.h"
#include "spreadsheet.h"
#include "plot.h"
#include "finddialog.h"

static QStringList recentFiles;

MainWindow::MainWindow()
{
    spreadsheet = new Spreadsheet;
    setCentralWidget(spreadsheet);

    /* Создание системы меню */
    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

    readSettings();
    findDialog = 0;

    // Установка двух объектов QTranslator на объект QApplication.
    qmPath = qApp->applicationDirPath() + "/tr";
    appTranslator = new QTranslator;
    qtTranslator = new QTranslator;

    // устанавливаем атрибут Qt::WA_DeleteOnClose для предотвращения утечек памяти
    // при закрытии пользователем окна.
    // Приводит к полному разрушению окна при закрытии
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowIcon(QIcon(":/images/icon.png"));
    setCurrentFile("");

    retranslateStrings();
}

void MainWindow::createActions()
{
    newAction = new QAction(this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(this);
    openAction->setIcon(QIcon(":/images/open.png"));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(this);
    saveAction->setIcon(QIcon(":/images/save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    for(int i = 0; i < MaxRecentFiles; i++) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    closeAction = new QAction(this);
    closeAction->setShortcut(QKeySequence::Close);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

    exitAction = new QAction(this);
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    cutAction = new QAction(this);
    cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, SIGNAL(triggered()), spreadsheet, SLOT(cut()));

    copyAction = new QAction(this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered()), spreadsheet, SLOT(copy()));

    pasteAction = new QAction(this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered()), spreadsheet, SLOT(paste()));

    deleteAction = new QAction(this);
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, SIGNAL(triggered()), spreadsheet, SLOT(del()));

    addRowAction = new QAction(this);
    addRowAction->setIcon(QIcon(":/images/plus.png"));
    addRowAction->setShortcut(QKeySequence::AddTab);
    connect(addRowAction, SIGNAL(triggered()), spreadsheet, SLOT(addRow()));

    removeRowAction = new QAction(this);
    removeRowAction->setIcon(QIcon(":/images/minus.png"));
    removeRowAction->setShortcut(QKeySequence::Back);
    connect(removeRowAction, SIGNAL(triggered()), spreadsheet, SLOT(removeRow()));

    selectRowAction = new QAction(this);
    connect(selectRowAction, SIGNAL(triggered()), spreadsheet, SLOT(selectCurrentRow()));

    selectColumnAction = new QAction(this);
    connect(selectColumnAction, SIGNAL(triggered()), spreadsheet, SLOT(selectCurrentColumn()));

    selectAllAction = new QAction(this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, SIGNAL(triggered()), spreadsheet, SLOT(selectAll()));

    findAction = new QAction(this);
    findAction->setIcon(QIcon(":/images/find.png"));
    findAction->setShortcut(QKeySequence::Find);
    connect(findAction, SIGNAL(triggered()), this, SLOT(find()));

    recalculateAction = new QAction(this);
    connect(recalculateAction, SIGNAL(triggered()), spreadsheet, SLOT(recalculate()));

    showGridAction = new QAction(this);
    showGridAction->setCheckable(true);
    showGridAction->setChecked(spreadsheet->showGrid());
    connect(showGridAction, SIGNAL(toggled(bool)), spreadsheet, SLOT(setShowGrid(bool)));

    fontAction = new QAction(this);
    connect(fontAction, SIGNAL(triggered()), this, SLOT(setMyFont()));

    colorAction = new QAction(this);
    connect(colorAction, SIGNAL(triggered()), this, SLOT(setMyColor()));

    autoRecalcAction = new QAction(this);
    autoRecalcAction->setCheckable(true);
    autoRecalcAction->setChecked(spreadsheet->autoRecalculate());
    connect(autoRecalcAction, SIGNAL(toggled(bool)), spreadsheet, SLOT(setAutoRecalculate(bool)));

    aboutAction = new QAction(this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    drawPlotAction = new QAction(this);
    drawPlotAction->setIcon(QIcon(":/images/plot.png"));
    connect(drawPlotAction, SIGNAL(triggered()), this, SLOT(drawPlot()));

    actionGroupLanguage = new QActionGroup(this);
    connect(actionGroupLanguage, SIGNAL(triggered(QAction*)), SIGNAL(groupLanguage(QAction*)));
    connect(this, SIGNAL(groupLanguage(QAction*)), SLOT(setLanguage(QAction*)));

    actionRussian = new QAction(actionGroupLanguage);
    actionRussian->setCheckable(true);

    actionEnglish = new QAction(actionGroupLanguage);
    actionEnglish->setCheckable(true);
    actionEnglish->setChecked(true);

}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(this);
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    separatorAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    menuBar()->addMenu(fileMenu);

    editMenu = new QMenu(this);
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);
    editMenu->addAction(addRowAction);
    editMenu->addAction(removeRowAction);

    selectSubMenu =  new QMenu(this);
    selectSubMenu->addAction(selectRowAction);
    selectSubMenu->addAction(selectColumnAction);
    selectSubMenu->addAction(selectAllAction);

    editMenu->addMenu(selectSubMenu);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
    menuBar()->addMenu(editMenu);

    languageMenu = new QMenu(this);
    languageMenu->addActions(actionGroupLanguage->actions());
    menuBar()->addMenu(languageMenu);

    plotMenu = new QMenu(this);
    plotMenu->addAction(drawPlotAction);
    menuBar()->addMenu(plotMenu);

    toolsMenu = new QMenu(this);
    toolsMenu->addAction(recalculateAction);
    menuBar()->addMenu(toolsMenu);

    optionsMenu = new QMenu(this);
    optionsMenu->addAction(fontAction);
    optionsMenu->addAction(colorAction);
    optionsMenu->addSeparator();
    optionsMenu->addAction(showGridAction);
    optionsMenu->addAction(autoRecalcAction);
    menuBar()->addMenu(optionsMenu);

    menuBar()->addSeparator();

    helpMenu = new QMenu(this);
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::createContextMenu()
{
    spreadsheet->addAction(cutAction);
    spreadsheet->addAction(copyAction);
    spreadsheet->addAction(pasteAction);
    spreadsheet->addAction(addRowAction);
    spreadsheet->addAction(removeRowAction);
    spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    fileToolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, fileToolBar);
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    editToolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, editToolBar);
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addAction(addRowAction);
    editToolBar->addAction(removeRowAction);
    editToolBar->addSeparator();
    editToolBar->addAction(findAction);
    editToolBar->addAction(drawPlotAction);
}

void MainWindow::createStatusBar()
{
    locationLabel = new QLabel("W999");
    locationLabel->setAlignment(Qt::AlignCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formuLabel = new QLabel;
    formuLabel->setIndent(3);
    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formuLabel, 1);

    connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(updateStatusBar()));
    connect(spreadsheet, SIGNAL(modified()), this, SLOT(spreadsheetModified()));

    updateStatusBar();
}

/* В функции выполняются все вызовы tr() для класса MainWindow и при каждом изменении пользователем языка приложения при помощи меню Language */
void MainWindow::retranslateStrings()
{
    /* Для каждого пункта меню QAction устанавливаем текст */
    newAction->setText(tr("&New"));
    /* Установка комментария в строке состояния */
    newAction->setStatusTip(tr("Create a new spreedwheet file"));

    openAction->setText(tr("&Open"));
    openAction->setStatusTip(tr("Open an existing spreadsheet file"));

    saveAction->setText(tr("&Save"));
    saveAction->setStatusTip(tr("Save the spreadsheet to disk"));

    saveAsAction->setText(tr("Save As..."));
    saveAsAction->setStatusTip(tr("Save the spreadsheet under a new "
                                  "name"));

    closeAction->setText(tr("&Close"));
    closeAction->setStatusTip(tr("Close this window"));

    exitAction->setText(tr("E&xit"));
    /* Установка клавиши быстрого вызова. Используется для команд, у которых нет стандартных клавиш быстрого вызова. */
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));

    cutAction->setText(tr("Cu&t"));
    cutAction->setStatusTip(tr("Cut the current selection's contents "
                               "to the clipboard"));

    copyAction->setText(tr("&Copy"));
    copyAction->setStatusTip(tr("Copy the current selection's contents "
                                "to the clipboard"));

    pasteAction->setText(tr("&Paste"));
    pasteAction->setStatusTip(tr("Paste the clipboard's contents into "
                                 "the current selection"));

    deleteAction->setText(tr("&Delete"));
    deleteAction->setStatusTip(tr("Delete the current selection's "
                                  "contents"));

    addRowAction->setText(tr("&Add Row"));
    addRowAction->setStatusTip(tr("Add a new row to the spreadsheet."));

    removeRowAction->setText(tr("&Remove Row"));
    removeRowAction->setStatusTip(tr("Remove row from the spreadsheet."));

    selectRowAction->setText(tr("&Row"));
    selectRowAction->setStatusTip(tr("Select all the cells in the "
                                  "current row"));

    selectColumnAction->setText(tr("&Column"));
    selectColumnAction->setStatusTip(tr("Select all the cells in the "
                                        "current column"));

    selectAllAction->setText(tr("&All"));
    selectAllAction->setStatusTip(tr("Select all the cells in the"
                                     "spreadsheet"));

    findAction->setText(tr("&Find..."));
    findAction->setStatusTip(tr("Find a matching cell"));

    recalculateAction->setText(tr("&Recalculate"));
    recalculateAction->setShortcut(tr("F9"));
    recalculateAction->setStatusTip(tr("Recalculate all the "
                                       "spreadsheet's formulas"));

    showGridAction->setText(tr("&Show Grid"));
    showGridAction->setStatusTip(tr("Show or hide the spreadsheet's "
                                    "grid"));

    fontAction->setText(tr("&Font"));
    fontAction->setStatusTip(tr("Change the color of the "
                                "program interface"));

    colorAction->setText(tr("&Color"));
    colorAction->setStatusTip(tr("Change the color of the "
                                "program interface"));

    autoRecalcAction->setText(tr("&Auto-Recalculate"));
    autoRecalcAction->setStatusTip(tr("Switch auto-recalculation on or "
                                      "off"));

    aboutAction->setText(tr("&About"));
    aboutAction->setStatusTip(tr("Show the application's About box"));

    aboutQtAction->setText(tr("About &Qt"));
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));

    drawPlotAction->setText(tr("&Plot"));
    drawPlotAction->setStatusTip(tr("Draw a plot of the dependency of "
                                    "specific data"));

    actionRussian->setText(tr("Russian"));
    actionEnglish->setText(tr("English"));

    /* Установка заголовков для окон и QMenu и QToolBar. */
    fileMenu->setTitle(tr("&File"));
    editMenu->setTitle(tr("&Edit"));
    selectSubMenu->setTitle(tr("&Select"));
    plotMenu->setTitle(tr("Plot"));
    languageMenu->setTitle(tr("Language"));
    toolsMenu->setTitle(tr("&Tools"));
    optionsMenu->setTitle(tr("&Options"));
    helpMenu->setTitle(tr("&Help"));
    fileToolBar->setWindowTitle(tr("&File"));
    editToolBar->setWindowTitle(tr("&Edit"));
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        retranslateStrings();
    QMainWindow::changeEvent(event);
}

void MainWindow::setLanguage(QAction *language)
{
    qtTranslator->load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(qtTranslator);

    if(language->text() == tr("Russian")) {
        appTranslator->load("lang_ru", qmPath);

        qDebug() << qmPath;
        qApp->installTranslator(appTranslator); //appTranslator хранит текущий перевод предложения

        retranslateStrings();
        qDebug() << language->text();
    }

    if(language->text() == tr("English")) {
        appTranslator->load("lang_en", qmPath);
        qApp->installTranslator(appTranslator); //qtTranslator хранит перевод библиотеки Qt

        retranslateStrings();
        qDebug() << language->text();
    }

}

void MainWindow::updateStatusBar()
{
    locationLabel->setText(spreadsheet->currentLocation()); //функция Spreadsheet::currentLocation() используется для отображения положения ячейки в строке состояния
    formuLabel->setText(spreadsheet->currentFormula()); //функция Spreadsheet::currentFormula() используется для отображения формулы ячейки в строке состояния
}

void MainWindow::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}

void MainWindow::newFile()
{
    MainWindow *win = new MainWindow;
    win->show();
}

bool MainWindow::okToContinue()
{
    if(isWindowModified()){
        int r = QMessageBox::warning(this, tr("Spreadsheet"),
                                     tr("The document has been modified.\n"
                                        "Do you want to save your changes?"),
                                     QMessageBox::Yes | QMessageBox::No |
                                     QMessageBox::Cancel);
        if (r == QMessageBox::Yes){
            return save();
        } else if (r == QMessageBox::Cancel){
            return false;
        }
    }
    return true;
}

void MainWindow::open()
{
    if(okToContinue()){
        QString fileName = QFileDialog::getOpenFileName(this,                           //getOpenFileName - статическая функция для получения имени файла
                                                        tr("Open Spreadsheet"),".",
                                                        tr("Spreadsheet files (*.sp)"));
        if(!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::loadFile(const QString &fileName) //Закрытая функция loadFile вызвана в open() для загрузки файла. Используется для файлов, которые открывались недавно.
{
    if(!spreadsheet->readFile(fileName)){                       //Функция readFile предназначена для чтения файла с диска.
        statusBar()->showMessage(tr("Loading canceled"), 2000);
        return false;
    }
    setCurrentFile(fileName); //Если загрузка успешна - вызов setCurrentFile для обновления заголовка окна
    statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

bool MainWindow::save()
{
    if(curFile.isEmpty()){
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveFile(const QString &fileName)
{
    if(!spreadsheet->writeFile(fileName)) { //Функция Spreadsheet::writeFile() реализует запись файла на диск.
        statusBar()->showMessage(tr("Saving canceled"), 2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File %1 saved").arg(fileName), 2000);
    return true;
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Spreadsheet"), ".",
                                                    tr("Spreadsheet files (*.sp)"));
    if(fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString showName = "Untitled";
    if (!curFile.isEmpty()) {
        showName = strippedName(curFile);
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);

        foreach (QWidget *win, QApplication::topLevelWidgets()) {
            if (MainWindow *mainWin = qobject_cast<MainWindow *>(win))
                mainWin->updateRecentFileActions();
        }
    }

    setWindowTitle(tr("%1[*] - %2").arg(showName)
                                   .arg(tr("Spreadsheet")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while (i.hasNext()) {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    for (int j = 0; j < MaxRecentFiles; j++) {
        if (j < recentFiles.count()) {
            QString text = tr("&%1 %2")
                           .arg(j + 1)
                           .arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        } else {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

void MainWindow::openRecentFile()
{
    if (okToContinue()) {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            loadFile(action->data().toString());
    }
}

void MainWindow::find()
{
    if(!findDialog) {
        findDialog = new FindDialog(this);
        connect(findDialog, SIGNAL(findNext(const QString &, Qt::CaseSensitivity)),
                spreadsheet, SLOT(findNext(const QString &, Qt::CaseSensitivity)));
        connect(findDialog, SIGNAL(findPrevious(const QString &, Qt::CaseSensitivity)),
                spreadsheet, SLOT(findPrevious(const QString &, Qt::CaseSensitivity)));
    }

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}

void MainWindow::setMyColor() //Слот setColor() соответствует пункту меню Options | Color.
{
    QColor myColor = QColorDialog::getColor(Qt::white, this, tr("Choose color")); // Вызывается диалоговое окно выбора нового цвета.
    if (myColor.isValid()) {
        setPalette(QPalette(myColor));
    }
}

void MainWindow::setMyFont() //Слот соответствует пункту меню Options | Font.
{
    bool bOk;
    QString tmp = tr("Choose font");
    QFont myFont = QFontDialog::getFont(&bOk, QFont("Segoe Print", 10), //Вызывается диалоговое окно выбора нового шрифта.
                                      this, tmp);
    if(bOk) {
        this->setFont(myFont);
        spreadsheet->setFont(myFont);
    }
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("About Spreadsheet"),
                       tr("<h2>Spreadsheet 1.1</h2>"
                          "<p>Copyright &copy; 2017 MGSU ISTAS 2-2. "
                          "Creator - Kravtsov Leonid Sergeevich. "
                          "Variant - 8."
                          "<p>Program in C ++ / Qt with GUI on the account "
                          "of the PC device \"Plotter\"."
                          "<p>This is a useful application that has "
                          "a complete graphical user interface for the application, "
                          "menus, toolbars, status bar and necessary dialog boxes. "
                          "<p>Spreadsheet is demonstrates QAction, QMainWindow, QMenuBar, "
                          "QStatusBar, QTableWidget, QToolBar, and many other "
                          "Qt classes. "));
}

void MainWindow::drawPlot()
{
    QVector<QString> plotData;
    for (int i = 0; i < spreadsheet->rowCount(); ++i) {
        QTableWidgetItem *item = spreadsheet->item(i, 3);
        if(!item)
            continue;
        int id = item->text().toInt();
        switch(id) {
            case 1: plotData.push_back(tr("Пузырьково-струйная")); break;
            case 2: plotData.push_back(tr("Термальная струйная")); break;
            case 3: plotData.push_back(tr("Микропьезоструйная")); break;
            case 4: plotData.push_back(tr("Пьезоэлектрическая")); break;
            case 5: plotData.push_back(tr("НР Scalable Printing Technology")); break;
        }
    }
    Plot p(this);
    p.setGraphData(plotData);
    p.exec();
    p.setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::writeSettings() //Функция writeSettings() используется для сохранения настроек
{
    /*QSettings cохраняет настройки в системном реестре (для Windows).
     * QSettings хранит настройки в виде пары "ключ-значение".
     * Здесь ключ подобен пути файловой системы */
    QSettings settings("ISTAS of MGSU", "Spreadsheet"); //В конструктор передается название учебного заведения и имя приложения. Эта информация используется для определения местоположения настроек.
    settings.setValue("geometry", saveGeometry());      //Далее создаем подключи и помещаем в них настройки.
    settings.setValue("recentFiles", recentFiles);

    QColor myColor = palette().background().color();
    settings.setValue("myColor", myColor);

    QFont myFont = font();
    settings.setValue("myFont", myFont);

    settings.setValue("showGrid", showGridAction->isChecked());
    settings.setValue("autoRecalc", autoRecalcAction->isChecked());
}

void MainWindow::readSettings() //Функция readSettings() загружает настройки, которые были сохранены функцией writeSettings().
{
    QSettings settings("ISTAS of MGSU", "Spreadsheet");
    //Второй аргумент функции value() определяет значение, принимаемое по умолчанию в случае отсутствия запрашиваемого параметра.
    //Значения по умолчанию будут использованы при первом запуске приложения.
    restoreGeometry(settings.value("geometry").toByteArray()); //Второй агрумент не задан - окно произвольного размера.
    recentFiles = settings.value("recentFiles").toStringList(); //Второй аргумент не задан - список недавно использованных файлов будет пуст.

    QColor myColor = settings.value("myColor", QColor(Qt::white)).value<QColor>();
    setPalette(QPalette(myColor));

    QFont myFont = settings.value("myFont").value<QFont>();
    setFont(myFont);
    spreadsheet->setFont(myFont);

    foreach (QWidget *win, QApplication::topLevelWidgets()) {       //foreach() используется для прохода по всем имеющимся в приложении виджетам.
        if (MainWindow *mainWin = qobject_cast<MainWindow *>(win))
            mainWin->updateRecentFileActions(); //Делается вызов функции updateRecentFileActions для всех виджетов типа MainWindow.
      }

    //Для одного виджета.
    //updateRecentFileActions();

    bool showGrid = settings.value("showGrid", true).toBool();
    showGridAction->setChecked(showGrid);
    bool autoRecalc = settings.value("autoRecalc", true).toBool();
    autoRecalcAction->setChecked(autoRecalc);
}
