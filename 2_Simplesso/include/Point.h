#ifndef POINT_H
#define POINT_H


class Point
{
    public:
        Point();
        Point(double x, double y);
        virtual ~Point();
        double GetX();
        double GetY();
        void SetX(double x);
        void SetY(double y);
        void SetPoint(double x, double y);
        void SetPoint(Point* p); // clona un punto
    protected:
    private:
        double m_x;
        double m_y;
};

#endif // POINT_H
