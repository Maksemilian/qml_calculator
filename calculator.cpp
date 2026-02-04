#include <QDebug>
#include <QStack>
#include <QVector>
#include <QRegularExpression>

#include <algorithm>
#include <vector>

#include "big_number.h"
#include "calculator.h"

//################### MathExpressionParser #################

int Calculator::MathExpressionParser::getPriority(QChar op)
{
    if (op == '+' || op == '-' || op == '%'/**/) return 1;
    //if (op == '%') return 2;
    if (op == '*' || op == '/') return 3;
    return 0;
}

QVector<QString> Calculator::MathExpressionParser::shuntingYard(QString expression)
{
    QString output;
    QStack<QChar> operators;
    QVector<QString> elementsExpression;
    QString number;
    for(int i=0;i<expression.size();++i)
    {
        auto token = expression[i];
        if (token.isDigit() || token == '.')
        {
            output += token;
            number += token;
        }
        else
        {
            if(!number.isEmpty())
            {
                elementsExpression.push_back(number);
                number.clear();
            }

            if( token == '-' && ((i>0 && expression[i-1]!='%' && (getPriority(expression[i-1]) > 0 || expression[i-1] == '(' || expression[i-1] == ')'))
                                 || (i == 0 && i < expression.size() && expression[i+1].isDigit())/**/
                                 ))
            {
                number += token;
                continue;
            }

            if( token == '%'
                && (i+1 < expression.size() && expression[i+1] != '%' && getPriority(expression[i+1]) > 0)  )
            {
                if(number.isEmpty() && !elementsExpression.empty() && Calculator::isNumber(elementsExpression.back()))
                {
                    auto mutableEl = elementsExpression.back();
                    mutableEl += token;
                    elementsExpression.pop_back();
                    elementsExpression.push_back(mutableEl);
                    continue;
                }
                number += token;
                continue;
            }

            if(token == 'e' )
            {
                QString comboToken = token;
                bool firstPlusOrMinusFound = false;
                for(int j=i+1;j<expression.size();++j,++i)
                {
                    auto charEl = expression[j];
                    if( ((charEl == '+' || charEl == '-') && !firstPlusOrMinusFound)
                        || (firstPlusOrMinusFound && charEl.isDigit()) )
                    {
                        comboToken+= charEl;
                        firstPlusOrMinusFound = true;
                    }
                    else break;
                }

                if(number.isEmpty() && !elementsExpression.empty() && Calculator::isNumber(elementsExpression.back()))
                {
                    auto mutableEl = elementsExpression.back();
                    mutableEl += comboToken;
                    elementsExpression.pop_back();
                    elementsExpression.push_back(mutableEl);
                    continue;
                }
            }

            if (token == '(')
            {
                operators.push(token);
            }
            else if (token == ')')
            {
                while (!operators.empty() && operators.top() != '(')
                {
                    output += operators.top();
                    elementsExpression.push_back(operators.top());
                    operators.pop();
                }
                operators.pop();
            }
            else
            {
                while (!operators.empty() && getPriority(operators.top()) >= getPriority(token))
                {
                    output += operators.top();
                    elementsExpression.push_back(operators.top());
                    operators.pop();
                }
                operators.push(token);
            }
        }
    }

    if(!number.isEmpty())
        elementsExpression.push_back(number);

    while (!operators.empty())
    {
        output += operators.top();
        elementsExpression.push_back(operators.top());
        operators.pop();
    }

    //qDebug()<<"DEBUG:"<<output;
    return elementsExpression;
}

//################### MathExpressionParser #####################

Calculator::Calculator(QObject *parent)
    : QObject{parent}
{
    _twoOperandsOperations["+"] = [this](BigNumber first,BigNumber second)
    {
        return first + second;
    };
    _twoOperandsOperations["-"] = [this](BigNumber first,BigNumber second)
    {
        return first - second;
    };
    _twoOperandsOperations["*"] = [this](BigNumber first,BigNumber second)
    {
        return first * second;
    };
    _twoOperandsOperations["/"] = [this](BigNumber first,BigNumber second)
    {
        return first / second;
    };
    _twoOperandsOperations["%"] = [this](BigNumber first,BigNumber second)
    {
        return first.procent(second);
    };
    _twoOperandsOperations["+%"] = [this](BigNumber first,BigNumber second)
    {
        return first.addProcent(second);
    };
    _twoOperandsOperations["-%"] = [this](BigNumber first,BigNumber second)
    {
        return first.subProcent(second);
    };
}

bool Calculator::isNumber(const QString &str)
{
    bool foundDot = false;
    bool foundMinus = false;
    bool foundNum = false;
    bool foundProcent = false;
    bool foundEps = false;
    for(const auto & let : str)
    {
        if(let.isNumber())
        {
            foundNum = true;
            continue;
        }
        else if(let == '.')
            foundDot == true;
        else if(let == '-' || let == '+')
            foundMinus = true;
        else if(let == '%')
            foundProcent = true;
        else if(let == 'e')
            foundEps = true;
        else
            return false;
    }
    return (foundNum && (foundDot || foundMinus || foundProcent || foundEps) ) || foundNum ;
}

PairQml Calculator::calculate(const QString & calculationLine)
{
    if(calculationLine.isEmpty())
        return {false,""};

    Calculator::MathExpressionParser parser;
    auto elementsExpression = parser.shuntingYard(calculationLine);
    QString output;
    QStack<BigNumber> stack;

    auto new_end_iterator = std::remove_if(elementsExpression.begin(),elementsExpression.end(),[this](QString &str){
        return str == QString("(") || str == QString(")");
    });
    elementsExpression.erase(new_end_iterator, elementsExpression.end());

    int countNum = 0;
    int countOperator = 0;
    for(auto &el : elementsExpression)
    {
        if(el == QString("(") || el == QString(")") || el == QString("%"))
            continue;
        if(isNumber(el))
            countNum++;
        else
            countOperator++;
    }

    if(countNum<=countOperator)
        return {false,_lastResult};

    for(int i=0;i<elementsExpression.size();++i)
    {
        const auto & el = elementsExpression[i];

        output += el;

        bool ok = isNumber(el);
        if(ok)
        {
            auto stringValue = el;
            auto posProcent = stringValue.indexOf("%");
            auto posEps = stringValue.indexOf("e");
            if(posProcent != std::string::npos)
            {
                stringValue.erase(stringValue.begin() + posProcent);
                BigNumber value (stringValue.toStdString());
                stack.push_back(value.procent());
                continue;
            }
            else if(posEps!= std::string::npos)
            {
                QString epsValue = stringValue.mid(posEps,stringValue.length()-posEps);

                stringValue = stringValue.mid(0,posEps);
                QRegularExpression separator("[+-]");
                QStringList epsValueList = epsValue.split(separator, Qt::SkipEmptyParts);

                BigNumber value (stringValue.toStdString());
                if(epsValueList.size() == 2)
                {
                    auto powerValue = epsValueList[1].toULongLong();
                    auto powerRaisingResult=  cpp_bin_float_quad(boost::multiprecision::pow(boost::multiprecision::cpp_int(10),powerValue));
                    BigNumber v(powerRaisingResult);
                    if(epsValue[1] == "+")
                        value = value * v;
                    else if(epsValue[1] == "-")
                          value = value / v;
                }
                stack.push_back(value);
                continue;
            }

            BigNumber value (el.toStdString());
            stack.push_back(value);
        }
        else
        {
            if(el == "%" && stack.size() == 1 )
            {
                auto value = stack.pop();
                stack.push_back(value.procent());
            }
            else
            {
                auto operation = el;
                if(stack.size() == 1)
                {
                    if(operation =="-")
                    {
                        auto &topEl = stack.top();
                        topEl = topEl * BigNumber("-1");
                    }
                    continue;
                }
                auto secondValue = stack.pop();
                auto firstValue = stack.pop();

                if((operation == "+" || operation == "-") && i+1 < elementsExpression.size() && elementsExpression[i+1] == "%")
                {
                    operation+="%";
                    i++;
                }
                if(_twoOperandsOperations.count(operation) > 0)
                {
                    auto & func = _twoOperandsOperations[operation];
                    auto result = func(firstValue,secondValue);
                    //std::cout<<firstValue <<" "<< operation.toStdString() <<" "<<secondValue<<" = "<<result<<std::endl;
                    stack.push_back(result);
                }
            }
        }
    }
    if(!stack.isEmpty() && stack.size() == 1)
    {
        auto result = stack.pop();
        _lastResult = QString::fromStdString(result.toString());
        return {true,_lastResult};
    }
    //qDebug()<<"ERROR_STRING:"<<output;
    return {false,""};
}
