/*!
 * \file
 * \brief Заголовочный файл
 *
 * Данный файл содержит объявление класса Сell,
 * который спроектирован для удобства работы со Spreadsheet.
 */
#ifndef CELL_H
#define CELL_H

#include <QTableWidgetItem> ///< Включение определения класса QTableWidgetItem.
/*!
 * \brief The Cell class
 *
 * Класс Cell применяется для хранения формул ячеек и обеспечивает переопределение функции QTableWidgetItem::data(),
 * которая вызывается в Spreadsheet через функцию QTableWidgetItem::text() для отображения результата вычисления формулы ячейки
 */
class Cell : public QTableWidgetItem
{
public:
    /*!
     * \brief Cell
     *
     * Конструктор вызывается при создании объекта.
     * В конструкторе устанавливается признак "dirty" только для кеша.
     * Родительский объект не нужно: когда делается вставка ячейки QTableWidget с помощью
     * setItem(), QTableWidget автоматически станет ее владельцем.
     */
    Cell();

    /*!
     * \brief clone
     *
     * Функция clone() вызывается в QTableWidget, когда необходимо создать новую ячейку:
     * пользователь вводит данные в пустую ячейку, которая до сих пор не использовалась.
     * \return вызов конструктора копирования по умолчанию, вызываемый при создании
     * экземпляров новых ячеек Cell.
     */
    QTableWidgetItem *clone() const;
    /*!
     * \brief setData
     *
     * Функция вызывается при создании новой формулы для обеспечения перерасчета
     * ячейки при последующем вызове text().
     * \param role получение роли данных ячейки.
     * \param value значение в ячейке.
     */
    void setData(int role, const QVariant &value);
    /*!
     * \brief data
     *
     * Функция data() класса QTableWidget переопределяется. Она возвращает текст,
     * который должен отображаться в электронной таблице, если в вызове указана
     * роль Qt::DisplayRole, или формулу, если в вызове указана роль Qt::EditRole.
     * \param role получение роли данных ячейки.
     * \return Текст в ячейке электронной таблицы.
     * \return Если нельзяполучить достоверное значение - возвращается "####".
     * \return Возвращает значения выравнивания.
     */
    QVariant data(int role) const;
    /*!
     * \brief setFormula
     *
     * Функция setFormula() задает формулу ячейки.
     * Вызывается из функции Spreadsheet::setFormula().
     * \param formula Формула ячейки.
     */
    void setFormula(const QString &formula);
    /*!
     * \brief formula
     *
     * Функция formula() удобна для получения данных EditRole заданного элемента.
     * Вызывается из Spreadsheet::formula().
     * \return Данные EditRole ячейки - формула.
     */
    QString formula() const;
    /*!
     * \brief setDirty
     *
     * Функция setDirty() вызывается для перерасчета значения ячейки.
     */
    void setDirty();

private:
    /*!
     * \brief value
     *
     * Закрытая функция Cell::value(), используемая в data(), возвращает значение ячейки
     * типа QVariant. Если флажок cacheIsDirty имеет значение true, необходимо выполнить
     * перерасчет значения.
     * \return значение cachedValue.
     */
    QVariant value() const;
    /*!
     * \brief evalExpression
     *
     * Функция evalExpression() возвращает значение выражения из ячейки электронной таблицы.
     * Выражение состоит из термов, разделенных знаками операций "+" или "-".
     * Термы состоят из факторов, разделенных знаками операций "*" или "/".
     * Разбивая выражения на термы, а термы на факторы, мы обеспечиваем правильную последовательность выполнения операций.
     * Для каждого грамматического символа имеется соответствующая функция-член, которая выполняет его синтаксический анализ,
     * и структура которой очень хорошо отражает его грамматику.
     * Построенные таким образом синтаксические анализаторы называются парсерами с рекурсивным спуском.
     * \param str Выражение в ячейке электронной таблицы.
     * \param pos Вхождение символа в выражение.
     * \return Результат разделения.
     */
    QVariant evalExpression(const QString &str, int &pos) const;
    /*!
     * \brief evalTerm
     *
     * Напоминает функцию evalExpression(), но имеет дело с операциями умножения и деления.
     * В этой функции нельзя допускать деление на нуль, так как приводит к ошибке.
     * Не рекомендуется проверять числа с плавающей точкой из-за ошибки округления, но
     * можно спокойно делать проверку на равенство значению 0.0 для предотвращения деления на нуль.
     * \param str Выражение в ячейке электронной таблицы.
     * \param pos Вхождение символа в выражение.
     * \return Результат разделения.
     */
    QVariant evalTerm(const QString &str, int &pos) const;
    /*!
     * \brief evalFactor
     *
     * В функции evalFactor() осуществляется рекурсия при синтаксическом анализе.
     * Она проверяет, не является ли фактор отрицательным, затем проверяется наличие
     * открытой скобки. Если она имеется, анализируется значение внутри скобок как
     * выражения, вызывая evalExpression(). При анализе выражения в скобках выражение
     * evalExpression() вызывает функцию evalTerm(), которая вызывает функцию
     * evalFactor(), которая вновь вызывает функцию evalExpression().
     * \param str Выражение в ячейке электронной таблицы.
     * \param pos Вхождение символа в выражение.
     * \return Результат разделения.
     */
    QVariant evalFactor(const QString &str, int &pos) const;
    //!Класс Cell расширяет QTableWidgetItem, добавляя две закрытые переменные:
    mutable QVariant cachedValue; ///< Переменная cachedValue кеширует значение ячейки в виде значения типа QVariant. Используем QVariant поскольку у ячеек разный тип.
    mutable bool cacheIsDirty;  ///< Переменная cacheIsDirty принимает значение true, если кешируемое значение устарело.
    //Ключевое слово mutable позволяет модифицировать переменные в функциях с модификатором const.
};

#endif // CELL_H
