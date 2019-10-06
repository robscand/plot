/*!
 * \file
 * \brief Заголовочный файл.
 *
 * Данный файл содержит объявление класса, обеспечивающего
 * прием данных и их компановку при построении графика
 */
#ifndef PLOT_H
#define PLOT_H

#include <QDialog> ///< Включение определения класса QDialog.

namespace Ui {
class Plot;
}

/*!
 * \brief The Plot class
 *
 * Класс Plot предназначен для использования инструментов класса
 * QCustomPlot с целью отрисовки графика.
 */
class Plot : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Plot
     *
     * Конструктор с параметром. Вызывается при создании объекта.
     * \param parent Указатель на родительский класс типа QWidget.
     */
    explicit Plot(QWidget *parent = 0);
    ~Plot();

    /*!
     * \brief setGraphData
     *
     * Метод получающий данные для графика из MainWindow.
     * \param plotData Данные для построения графика.
     */
    void setGraphData(QVector<QString> &plotData);

private:
    Ui::Plot *ui;
};

#endif // PLOT_H
