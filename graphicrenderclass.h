#ifndef GRAPHICRENDERCLASS_H
#define GRAPHICRENDERCLASS_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

class GraphicRenderClass : public QOpenGLWidget
{
    Q_OBJECT
public:
    GraphicRenderClass(QWidget *parent);

    float m_lineWidth = 0.005f;
    float m_gridWidth = 0.025f;
    float m_pointSize = 1;
    bool m_isRenderFromCoeffs = true;
    bool m_isRenderCombined = true;
    QVector<QVector2D> m_points;
    QVector2D m_squareSize;
    QVector2D m_squareOffset;

    QVector<float> m_equationCoeffsRender;
    QVector<QVector4D> m_equationCoeffsPoints;
public slots:
    void clearTransformation();

protected:
    QMatrix3x3 getAffineTransform() const;
    QMatrix3x3 getInverseAffineTransform() const;
    QMatrix3x3 pointRotationMatrix;

    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void initShaders();
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void initPlate();

    QVector3D m_backgroundColor = QVector3D(0.7, 0.7, 0.7);
    QVector3D m_fieldColor = QVector3D(0.27, 0.27, 0.27);
    QVector3D m_gridColor = QVector3D(0.7, 0.7, 0.7);
    QVector3D m_graphColor = QVector3D(0.7, 0, 0);
    QOpenGLShaderProgram m_program;
    QOpenGLShaderProgram m_program2;
    QOpenGLBuffer m_vao;
    QOpenGLBuffer m_vbo_verticles;
    QOpenGLBuffer m_vbo_textels;

    QVector2D m_scaleWindow = QVector2D(1.0f, 1.0f);
    float m_scaleGraphic = 1;
    QVector2D m_translate = QVector2D(0.0f, 0.0f);

    float *m_vertices = nullptr;

    const float m_scaleFactor = 1.1;

    bool m_firstDraw = true;
    QVector2D m_lastPos;
};

#endif // GRAPHICRENDERCLASS_H
