#ifndef CONICALSECTIONWORKER_H
#define CONICALSECTIONWORKER_H

#include <QVector>

class CPoint
{
public:
    CPoint(const float &x, const float &y) :x(x), y(y) {};
    float x = 0;
    float y = 0;
};

//! Ax + By + C = 0
class LinearEquation
{
public:
    LinearEquation(const float &a, const float &b, const float &c) :a(a), b(b), c(c) {};
    float a = 0;
    float b = 0;
    float c = 0;
};

struct ConicalEquation
{
    ConicalEquation() {};
    ConicalEquation(
            const float &a,
            const float &b,
            const float &c,
            const float &d,
            const float &e,
            const float &f) :
        a(a),
        b(b),
        c(c),
        d(d),
        e(e),
        f(f)
    {};
    float a = 0;
    float b = 0;
    float c = 0;
    float d = 0;
    float e = 0;
    float f = 0;

    float calculate(const CPoint &p) const;

    QString getType() const;
};

//! Ax + By + C = 0 -> linear equation
//! Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0 -> conical equation
LinearEquation LinearEquationFromTwoPoints(const CPoint &p1, const CPoint &p2);
ConicalEquation ConicalEquationFromTwoLinearEquation(const LinearEquation &eq1, const LinearEquation &eq2);
ConicalEquation ConicalEquationFromTwoConicalEquation(const ConicalEquation &eq1, const ConicalEquation &eq2, const CPoint &fifthPoint);
ConicalEquation conicalEquationCoefficientsFromPoints(const CPoint &p1, const CPoint &p2, const CPoint &p3, const CPoint &p4, const CPoint &p5);
ConicalEquation conicalEquationFromTwoTangentsXYAndThreePoints(const float &xOffset, const float &yOffset, const CPoint &p1, const CPoint &p2, const CPoint &p3);

#endif // CONICALSECTIONWORKER_H
