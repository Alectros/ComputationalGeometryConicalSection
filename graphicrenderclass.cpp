#include "graphicrenderclass.h"

#include <QPainter>
#include <QWheelEvent>
#include <QPointF>

GraphicRenderClass::GraphicRenderClass(QWidget *parent) : QOpenGLWidget(parent)
{
    setMinimumSize({800, 800});
    setMaximumSize({800, 800});
    m_equationCoeffsRender = {1, 0, 0, 0, -1, 0};
    m_equationCoeffsPoints = {
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {-1, -1, -1, -1},
        {0, 0, 0, 0}
    };
    pointRotationMatrix(0, 0) = 1e-2;
    pointRotationMatrix(1, 1) = 1e-2;
    pointRotationMatrix(0, 1) = 1e-2;
    pointRotationMatrix(1, 0) = -1e-2;

    m_points = QVector<QVector2D>(8);
    m_equationCoeffsPoints = QVector<QVector4D>(6);
    m_equationCoeffsRender = QVector<float>(6);
    m_squareSize = {0.5f, 0.5f};
}

void GraphicRenderClass::clearTransformation()
{
    m_scaleGraphic = 1;
    m_translate = QVector2D(0.0f, 0.0f);
}

QMatrix3x3 GraphicRenderClass::getAffineTransform() const
{
    QMatrix3x3 affine;
    affine(0, 0) = m_scaleWindow[0] * m_scaleGraphic;
    affine(1, 1) = m_scaleWindow[1] * m_scaleGraphic;
    affine(0, 2) = m_translate[0];
    affine(1, 2) = m_translate[1];
    return affine;
}

QMatrix3x3 GraphicRenderClass::getInverseAffineTransform() const
{
    QMatrix3x3 affine;
    affine(0, 0) = 1 / (m_scaleGraphic / m_scaleWindow[0]);
    affine(1, 1) = 1 / (m_scaleGraphic / m_scaleWindow[1]);
    return affine;
}

void GraphicRenderClass::initializeGL()
{
    glClearColor(m_backgroundColor[0],m_backgroundColor[1], m_backgroundColor[2], 1);
    initShaders();
    initPlate();
    clearTransformation();
}

void GraphicRenderClass::resizeGL(int width, int height)
{
    if (width == 0 || height == 0)
        return;
    if (width > height) {
        m_scaleWindow[0] = 1;
        m_scaleWindow[1] = static_cast<float>(height) / width;
    } else {
        m_scaleWindow[0] = static_cast<float>(width) / height;
        m_scaleWindow[1] = 1;
    }
}

void GraphicRenderClass::paintGL()
{
    glClearColor(m_backgroundColor[0],m_backgroundColor[1], m_backgroundColor[2], 1);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program.bind();
    m_vao.bind();
    const QMatrix3x3 model = getAffineTransform();
    auto modelLocation = m_program.uniformLocation("matrix");
    m_program.setUniformValue(modelLocation, model);
    auto lWidthLocation = m_program.uniformLocation("lWidth");
    m_program.setUniformValue(lWidthLocation, m_lineWidth);
    auto denseLocation = m_program.uniformLocation("gridDense");
    m_program.setUniformValue(denseLocation, 0.1f);
    auto gridLocation = m_program.uniformLocation("gWidth");
    m_program.setUniformValue(gridLocation, m_gridWidth);
    auto ALocation = m_program.uniformLocation("rA");
    m_program.setUniformValue(ALocation, m_equationCoeffsRender[0]);
    auto BLocation = m_program.uniformLocation("rB");
    m_program.setUniformValue(BLocation, m_equationCoeffsRender[1]);
    auto CLocation = m_program.uniformLocation("rC");
    m_program.setUniformValue(CLocation, m_equationCoeffsRender[2]);
    auto DLocation = m_program.uniformLocation("rD");
    m_program.setUniformValue(DLocation, m_equationCoeffsRender[3]);
    auto ELocation = m_program.uniformLocation("rE");
    m_program.setUniformValue(ELocation, m_equationCoeffsRender[4]);
    auto FLocation = m_program.uniformLocation("rF");
    m_program.setUniformValue(FLocation, m_equationCoeffsRender[5]);

    auto vALocation = m_program.uniformLocation("A");
    m_program.setUniformValue(vALocation, m_equationCoeffsPoints[0]);
    auto vBLocation = m_program.uniformLocation("B");
    m_program.setUniformValue(vBLocation, m_equationCoeffsPoints[1]);
    auto vCLocation = m_program.uniformLocation("C");
    m_program.setUniformValue(vCLocation, m_equationCoeffsPoints[2]);
    auto vDLocation = m_program.uniformLocation("D");
    m_program.setUniformValue(vDLocation, m_equationCoeffsPoints[3]);
    auto vELocation = m_program.uniformLocation("E");
    m_program.setUniformValue(vELocation, m_equationCoeffsPoints[4]);
    auto vFLocation = m_program.uniformLocation("F");
    m_program.setUniformValue(vFLocation, m_equationCoeffsPoints[5]);


    auto sqWLocation = m_program.uniformLocation("sqWidth");
    m_program.setUniformValue(sqWLocation, m_squareSize.x());
    auto sqHLocation = m_program.uniformLocation("sqHeight");
    m_program.setUniformValue(sqHLocation, m_squareSize.y());
    auto sqOLocation = m_program.uniformLocation("sqOffset");
    m_program.setUniformValue(sqOLocation, m_squareOffset);

    auto fieldColorLocation = m_program.uniformLocation("fieldColor");
    m_program.setUniformValue(fieldColorLocation, m_fieldColor);
    auto gridColorLocation = m_program.uniformLocation("gridColor");
    m_program.setUniformValue(gridColorLocation, m_gridColor);
    auto graphColorLocation = m_program.uniformLocation("graphColor");
    m_program.setUniformValue(graphColorLocation, m_graphColor);

    auto renderSingleLocation = m_program.uniformLocation("renderFromCoeffs");
    m_program.setUniformValue(renderSingleLocation, m_isRenderFromCoeffs);

    auto renderCombsLocation = m_program.uniformLocation("renderCombined");
    m_program.setUniformValue(renderCombsLocation, m_isRenderCombined);

    for(int i = 0; i < 4; i++) {
        auto pointC1Location = m_program.uniformLocation(QString("p%1").arg(i));
        m_program.setUniformValue(pointC1Location, m_points[i]);
    }

    glDrawArrays(GL_TRIANGLES, 0, 6);

    m_program2.bind();
    const QMatrix3x3 model2 = pointRotationMatrix * getInverseAffineTransform();
    auto firstColor2Location = m_program2.uniformLocation("color");
    auto model2Location = m_program2.uniformLocation("matrix");
    for(int i = 0; i < 8; i++) {
        m_program2.setUniformValue(firstColor2Location, QVector3D({static_cast<float>(i / 4), static_cast<float>(i / 2 % 2), static_cast<float>(i % 2)}));
        QMatrix3x3 tr;
        tr(0, 2) += (m_points[i].x() - m_translate.x() / m_scaleWindow[0]) / m_scaleGraphic;
        tr(1, 2) += (m_points[i].y() - m_translate.y() / m_scaleWindow[1]) / m_scaleGraphic;
        m_program2.setUniformValue(model2Location, tr * model2);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    this->update();
}

void GraphicRenderClass::initShaders()
{
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, "../VuchGeomTask1/graphicVertexSh.vert")) {
        qDebug() << QString("error in vertex shader: %1\n").arg(m_program.log());
        close();
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, "../VuchGeomTask1/GraphicFragmentShader.frag")){
        qDebug() << QString("error in fragment shader: %1\n").arg(m_program.log());
        close();
    }
    if(!m_program.link()) {
        qDebug() << QString("error in link: %1\n").arg(m_program.log());
        close();
    }

    if(!m_program2.addShaderFromSourceFile(QOpenGLShader::Vertex, "../VuchGeomTask1/DrawPointVertexShader.vert")) {
        qDebug() << QString("error in vertex shader: %1\n").arg(m_program.log());
        close();
    }
    if (!m_program2.addShaderFromSourceFile(QOpenGLShader::Fragment, "../VuchGeomTask1/DrawPointsFragmentShader.frag")){
        qDebug() << QString("error in fragment shader: %1\n").arg(m_program.log());
        close();
    }
    if(!m_program2.link()) {
        qDebug() << QString("error in link: %1\n").arg(m_program.log());
        close();
    }
}

void GraphicRenderClass::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons()) {
        if (m_firstDraw) {
            m_firstDraw = false;
        } else {
            QVector2D current;
            current[0] = (float) event->x() / this->width();
            current[1] = (float) (this->height() - event->y()) / this->height();

            QVector2D movement = 2 * (current - m_lastPos);

            movement[0] /= (m_scaleWindow[0] / m_scaleGraphic);
            movement[1] /= (m_scaleWindow[1] / m_scaleGraphic);

            m_translate -= movement;
        }
        m_lastPos[0] = (float) event->x() / this->width();
        m_lastPos[1] = (float)(this->height() - event->y()) / this->height();
    }
}

void GraphicRenderClass::mouseReleaseEvent(QMouseEvent *event)
{
    m_firstDraw = true;
}

void GraphicRenderClass::wheelEvent(QWheelEvent *event)
{
    const float ticks = event->angleDelta().y() / 120;
    if(ticks > 0)
        m_scaleGraphic *= m_scaleFactor;
    else
        m_scaleGraphic /= m_scaleFactor;
}

void GraphicRenderClass::initPlate()
{
    m_vertices = new float[12];
    m_vertices[0] = -1.0;
    m_vertices[1] = -1.0;

    m_vertices[2] =  1.0;
    m_vertices[3] = -1.0;

    m_vertices[4] = -1.0;
    m_vertices[5] =  1.0;

    m_vertices[6] =  1.0;
    m_vertices[7] = -1.0;

    m_vertices[8] = -1.0;
    m_vertices[9] =  1.0;

    m_vertices[10] = 1.0;
    m_vertices[11] = 1.0;

    m_vao.create();
    m_vao.bind();

    m_vbo_verticles.create();
    m_vbo_verticles.bind();
    m_vbo_verticles.setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_vbo_verticles.allocate(m_vertices, 12 * sizeof(float));

    const auto attributePosition = m_program.attributeLocation("position");

    m_program.enableAttributeArray(attributePosition);

    m_program.setAttributeBuffer(attributePosition,GL_FLOAT, 0, 2);
    m_vbo_verticles.release();

    m_vao.release();
}
