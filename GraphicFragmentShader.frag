#version 330
in vec2 fragPos;

uniform mat3 matrix;

uniform float lWidth;
uniform float gWidth;

uniform float sqWidth;
uniform float sqHeight;
uniform vec2 sqOffset;

uniform float rA;
uniform float rB;
uniform float rC;
uniform float rD;
uniform float rE;
uniform float rF;

uniform vec4 A;
uniform vec4 B;
uniform vec4 C;
uniform vec4 D;
uniform vec4 E;
uniform vec4 F;

uniform float gridDense;
uniform bool renderFromCoeffs;
uniform bool renderCombined;

uniform vec3 graphColor;
uniform vec3 fieldColor;
uniform vec3 gridColor;

uniform vec2 p0;
uniform vec2 p1;
uniform vec2 p2;
uniform vec2 p3;

bool isInBoundingBox(vec2 pos, int flag)
{
    if (flag == 0) {
        return (pos.x >= -sqWidth + sqOffset.x && pos.x <= p0.x)
                &&
                (pos.y >= -sqHeight + sqOffset.y && pos.y <= p1.y)
                &&
                (cross(vec3(p1 - p0, 0), vec3(pos - p0, 0)).z > 0);
    } else if (flag == 1) {
        return (pos.x >= -sqWidth + sqOffset.x && pos.x <= p3.x )
                &&
                (pos.y >= p1.y  && pos.y <= sqHeight + sqOffset.y)
                &&
                (cross(vec3(p3 - p1, 0), vec3(pos - p1, 0)).z > 0);
    } else if (flag == 2) {
        return (pos.x >= p0.x  && pos.x <= sqWidth + sqOffset.x)
                &&
                (pos.y >= -sqHeight + sqOffset.y && pos.y <= p2.y)
                &&
                (cross(vec3(p0 - p2, 0), vec3(pos - p2, 0)).z > 0);
    } else if (flag == 3) {
        return (pos.x >= p3.x && pos.x <= sqWidth + sqOffset.x)
                &&
                (pos.y >= p2.y && pos.y <= sqHeight + sqOffset.y)
                &&
                (cross(vec3(p2 - p3, 0), vec3(pos - p3, 0)).z > 0);
    } else if (flag == 4) {
        return (pos.x >= -sqWidth + sqOffset.x && pos.x <= sqWidth + sqOffset.x)
                &&
                (pos.y >= -sqHeight + sqOffset.y && pos.y <= sqHeight + sqOffset.y);
    }
    return false;
}

void main(void)
{
    vec3 pos = matrix * vec3(fragPos, 1);

    float x2 = pos.x * pos.x;
    float y2 = pos.y * pos.y;
    float xy = pos.x * pos.y;
    float x = pos.x;
    float y = pos.y;

    float rFuncValue = abs(rA*x2 + rB*xy + rC*y2 + rD*x + rE*y + rF);

    vec4 functionValue;
    functionValue.x = abs(A.x*x2 + B.x*xy + C.x*y2 + D.x*x + E.x*y + F.x);
    functionValue.y = abs(A.y*x2 + B.y*xy + C.y*y2 + D.y*x + E.y*y + F.y);
    functionValue.z = abs(A.z*x2 + B.z*xy + C.z*y2 + D.z*x + E.z*y + F.z);
    functionValue.w = abs(A.w*x2 + B.w*xy + C.w*y2 + D.w*x + E.w*y + F.w);

    if ((rFuncValue < lWidth) && renderFromCoeffs) {
        gl_FragColor = vec4(graphColor, 1);
    } else if (renderCombined && (functionValue.x < lWidth) && isInBoundingBox(pos.xy, 0)) {
        gl_FragColor = vec4(0, 0.4, 1, 1);
    } else if (renderCombined && (functionValue.y < lWidth) && isInBoundingBox(pos.xy, 1)) {
        gl_FragColor = vec4(0, 0.4, 1, 1);
    } else if (renderCombined && (functionValue.z < lWidth) && isInBoundingBox(pos.xy, 2)) {
        gl_FragColor = vec4(0, 0.4, 1, 1);
    } else if (renderCombined && (functionValue.w < lWidth) && isInBoundingBox(pos.xy, 3)) {
        gl_FragColor = vec4(0, 0.4, 1, 1);
    } else {
        vec4 finalColor;
        vec2 scaledPoint = (pos.xy) / gridDense;
        vec2 scaledPoint2 = (pos.xy) / gridDense / 10;
        if (abs(scaledPoint2.x) - trunc(abs(scaledPoint2.x)) < gWidth / 5 || abs(scaledPoint2.y) - trunc(abs(scaledPoint2.y)) < gWidth / 5)
            finalColor = vec4(gridColor, 1);
        else if (abs(scaledPoint.x) - trunc(abs(scaledPoint.x)) < gWidth || abs(scaledPoint.y) - trunc(abs(scaledPoint.y)) < gWidth) {
            finalColor = vec4(gridColor * 0.6, 1);
        } else
            finalColor = vec4(fieldColor, 1);
        if (isInBoundingBox(pos.xy, 4))
            gl_FragColor = finalColor;
        else
            gl_FragColor = finalColor * 0.5;

    }
}
