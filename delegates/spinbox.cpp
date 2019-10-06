/*!
 * \file
 * \brief Исполняемый файл.
 *
 * Исходный файл вспомогательного класс-делегата SpinBoxDelegate.
 * В нем происходит создание виджета счетчика. Происходит установка
 * границ счета. Редактирование ячейки происходит путем задания
 * числовых значений.
 */
#include "spinbox.h"

#include <QSpinBox>

SpinBoxDelegate::SpinBoxDelegate(QWidget *parent):
    QItemDelegate(parent)
{

}

SpinBoxDelegate::~SpinBoxDelegate()
{

}


QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{

    QSpinBox *editor = new QSpinBox(parent);
    editor->setMinimum(0);
    editor->setMaximum(999999);
    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor,
                             const QModelIndex &index) const
{
    int val = index.model()->data(index, Qt::EditRole).toInt();

    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);

    if (val)

        spinBox->setValue(val);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                            const QModelIndex &index) const
{
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);

    int val = spinBox->value();

    if (val)

        model->setData(index, val, Qt::EditRole);
}


void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
