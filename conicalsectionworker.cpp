#include "conicalsectionworker.h"

#include <cmath>
#include <limits>

LinearEquation LinearEquationFromTwoPoints(const CPoint &p1, const CPoint &p2)
{
    float a = 0;
    float b = 0;
    float c = 0;

    const float dx = p2.x - p1.x;
    const float dy = p2.y - p1.y;

    a = dy;
    b = -dx;
    c = -p1.x * dy + p1.y * dx;

    return {a, b, c};
}

ConicalEquation ConicalEquationFromTwoLinearEquation(const LinearEquation &eq1, const LinearEquation &eq2)
{
    ConicalEquation conical;
    conical.a = eq1.a * eq2.a;
    conical.b = eq1.a * eq2.b + eq1.b * eq2.a;
    conical.c = eq1.b * eq2.b;
    conical.d = eq1.a * eq2.c + eq1.c * eq2.a;
    conical.e = eq1.b * eq2.c + eq1.b * eq2.c;
    conical.f = eq1.c * eq2.c;

    return conical;
}

ConicalEquation ConicalEquationFromTwoConicalEquation(const ConicalEquation &eq1, const ConicalEquation &eq2, const CPoint &fifthPoint)
{
    const float f1 = eq1.calculate(fifthPoint);
    const float f2 = eq2.calculate(fifthPoint);
    float lya = 0;
    if (std::abs(f1 - f2) > std::numeric_limits<float>::epsilon())
        lya = f1 / (f1 - f2);
    return {
                (1 - lya)*eq1.a + lya*eq2.a,
                (1 - lya)*eq1.b + lya*eq2.b,
                (1 - lya)*eq1.c + lya*eq2.c,
                (1 - lya)*eq1.d + lya*eq2.d,
                (1 - lya)*eq1.e + lya*eq2.e,
                (1 - lya)*eq1.f + lya*eq2.f
    };
}

float ConicalEquation::calculate(const CPoint &p) const
{
    const float x2 = p.x * p.x;
    const float y2 = p.y * p.y;
    const float xy = p.x * p.y;

    return a * x2 + b * xy + c * y2 + d * p.x + e * p.y + f;
}

QString ConicalEquation::getType() const
{
    const float I1 = a;
    const float I2 = a*c - (b / 2)*(b / 2);
    const float I3 = a*c*f + 2 * (b/2)*(d/2)*(e/2) - (d/2)*(d/2)*c - (b/2)*(b/2)*f - (e/2)*(e/2)*a;

    if (std::abs(I3) < std::numeric_limits<float>::epsilon()) {
        if (std::abs(I2) < std::numeric_limits<float>::epsilon()) {
            if (std::abs(I1) < std::numeric_limits<float>::epsilon())
                return "Одна прямая";
            if (I1 > 0)
                return "Нет действ. точек";
            else
                return "Пара действ. паралельных прямых";
        }
        if (I2 > 0)
            return "Эллипс, выродившийся в точку";
        return "Пара действ. пересекающихся прямых";
    } else {
        if (std::abs(I2) < std::numeric_limits<float>::epsilon()) {
            return "Парабола";
        }
        if (I2 < 0)
            return "Гипербола";
        else {
            if (I3 / I1 > 0)
                return "Нет действительных точек";
            else
                return "Эллипс";
        }
    }
}

ConicalEquation conicalEquationCoefficientsFromPoints(const CPoint &p1, const CPoint &p2, const CPoint &p3, const CPoint &p4, const CPoint &p5)
{
    LinearEquation l12 = LinearEquationFromTwoPoints(p1, p2);
    LinearEquation l34 = LinearEquationFromTwoPoints(p3, p4);
    LinearEquation l23 = LinearEquationFromTwoPoints(p2, p3);
    LinearEquation l14 = LinearEquationFromTwoPoints(p1, p4);
    ConicalEquation c1 = ConicalEquationFromTwoLinearEquation(l12, l34);
    ConicalEquation c2 = ConicalEquationFromTwoLinearEquation(l23, l14);
    ConicalEquation final = ConicalEquationFromTwoConicalEquation(c1, c2, p5);
    return final;
}


ConicalEquation conicalEquationFromTwoTangentsXYAndThreePoints(const float &xOffset, const float &yOffset, const CPoint &p1, const CPoint &p2, const CPoint &p3)
{
    LinearEquation l12 = LinearEquationFromTwoPoints(p1, p2);
    ConicalEquation c1(0, 1, 0, -yOffset, -xOffset, xOffset * yOffset);
    ConicalEquation c2 = ConicalEquationFromTwoLinearEquation(l12, l12);
    ConicalEquation final = ConicalEquationFromTwoConicalEquation(c1, c2, p3);
    return final;
}
