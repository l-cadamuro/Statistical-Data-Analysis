#include "Complex.h"
#include <cmath>
#include <iostream>

#define PI 3.14159265359

Complex::Complex()
{
    //ctor
}

Complex::Complex(double re, double im)
{
    m_re = re;
    m_im = im;
}

Complex::~Complex()
{
    //dtor
}

void Complex::SetPolar(double rho, double theta)
{
    if (rho < 0)
    {
        std::cout << "Warning: rho < 0. Assuming absolute value\n";
        rho = -1.*rho;
    }
    m_re = rho*cos(theta);
    m_im = rho*sin(theta);
    return;
}

void Complex::SetRe(double re)
{
    m_re = re;
}

void Complex::SetIm(double im)
{
    m_im = im;
}

double Complex::GetRe()
{
    return m_re;
}

double Complex::GetIm()
{
    return m_im;
}

double Complex::GetRho()
{
    return sqrt(m_re*m_re + m_im*m_im);
}

double Complex::GetTheta()
{
    if (m_re == 0)
    {return 0;}

    if (m_re > 0)
    {return atan(m_im/m_re);}

    else
    {return (atan(m_im/m_re) + PI);}

}

Complex& Complex::Conjugate()
{
    m_im = -1.*m_im;
    return *this;
}

double Complex::GetSquareModulus()
{
    return (m_re*m_re + m_im*m_im);
}

Complex& Complex::operator= (const Complex& original)
{
    m_re = original.m_re;
    m_im = original.m_im;
    return *this;
}

Complex& Complex::operator+= (const Complex& toadd)
{
    m_re += toadd.m_re;
    m_im += toadd.m_im;
    return *this;
}

Complex& Complex::operator-= (const Complex& toadd)
{
    m_re -= toadd.m_re;
    m_im -= toadd.m_im;
    return *this;
}


const Complex Complex::operator+ (const Complex& toadd) const
{
    Complex result = *this;
    result += toadd;
    return result;
}

const Complex Complex::operator- (const Complex& toadd) const
{
    Complex result = *this;
    result -= toadd;
    return result;
}

const Complex Complex::operator* (const Complex& tomult) const
{
    Complex result = *this;
    result.m_re = m_re*tomult.m_re - m_im*tomult.m_im;
    result.m_im = m_re*tomult.m_im + m_im*tomult.m_re;
    return result;
}
