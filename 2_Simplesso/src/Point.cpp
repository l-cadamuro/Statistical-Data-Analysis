#include "Point.h"

Point::Point()
{
    //ctor
    m_x = 0;
    m_y = 0;
}

Point::Point(double x, double y)
{
    m_x = x;
    m_y = y;
}

Point::~Point()
{
    //dtor
}

double Point::GetX()
{
    return m_x;
}

double Point::GetY()
{
    return m_y;
}

void Point::SetX(double x)
{
    m_x = x;
    return;
}

void Point::SetY(double y)
{
    m_y = y;
    return;
}

void Point::SetPoint(double x, double y)
{
    m_x = x;
    m_y = y;
    return;
}

void Point::SetPoint(Point* p)
{
    m_x = p->GetX();
    m_y = p->GetY();
    return;
}
