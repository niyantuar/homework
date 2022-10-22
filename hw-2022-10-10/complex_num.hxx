#ifndef COMPLEX_NUM_HXX
#define COMPLEX_NUM_HXX

#include <iostream>


class ComplexNum {
private:
	double _re = 0;
	double _im = 0;
public:
	ComplexNum() = default;
	ComplexNum(double a);
	ComplexNum(double re, double im);

    static ComplexNum i;

	double get_abs() const;
 
	double get_re() const;

	double get_im() const;

    ComplexNum operator+=(ComplexNum other);
    ComplexNum operator-=(ComplexNum other);
    ComplexNum operator*=(ComplexNum other);

    friend std::istream& operator>>(std::istream& istream, ComplexNum& complex_num);
};

ComplexNum operator+(ComplexNum left, ComplexNum right);
ComplexNum operator-(ComplexNum left, ComplexNum right);
ComplexNum operator*(ComplexNum left, ComplexNum right);
std::ostream& operator<<(std::ostream& ostream, ComplexNum complex_num);

#endif
