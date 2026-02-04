#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/math/special_functions/gamma.hpp>

using namespace boost::multiprecision;
using namespace boost::multiprecision::backends;

class BigNumber
{

   friend std::ostream& operator<<(std::ostream& stream, const BigNumber& value);

public:
    BigNumber();
    BigNumber(std::string value);
    BigNumber(cpp_bin_float_quad value);

    const BigNumber operator+(const BigNumber& rv) const;
    const BigNumber operator-(const BigNumber& rv) const;
    const BigNumber operator*(const BigNumber& rv) const;
    const BigNumber operator/(const BigNumber& rv) const;
    const BigNumber procent() const;
    const BigNumber procent(const BigNumber& rv) const;
    const BigNumber addProcent(const BigNumber& rv) const;
    const BigNumber subProcent(const BigNumber& rv) const;

    std::string toString()const;

private:
    cpp_bin_float_quad _value;
};

#endif // BIG_NUMBER_H
