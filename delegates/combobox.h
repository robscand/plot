/*!
 * \file
 * \brief Заголовочный файл.
 *
 * Данный файл содержит объявление класса ComboBoxDelegate,
 * который играет роль делегата - класса отображения элементов.
 */
#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QItemDelegate> ///< Включение определения класса QItemDelegate.
/*!
 * \brief The ComboBoxDelegate class
 *
 * Класс-делегат, используемый для обеспечения корректности вводимых пользователем данных.
 * В ячейку электронной таблицы помещается виджет выпадающего списка.
 */
class ComboBoxDelegate : public QItemDelegate
{
    QHash<int, QString> m_values;

public:
    ComboBoxDelegate(QWidget *parent = 0); ///< Конструктор.
    ~ComboBoxDelegate(); ///< Деструктор.
    QWidget * createEditor( QWidget * parent,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index ) const; ///< Метод создающий виджет-редактор.
    void setEditorData( QWidget * editor, const QModelIndex &index ) const; ///< Метод передающий значение из таблицы в виджет-редактор.
    void setModelData( QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index ) const; ///< Метод передающий значение из виджета-редактора в таблицу.
    void updateEditorGeometry( QWidget *editor,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const; ///< Метод определяющий расположение виджета-редактора.
    void paint( QPainter * painter, const QStyleOptionViewItem &option,
                const QModelIndex &index ) const; ///< Метод отображающий виждет-радактор в таблице.
    inline QHash<int, QString>& values() { return m_values; } ///< Переменная, которая передаёт строки в виджет-редактор.



};
#endif // COMBOBOX_H
