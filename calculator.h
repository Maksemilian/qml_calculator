#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QVariant>
#include <QPair>

#include <map>
#include <functional>
class BigNumber;

class PairQml {
    Q_GADGET

    Q_PROPERTY(QVariant first MEMBER first CONSTANT FINAL)
    Q_PROPERTY(QVariant second MEMBER second CONSTANT FINAL)

public:
    PairQml() = default;
    PairQml(QVariant f, QVariant s): first(f), second(s) {}

    QVariant first;
    QVariant second;
};

class Calculator : public QObject
{
    Q_OBJECT
private:
const QString ERROR_MESSAGE = "Incorrect expression";
private:
    class MathExpressionParser
    {
    private :
        int getPriority(QChar op) ;
    public:
        QVector<QString> shuntingYard(QString expression) ;
    };
public:
    static bool isNumber(const QString &str);
    explicit Calculator(QObject *parent = nullptr);
    Q_INVOKABLE PairQml calculate(const QString &calculationLine);
    Q_INVOKABLE QString getErrorMessageString(){return ERROR_MESSAGE;}
signals:

private:
    std::map<QString,std::function<BigNumber(BigNumber,BigNumber)>> _twoOperandsOperations;
    QString _lastResult;
};

#endif // CALCULATOR_H
