#ifndef COMPLEX_H
#define COMPLEX_H

// classe che definisce un numero complesso


class Complex
{
    public:
        Complex();
        Complex(double re, double im);
        virtual ~Complex();
        void SetPolar(double rho, double theta);
        void SetRe(double re);
        void SetIm(double im);
        double GetRe();
        double GetIm();
        double GetRho();
        double GetTheta();
        Complex& Conjugate();
        double GetSquareModulus();
        Complex& operator= (const Complex& original);
        Complex& operator+= (const Complex& toadd);
        Complex& operator-= (const Complex& toadd);
        const Complex operator+ (const Complex& toadd) const;
        const Complex operator- (const Complex& toadd) const;
        const Complex operator* (const Complex& tomult) const;

    protected:
    private:
        double m_re;
        double m_im;
};

#endif // COMPLEX_H
