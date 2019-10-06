/*!
 * \file
 * \brief Заголовочный файл.
 *
 * Данный файл содержит объявление класса SpinBoxDelegate,
 * который играет роль делегата - класса отображения элементов.
 */
#ifndef SPINBOX_H
#define SPINBOX_H

#include <QItemDelegate> ///< Включение определения класса QItemDelegate.
/*!
 * \brief The SpinBoxDelegate class
 *
 * Класс-делегат, используемый для обеспечения корректности вводимых пользователем данных.
 * В ячейку электронной таблицы помещается виджет счетчика.
 */
class SpinBoxDelegate : public QItemDelegate
{
public:
    SpinBoxDelegate(QWidget *parent = 0); ///< Конструктор.
    ~SpinBoxDelegate(); ///< Деструктор.

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const; ///< Метод создающий виджет-редактор
    void setEditorData(QWidget *editor, const QModelIndex &index) const; ///< Метод передающий значение из таблицы в виджет-редактор
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const; ///< Метод передающий значение из виджета-редактора в таблицу
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const; ///< Метод определяющий расположение виджета-редактора
};


#endif // SPINBOX_H
