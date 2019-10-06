/*!
 * \file
 * \brief Исполняемый файл.
 *
 * Исходный файл класса Plot. С помощью методов класса QCustomPlot в нем
 * создается графическое представление зависимости моделей принтеров
 * от технологии печати; создаются и подписываются оси, определяется их масштаб.
 */
#include "plot.h"
#include "ui_plot.h"

Plot::Plot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Plot)
{
    ui->setupUi(this);
}

Plot::~Plot()
{
    delete ui;
}

void Plot::setGraphData(QVector<QString> &plotData)
{
    QMap<QString, double> myMap;//myMap это таблица содержищая пары: уникальный ключ и его значени
    for (int i = 0; i < plotData.size(); i++)
    {
        if (myMap.contains(plotData[i]))
            //Если ключ - имя метода печати - уже есть в myMap,
            //то увевличиваем значение этого ключа на единицу
            myMap[plotData[i]] += 1.0;
        else
            myMap[plotData[i]] = 1.0;
    }

    QVector<double>  ticks; // Значение x
    QVector<QString> labels; // Подписи значений x (Методы)
    QVector<double>  data; // Значение y
    double counter = 1.0;

    //Цикл итератора по элементам вектора и формирование значение осей x и y и надписей
    for (QMap<QString,double>::iterator it = myMap.begin(); it != myMap.end(); it++ )
    {
        ticks.push_back(counter);
        counter += 1.0;

        data.push_back(it.value());
        labels.push_back(it.key());
    }

    // Создаем объект типа график со столбцами
    QCPBars *myBars = new QCPBars(ui->widget->xAxis, ui->widget->yAxis);

    // Устанавливаем ширину и цвет pen
    QPen pen;
    pen.setWidthF(2.2);
    pen.setColor(QColor(100, 125, 0));
    myBars->setPen(pen);

    //Устанавливаем цвет кисточки(фона столбцов)
    myBars->setBrush(QColor(100, 255, 0, 50));

    // Передаем значение x y
    myBars->setData(ticks, data);

    ui->widget->addPlottable(myBars);

    // Подготавливаем ось х
    ui->widget->xAxis->setAutoTicks(false);
    ui->widget->xAxis->setAutoTickLabels(false);
    ui->widget->xAxis->setLabel(tr("Methods of printing"));
    ui->widget->xAxis->setTickVector(ticks);
    ui->widget->xAxis->setTickVectorLabels(labels);
    ui->widget->xAxis->setSubTickCount(0);

    // Подготавливаем ось у
    ui->widget->yAxis->setAutoSubTicks(false);//
    ui->widget->yAxis->setAutoTickStep(false);
    ui->widget->yAxis->setTickStep(1.0);
    ui->widget->yAxis->setLabel(tr("The number of models"));

    ui->widget->rescaleAxes();//перемаштабирование осей под наши значения
    ui->widget->xAxis->setRange(0, counter); // диапазон оси х

    ui->widget->replot();
}
