#include "complex_num.hxx"
#include <cmath>
#include <algorithm>

ComplexNum ComplexNum::i{0, 1};

ComplexNum::ComplexNum(double re) :_re{re} {}

ComplexNum::ComplexNum(double re, double im) : _re{re}, _im{im} {}

double ComplexNum::get_abs() const {
	return std::sqrt(_re * _re + _im * _im);
}

double ComplexNum::get_re() const {
	return _re;
}

double ComplexNum::get_im() const {
	return _im;
}

ComplexNum ComplexNum::operator+=(ComplexNum other) {
    _re += other._re;
    _im += other._im;
    return *this;
}

ComplexNum ComplexNum::operator-=(ComplexNum other) {
    _re -= other._re;
    _im -= other._im;
    return *this;
}

ComplexNum ComplexNum::operator*=(ComplexNum other) {
    double re = _re * other._re - _im * other._im;
    double im =  _re * other._im + _im * other._re;
    _re = re;
    _im = im;
    return *this;
}

ComplexNum operator+(ComplexNum left, ComplexNum right) {
    left += right;
    return left;
}

ComplexNum operator-(ComplexNum left, ComplexNum right) {
    left -= right;
    return left;
}

ComplexNum operator*(ComplexNum left, ComplexNum right) {
    left *= right;
    return left;
}

std::ostream& operator<<(std::ostream& ostream, ComplexNum complex_num) {
    ostream << complex_num.get_re() << " + " << complex_num.get_im() << "i";
    return ostream;
}

std::istream& operator>>(std::istream& istream, ComplexNum& complex_num) {
    istream >> complex_num._re >> complex_num._im;
    return istream;
}
