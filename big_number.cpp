#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include "big_number.h"

BigNumber::BigNumber():_value(0) {}
BigNumber::BigNumber(cpp_bin_float_quad value):_value(value) {}
BigNumber::BigNumber(std::string strValue):_value(strValue) {}

const BigNumber BigNumber::operator+(const BigNumber& rv) const {
    return (_value + rv._value);
}

const BigNumber BigNumber::operator-(const BigNumber& rv) const {
    return (_value - rv._value);
}

const BigNumber BigNumber::operator*(const BigNumber& rv) const {
    return (_value * rv._value);
}

const BigNumber BigNumber::operator/(const BigNumber& rv) const {
    return (_value / rv._value);
}

const BigNumber BigNumber::procent() const {
    return (_value / 100);
}

const BigNumber BigNumber::procent(const BigNumber& rv) const {
    return (procent() * rv._value);
}

const BigNumber BigNumber::addProcent(const BigNumber& rv) const
{
    return _value * (1+rv._value/100);
}

const BigNumber BigNumber::subProcent(const BigNumber& rv) const
{
    return _value*(1-rv._value/100);
}

std::string BigNumber::toString() const
{
    std::stringstream ss;
    //<<std::setprecision(3);//если добавить то некорректно выводится число большое - 2.46914e+29
    ss << std::setprecision(std::numeric_limits<cpp_bin_float_quad>::digits10)/*<<std::setprecision(15)<<std::setw( std::ios::floatfield	)*/;
    ss << _value;
    return ss.str();
}

std::ostream& operator<<(std::ostream& stream, const BigNumber& value)
{
    stream << value._value;
    return stream;
}
