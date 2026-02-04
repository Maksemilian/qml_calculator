#include <QtTest>
#include "../calculator.h"
class CalculatorTest: public QObject
{
    Q_OBJECT
public:
    explicit CalculatorTest(QObject *parent = 0);
private:
    void test();
private slots:
    void all_basic_opearations();
    void plus_and_minus();
    void procent();
    void float_values_opearation();
    // *,/,+,-
    void all_basic_opearations_data();
    void plus_and_minus_data();
    void procent_data();
    void float_values_opearation_data();
};

CalculatorTest::CalculatorTest(QObject *parent) :
    QObject(parent)
{
}

void CalculatorTest::test()
{
    Calculator calc;
    QFETCH(QString, input_string);
    QFETCH(QString, result_string);
    QCOMPARE(calc.calculate(input_string).second.toString(),result_string);
}

void CalculatorTest::all_basic_opearations()
{
    test();
}
void CalculatorTest::plus_and_minus()
{
    test();
}

void CalculatorTest::procent()
{
    test();
}

void CalculatorTest::float_values_opearation()
{
    test();
}

void CalculatorTest::all_basic_opearations_data()
{
    QTest::addColumn<QString>("input_string");
    QTest::addColumn<QString>("result_string");

    QTest::newRow("3+4*2/(1-5)") << "3+4*2/(1-5)" << "1" ;
    QTest::newRow("1*(2+4)*5")     << "1*(2+4)*5" << "30" ;
    QTest::newRow("1*(2+4)*5+2")     << "1*(2+4)*5+2" << "32" ;
    QTest::newRow("1+2*4+5")     << "1+2*4+5" << "14" ;
    QTest::newRow("1+2*(4+5)")     << "1+2*(4+5)" << "19" ;
    QTest::newRow("(1+2)*4+5")     << "(1+2)*4+5" << "17" ;
}


void CalculatorTest::plus_and_minus_data()
{
    QTest::addColumn<QString>("input_string");
    QTest::addColumn<QString>("result_string");

    QTest::newRow("-5-5") << "-5-5" << "-10" ;
    QTest::newRow("-5+-5") << "-5+-5" << "-10" ;
    QTest::newRow("5+-5") << "5+-5" << "0" ;
    QTest::newRow("-5+5") << "5+-5" << "0" ;
    QTest::newRow("5--5") << "5--5" << "10" ;
}

void CalculatorTest::procent_data()
{
    QTest::addColumn<QString>("input_string");
    QTest::addColumn<QString>("result_string");

    QTest::newRow("5%") << "5%" << "0.05" ;
    QTest::newRow("-5%") << "-5%" << "-0.05" ;
    QTest::newRow("-5%-2%") << "-5%-2%" << "-0.049" ;
    QTest::newRow("-5%*-2%") << "-5%*-2%" << "0.001" ;
    QTest::newRow("5+5%") << "5+5%" << "5.25" ;
    QTest::newRow("5%+5") << "5%+5" << "5.05" ;
    QTest::newRow("5-5%") << "5-5%" << "4.75" ;
    QTest::newRow("5%-5") << "5%-5" << "-4.95" ;
    QTest::newRow("5%100") << "5%100" << "5" ;
    QTest::newRow("-5%100") << "-5%100" << "-5" ;
    QTest::newRow("5%5%5%") << "5%5%5%" << "0.000125" ;
    QTest::newRow("5000%%%") << "5000%%%" << "0.005" ;
    QTest::newRow("5*5-5%") << "5*5-5%" << "23.75" ;
    QTest::newRow("5%*7") << "5%*7" << "0.35" ;
    QTest::newRow("5*5+(5%*2)") << "5*5+(5%*2)" << "25.1" ;
    QTest::newRow("5*5-(5%*2)") << "5*5-(5%*2)" << "24.9" ;
    QTest::newRow("5*5-5%*2") << "5*5-5%*2" << "24.9" ;
}

void CalculatorTest::float_values_opearation_data()
{
    QTest::addColumn<QString>("input_string");
    QTest::addColumn<QString>("result_string");

    QTest::newRow("5.5+5.7") << "5.5+5.7" << "11.2" ;
    QTest::newRow("5.5*5.7") << "5.5*5.7" << "31.35" ;
    QTest::newRow("5.5/2") << "5.5/2" << "2.75" ;
}

QTEST_APPLESS_MAIN(CalculatorTest)


#include "tst_calculator_test.moc"
