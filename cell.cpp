/*!
 * \file
 * \brief Исполняемый файл.
 *
 * Исходный файл вспомогательного класса Cell выполняет задачу преобразования формулы в значение.
 * Отображаемый в ячейке текст соответствует значению, полученному в результате расчета формулы,
 * а не является текстом самой формулы.
 */
#include <QtGui>

#include "cell.h"

Cell::Cell() //В конструкторе нам необходимо установить признак "dirty" ("грязный") только для кеша.
{            //Передавать родительский объект нет необходимости.
    setDirty();
}

QTableWidgetItem *Cell::clone() const //Функция clone() вызывается в QTableWidget, когда необходимо создать новую ячейку:
{                                     //пользователь вводит данные в пустую ячейку, которая до сих пор не использовалась.
    return new Cell(*this); //Cоздавая с помощью функции члена экземпляры новых ячеек Cell в функции clone(),
}                           //по умолчанию используется конструктор копирования, автоматически создаваемый C++.

void Cell::setFormula(const QString &formula) //Функция setFormula() задает формулу ячейки;
{                                             //удобная функция для вызова setData() с указанием роли правки - EditRole.
    setData(Qt::EditRole, formula);           //Вызывается из функции Spreadsheet::setFormula().
}

QString Cell::formula() const // Функция formula() удобна для получения данных EditRole заданного элемента.
{                             //Вызывается из Spreadsheet::formula().
    return data(Qt::EditRole).toString();
}

void Cell::setData(int role, const QVariant &value)
{
    QTableWidgetItem::setData(role, value);
    if (role == Qt::EditRole)
        setDirty();
}

void Cell::setDirty() // Функция setDirty() вызывается для перерасчета значения ячейки
{
    cacheIsDirty = true; //Установка флажка cacheIsDirty на значение true указывает на то,
}                        //что значение cachedValue больше не отражает текущее состояние.

/* Функция data() класса QTableWidgetItem переопределяется.
 * Она возвращает текст, который должен отображаться в электронной таблице, если в вызове указана роль Qt::DisplayRole,
 * или формулу, если в вызове указана роль Qt::EditRole. */
QVariant Cell::data(int role) const
{
    if (role == Qt::DisplayRole) {
        if (value().isValid()) {
            return value().toString(); //При задании роли DisplayRole используется функция value() для расчета значения ячейки.
        } else {
            return "####"; //Если нельзя получить достоверное значение( из-за того, что формула неверна), возвращается значение "####".
        }
    } else if (role == Qt::TextAlignmentRole) { //Функция обеспечивает подходящее выравнивание, если вызывается с ролью Qt::TextAlignmentRole.
        if (value().type() == QVariant::String) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return int(Qt::AlignRight | Qt::AlignVCenter);
        }
    } else {
        return QTableWidgetItem::data(role);
    }
}

const QVariant Invalid; //Используемый по умолчанию конструктор QVariant устанавливает значение "invalid" (недопустимое).

QVariant Cell::value() const //Закрытая функция value() возвращает значение ячейки.
{
    if (cacheIsDirty) {         //Если флажок cacheIsDirty имеет значение true, необходимо выполнить пересчет значения.
        cacheIsDirty = false;
        QString formulaStr = formula();
        if (formulaStr.startsWith('\'')) {      //Если формула начинается с одиночной кавычки ("'12345"), то кавычка занимает позицию 0,
            cachedValue = formulaStr.mid(1);    //а значение представляет собой строку в позициях с 1 по последнюю.
        } else if (formulaStr.startsWith('=')) { //Если формула начинается со знака равенства, то мы выделяем строку, начиная с позиции 1
            cachedValue = Invalid;               // и удаляем из нее любые пробелы.
            QString expr = formulaStr.mid(1);
            expr.replace(" ", "");
            expr.append(QChar::Null);

            int pos = 0; //Переменная pos задает позицию символа, с которого должен начинаться синтаксический анализ выражения.
            cachedValue = evalExpression(expr, pos); //Вызов функции evalExpression() для вычисления значения выражения. Аргумент pos передается по ссылке.
            if (expr[pos] != QChar::Null) //После вызова функции evalExpression() в позиции pos должен быть установлен символ QChar::Null, если синтаксический анализ завершился успешно.
                cachedValue = Invalid; //Если синтаксический анализ не закончился успешно, происходит установка cachedValue на значение Invalid.
        } else { //Если формула не начинается с одной кавычки или знака равенства, происходит попытка преобразовать ее в число с плавающей точкой, используя функцию toDouble().
            bool ok;
            double d = formulaStr.toDouble(&ok);
            if (ok) { //Если преобразование удается, происходит установка cachedValue на полученное значение.
                cachedValue = d;
            } else { //В противном случае cachedValue устанавливается на строку формулы.
                cachedValue = formulaStr;
            }
        }
    }
    return cachedValue;
}

/* Функция evalExpression() возвращает значение выражения из ячейки электронной таблицы.
 * Выражение состоит из термов, разделенных знаками операций "+" или "-".
 * Термы состоят из факторов, разделенных знаками операций "*" или "/".
 * Разбивая выражения на термы, а термы на факторы, мы обеспечиваем правильную последовательность выполнения операций.
 * Для каждого грамматического символа имеется соответствующая функция-член, которая выполняет его синтаксический анализ,
 * и структура которой очень хорошо отражает его грамматику.
 * Построенные таким образом синтаксические анализаторы называются парсерами с рекурсивным спуском. */
QVariant Cell::evalExpression(const QString &str, int &pos) const
{
    QVariant result = evalTerm(str, pos); //Вызов функции evalTerm() для получения значения первого терма
    while (str[pos] != QChar::Null) {
        QChar op = str[pos];
        if (op != '+' && op != '-') //Если после первого терма нет символа "+" или "-", то выражение состоит из одного единственного терма,
            return result;          //возвращение значения терма в качестве значения всего выражения.
        ++pos;

        QVariant term = evalTerm(str, pos); //Если после первого терма идет символ "+" или "-", вызов evalTerm() во второй раз.
        if (result. type() == QVariant::Double
                && term.type() == QVariant::Double) { //После получения значений первых двух термов вычисляется результат операции в зависимости от оператора.
            if (op == '+') {                          //Если оба терма имеют тип double, высчитывается результат в виде числа типа double.
                result = result.toDouble() + term.toDouble();
            } else {
                result = result.toDouble() - term.toDouble();
            }
        } else {
            result = Invalid;   //В противном случае результат устанавливается на значение Invalid.
        }
    }
    return result;
} //Процедура продолжается, пока не закончатся термы.

/* Напоминает функцию evalExpression(), но имеет дело с операциями умножения и деления.
 * В этой функции нельзя допускать деление на нуль, так как приводит к ошибке.
 * Не рекомендуется проверять числа с плавающей точкой из-за ошибки округления, но
 * можно спокойно делать проверку на равенство значению 0.0 для предотвращения деления на нуль. */
QVariant Cell::evalTerm(const QString &str, int &pos) const
{
    QVariant result = evalFactor(str, pos);
    while (str[pos] != QChar::Null) {
        QChar op = str[pos];
        if (op != '*' && op != '/')
            return result;
        ++pos;

        QVariant factor = evalFactor(str, pos);
        if (result.type() == QVariant::Double
                && factor.type() == QVariant::Double) {
            if (op == '*') {
                result = result.toDouble() * factor.toDouble();
            } else {
                if (factor.toDouble() == 0.0) {
                    result = Invalid;
            } else {
                    result = result.toDouble() / factor.toDouble();
                }
            }
        } else {
            result = Invalid;
        }
    }
    return result;
}

QVariant Cell::evalFactor(const QString &str, int &pos) const //
{
    QVariant result;
    bool negative = false;

    if (str[pos] == '-') { //Начинаем с проверки, не является ли фактор отрицательным
        negative = true;
        ++pos;
    }

    if(str[pos] == '(') { //Проверка наличия открытой скобки
        ++pos;
        /*Если она имеется, происходит анализ внутри скобок как выражение, вызывая функцию evalExpression().
         * При анализе выражения внутри скобок функция evalExpression() вызывает функцию evalTerm(), которая в свою очередь вызывает evalFactor().
         * Именно в этом месте осуществляется рекурсия при синтаксическом анализе. */
        result = evalExpression(str, pos);
        if (str[pos] != ')')
            result = Invalid;
        ++pos;
    } else { //Если фактором не является вложенное выражение, выделяем следующую лексему - token - она должна задавать обозначение ячейки или быть числом.
        QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
        QString token;
        while (str[pos].isLetterOrNumber() || str[pos] == '.') {
            token += str[pos];
            ++pos;
        }

        if (regExp.exactMatch(token)) { //Если лексема удовлетворяет регулярному выражению в переменной QRegExp,
            int column = token[0].toUpper().unicode() - 'A';
            int row = token.mid(1).toInt() - 1;
            Cell *c = static_cast<Cell *>(tableWidget()->item(row, column));
            if (c) {
                result = c->value(); //то она является ссылкой на ячейку, и вызывается функция value() для ячейки
            } else {
                result = 0.0;
            }
        } else { //если лексема не является ссылкой на ячейку, мы рассматриваем ее как число.
            bool ok;
            result = token.toDouble(&ok);
            if (!ok)
                result = Invalid;
        }
    }

    if (negative) {
        if (result.type() == QVariant::Double) {
            result = -result.toDouble();
        } else {
            result = Invalid;
        }
    }
    return result;
}





