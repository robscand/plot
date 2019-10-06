/*!
 * \file
 * \brief Заголовочный файл.
 *
 * Данный файл содержит объявление класса FindDialog.
 */
#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog> ///< Включение определения класса QDialog.

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

/*!
 * \brief The FindDialog class
 *
 * Класс FindDialog предназначен для создания диалогового окна Find,
 * которое позволяет пользователю выполнять поиск текста в электронной таблице.
 */
class FindDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief FindDialog
     *
     * Конструктор с параметром. Вызывается при создании объекта.
     * \param parent Указатель на родительский класс типа QWidget.
     */
    FindDialog(QWidget *parent = 0);

signals:
    /*!
     * \brief findPrevious
     *
     * Сигнал генерируется диалоговым окном, если флажок
     * поиска в обратном направлении (Search backward) не установлен.
     * \param str текст, считываемый из строки редактирования.
     * \param cs переменная перечисления Qt::CaseSensitivity
     * и принимает значение Qt::CaseSensitive или Qt::CaseInsensitive.
     */
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    /*!
     * \brief findPrevious
     *
     * Сигнал генерируется диалоговым окном, если установлен флажок
     * поиска в обратном направлении (Search backward).
     * \param str текст, считываемый из строки редактирования.
     * \param cs переменная перечисления Qt::CaseSensitivity
     * и принимает значение Qt::CaseSensitive или Qt::CaseInsensitive.
     */
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);

private slots:
    /*!
     * \brief findClicked
     *
     * Слот findClicked() вызывается при нажатии пользователем кнопки Find.
     * Он генерирует сигнал findPrevious() или findNext() в зависимости
     * от состояния флажка Search backward.
     */
    void findClicked();
    /*!
     * \brief enableFindButton
     *
     * Слот enableFindButton() вызывается при любом изменении значения
     * в строке редактирования. Устанавливает активный режим кнопки, если
     * в редактируемой строке имеется какой-нибудь текст.
     * \param text текст, считываемый из строки редактирования.
     */
    void enableFindButton(const QString &text);

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *caseCheckBox;
    QCheckBox *backwardCheckBox;
    QPushButton *findButton;
    QPushButton *closeButton;
};

#endif // FINDDIALOG_H
