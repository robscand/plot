/*!
 * \file
 * \brief Исполняемый файл.
 *
 * Исходный файл вспомогательного класса-делегата ComboBoxDelegate.
 * В нем происходит создание виджета выадающего списка и помещение в него
 * требуемых пользоваетелем данных. Редактирование ячейки происходит
 * только с помощью этих значений.
 */
#include "combobox.h"
#include <QPainter>
#include <QComboBox>

ComboBoxDelegate::ComboBoxDelegate( QWidget *parent ):
    QItemDelegate(parent)
{
    values().insert(1, tr("Пузырьково-струйная"));
    values().insert(2, tr("Термальная струйная"));
    values().insert(3, tr("Микропьезоструйная"));
    values().insert(4, tr("Пьезоэлектрическая"));
    values().insert(5, tr("НР Scalable Printing Technology"));
}

ComboBoxDelegate::~ComboBoxDelegate()
{}


QWidget * ComboBoxDelegate::createEditor( QWidget * parent,
                                      const QStyleOptionViewItem& /* option */,
                                      const QModelIndex& /* index */) const
{
    QComboBox * pEditor = new QComboBox( parent );

    QHash<int, QString>::const_iterator i = m_values.constBegin();
    while ( i != m_values.constEnd() )
    {
        pEditor -> addItem( i.value(), i.key() );
        ++i;
     }

    pEditor -> installEventFilter( const_cast<ComboBoxDelegate*>( this ) );
    return pEditor;
}


void ComboBoxDelegate::setEditorData( QWidget * editor, const QModelIndex &index) const
{

    int value = index.model()->data( index, Qt::EditRole ).toInt();

    QComboBox * cb = static_cast<QComboBox*>( editor );
    int idx = cb->findData( value );
    if ( idx < 0 ) return;
    cb->setCurrentIndex( idx );
}

void ComboBoxDelegate::setModelData( QWidget * editor,
                                    QAbstractItemModel *model,
                                    const QModelIndex& index) const
{

    QComboBox * cb = static_cast<QComboBox*>( editor );
    int idx = cb->currentIndex();
    if ( idx < 0 ) return;
    int value = cb->itemData( idx ).toInt();
    model->setData( index, value,Qt::EditRole );
}

void ComboBoxDelegate::updateEditorGeometry( QWidget *editor,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex& /* index */) const
{

    editor->setGeometry( option.rect );
}

void ComboBoxDelegate::paint( QPainter * painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index ) const
{
    QStyleOptionViewItem opt = option;
    int value = index.data().toInt();


    QString s = m_values[ value ];
    QVariant color = index.data( Qt::TextColorRole );

    if ( color.isValid() && qvariant_cast<QColor>(color).isValid() )
        opt.palette.setColor( QPalette::Text, qvariant_cast<QColor>(color) );

    opt.displayAlignment = Qt::AlignVCenter | Qt::AlignLeft;

    drawDisplay(painter, opt, opt.rect, s);
    drawFocus(painter, opt, opt.rect);
}



