/*!
 * \file
 * \brief Исполняемый файл.
 *
 * Функция main вызывается при старте программы.
 * В ней создается объект главного окна и выводится
 * на экран для обработки.
 */
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow *win = new MainWindow;
    win->show();
    return app.exec();
}
