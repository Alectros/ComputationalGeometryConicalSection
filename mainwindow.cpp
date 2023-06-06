#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "conicalsectionworker.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sliderFieldSize->setRange(1, 1000);
    ui->sliderGraphSize->setRange(1, 1000);

    connect(ui->spinCoeffA, SIGNAL(valueChanged(double)), this, SLOT(coefficientsChanged()));
    connect(ui->spinCoeffB, SIGNAL(valueChanged(double)), this, SLOT(coefficientsChanged()));
    connect(ui->spinCoeffC, SIGNAL(valueChanged(double)), this, SLOT(coefficientsChanged()));
    connect(ui->spinCoeffD, SIGNAL(valueChanged(double)), this, SLOT(coefficientsChanged()));
    connect(ui->spinCoeffE, SIGNAL(valueChanged(double)), this, SLOT(coefficientsChanged()));
    connect(ui->spinCoeffF, SIGNAL(valueChanged(double)), this, SLOT(coefficientsChanged()));
    connect(ui->sliderGraphSize, SIGNAL(valueChanged(int)), this, SLOT(lineWidthChanged()));
    connect(ui->sliderFieldSize, SIGNAL(valueChanged(int)), this, SLOT(gridWidthChanged()));
    connect(ui->checkBoxGraphCoeff, SIGNAL(stateChanged(int)), this, SLOT(switchRenderFromCoeffMode()));
    connect(ui->checkBoxGraphPoints, SIGNAL(stateChanged(int)), this, SLOT(switchRenderCombinedMode()));
    connect(ui->buttonInitCoeffs, &QPushButton::clicked, this, &::MainWindow::resetCoeficients);
    connect(ui->buttInitTransf, &QPushButton::clicked, this, &::MainWindow::resetTransformation);

    connect(ui->spinPoint0X, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint1X, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint2X, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint3X, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint4X, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint5X, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint6X, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint7X, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint0Y, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint1Y, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint2Y, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint3Y, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint4Y, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint5Y, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint6Y, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));
    connect(ui->spinPoint7Y, SIGNAL(valueChanged(double)), this, SLOT(pointsCoordsChanged()));

    connect(ui->squareHeight, SIGNAL(valueChanged(double)), this, SLOT(squareParamsUpdate()));
    connect(ui->squareWidth, SIGNAL(valueChanged(double)), this, SLOT(squareParamsUpdate()));
    connect(ui->squareOffsetX, SIGNAL(valueChanged(double)), this, SLOT(squareParamsUpdate()));
    connect(ui->squareOffsetY, SIGNAL(valueChanged(double)), this, SLOT(squareParamsUpdate()));

    connect(ui->buttonCalculate, &QPushButton::clicked, this, &::MainWindow::calculateKoeffs);

    coefficientsChanged();
    switchRenderFromCoeffMode();
    switchRenderCombinedMode();
    resetTransformation();
    resetCoeficients();
    pointsCoordsChanged();
    calculateKoeffs();
    squareParamsUpdate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::coefficientsChanged()
{
    QVector<float> coeffs(6);
    coeffs[0] = ui->spinCoeffA->value();
    coeffs[1] = ui->spinCoeffB->value();
    coeffs[2] = ui->spinCoeffC->value();
    coeffs[3] = ui->spinCoeffD->value();
    coeffs[4] = ui->spinCoeffE->value();
    coeffs[5] = ui->spinCoeffF->value();

    ui->openGLWidget->m_equationCoeffsRender = coeffs;
}

void MainWindow::lineWidthChanged()
{
    ui->openGLWidget->m_lineWidth = static_cast<float>(ui->sliderGraphSize->value()) / 10000.0;
}

void MainWindow::gridWidthChanged()
{
    ui->openGLWidget->m_gridWidth = static_cast<float>(ui->sliderFieldSize->value()) / 2000.0;
}

void MainWindow::switchRenderFromCoeffMode()
{
    ui->openGLWidget->m_isRenderFromCoeffs = ui->checkBoxGraphCoeff->isChecked();
}

void MainWindow::switchRenderCombinedMode()
{
    ui->openGLWidget->m_isRenderCombined = ui->checkBoxGraphPoints->isChecked();
}

void MainWindow::resetTransformation()
{
    ui->openGLWidget->clearTransformation();
}

void MainWindow::resetCoeficients()
{
    ui->openGLWidget->m_equationCoeffsRender = {1, 0, 0, 0, -1, 0};
    ui->spinCoeffA->setValue(1);
    ui->spinCoeffB->setValue(0);
    ui->spinCoeffC->setValue(0);
    ui->spinCoeffD->setValue(0);
    ui->spinCoeffE->setValue(-1);
    ui->spinCoeffF->setValue(0);
}

void MainWindow::pointsCoordsChanged()
{
    ui->openGLWidget->m_points[0].setX(ui->spinPoint0X->value());
    ui->openGLWidget->m_points[0].setY(ui->spinPoint0Y->value());
    ui->openGLWidget->m_points[1].setX(ui->spinPoint1X->value());
    ui->openGLWidget->m_points[1].setY(ui->spinPoint1Y->value());
    ui->openGLWidget->m_points[2].setX(ui->spinPoint2X->value());
    ui->openGLWidget->m_points[2].setY(ui->spinPoint2Y->value());
    ui->openGLWidget->m_points[3].setX(ui->spinPoint3X->value());
    ui->openGLWidget->m_points[3].setY(ui->spinPoint3Y->value());
    ui->openGLWidget->m_points[4].setX(ui->spinPoint4X->value());
    ui->openGLWidget->m_points[4].setY(ui->spinPoint4Y->value());
    ui->openGLWidget->m_points[5].setX(ui->spinPoint5X->value());
    ui->openGLWidget->m_points[5].setY(ui->spinPoint5Y->value());
    ui->openGLWidget->m_points[6].setX(ui->spinPoint6X->value());
    ui->openGLWidget->m_points[6].setY(ui->spinPoint6Y->value());
    ui->openGLWidget->m_points[7].setX(ui->spinPoint7X->value());
    ui->openGLWidget->m_points[7].setY(ui->spinPoint7Y->value());
}

void MainWindow::calculateKoeffs()
{
    ConicalEquation con0 = conicalEquationFromTwoTangentsXYAndThreePoints(
                -ui->squareWidth->value() + ui->squareOffsetX->value(), -ui->squareHeight->value() + ui->squareOffsetY->value(),
                {static_cast<float>(ui->spinPoint0X->value()), static_cast<float>(ui->spinPoint0Y->value())},
                {static_cast<float>(ui->spinPoint1X->value()), static_cast<float>(ui->spinPoint1Y->value())},
                {static_cast<float>(ui->spinPoint4X->value()), static_cast<float>(ui->spinPoint4Y->value())}
                 );

    ConicalEquation con1 = conicalEquationFromTwoTangentsXYAndThreePoints(
                -ui->squareWidth->value() + ui->squareOffsetX->value(), ui->squareHeight->value() + ui->squareOffsetY->value(),
                {static_cast<float>(ui->spinPoint1X->value()), static_cast<float>(ui->spinPoint1Y->value())},
                {static_cast<float>(ui->spinPoint3X->value()), static_cast<float>(ui->spinPoint3Y->value())},
                {static_cast<float>(ui->spinPoint5X->value()), static_cast<float>(ui->spinPoint5Y->value())}
                 );

    ConicalEquation con3 = conicalEquationFromTwoTangentsXYAndThreePoints(
                ui->squareWidth->value() + ui->squareOffsetX->value(), ui->squareHeight->value() + ui->squareOffsetY->value(),
                {static_cast<float>(ui->spinPoint2X->value()), static_cast<float>(ui->spinPoint2Y->value())},
                {static_cast<float>(ui->spinPoint3X->value()), static_cast<float>(ui->spinPoint3Y->value())},
                {static_cast<float>(ui->spinPoint7X->value()), static_cast<float>(ui->spinPoint7Y->value())}
                 );

    ConicalEquation con2 = conicalEquationFromTwoTangentsXYAndThreePoints(
                ui->squareWidth->value() + ui->squareOffsetX->value(), -ui->squareHeight->value() + ui->squareOffsetY->value(),
                {static_cast<float>(ui->spinPoint0X->value()), static_cast<float>(ui->spinPoint0Y->value())},
                {static_cast<float>(ui->spinPoint2X->value()), static_cast<float>(ui->spinPoint2Y->value())},
                {static_cast<float>(ui->spinPoint6X->value()), static_cast<float>(ui->spinPoint6Y->value())}
                 );

    ui->openGLWidget->m_equationCoeffsPoints[0][0] = con0.a;
    ui->openGLWidget->m_equationCoeffsPoints[1][0] = con0.b;
    ui->openGLWidget->m_equationCoeffsPoints[2][0] = con0.c;
    ui->openGLWidget->m_equationCoeffsPoints[3][0] = con0.d;
    ui->openGLWidget->m_equationCoeffsPoints[4][0] = con0.e;
    ui->openGLWidget->m_equationCoeffsPoints[5][0] = con0.f;

    ui->openGLWidget->m_equationCoeffsPoints[0][1] = con1.a;
    ui->openGLWidget->m_equationCoeffsPoints[1][1] = con1.b;
    ui->openGLWidget->m_equationCoeffsPoints[2][1] = con1.c;
    ui->openGLWidget->m_equationCoeffsPoints[3][1] = con1.d;
    ui->openGLWidget->m_equationCoeffsPoints[4][1] = con1.e;
    ui->openGLWidget->m_equationCoeffsPoints[5][1] = con1.f;

    ui->openGLWidget->m_equationCoeffsPoints[0][2] = con2.a;
    ui->openGLWidget->m_equationCoeffsPoints[1][2] = con2.b;
    ui->openGLWidget->m_equationCoeffsPoints[2][2] = con2.c;
    ui->openGLWidget->m_equationCoeffsPoints[3][2] = con2.d;
    ui->openGLWidget->m_equationCoeffsPoints[4][2] = con2.e;
    ui->openGLWidget->m_equationCoeffsPoints[5][2] = con2.f;

    ui->openGLWidget->m_equationCoeffsPoints[0][3] = con3.a;
    ui->openGLWidget->m_equationCoeffsPoints[1][3] = con3.b;
    ui->openGLWidget->m_equationCoeffsPoints[2][3] = con3.c;
    ui->openGLWidget->m_equationCoeffsPoints[3][3] = con3.d;
    ui->openGLWidget->m_equationCoeffsPoints[4][3] = con3.e;
    ui->openGLWidget->m_equationCoeffsPoints[5][3] = con3.f;

    const QString templateForCoeffs = "Type: %1\n"
                                      "-----------------\n"
                                      "A: %2\n"
                                      "B: %3\n"
                                      "C: %4\n"
                                      "D: %5\n"
                                      "E: %6\n"
                                      "F: %7";

    ui->text0->setText(templateForCoeffs.arg(con0.getType(),
                                             QString::number(con0.a),
                                             QString::number(con0.b),
                                             QString::number(con0.c),
                                             QString::number(con0.d),
                                             QString::number(con0.e),
                                             QString::number(con0.f)));

    ui->text1->setText(templateForCoeffs.arg(con1.getType(),
                                             QString::number(con1.a),
                                             QString::number(con1.b),
                                             QString::number(con1.c),
                                             QString::number(con1.d),
                                             QString::number(con1.e),
                                             QString::number(con1.f)));

    ui->text2->setText(templateForCoeffs.arg(con2.getType(),
                                             QString::number(con2.a),
                                             QString::number(con2.b),
                                             QString::number(con2.c),
                                             QString::number(con2.d),
                                             QString::number(con2.e),
                                             QString::number(con2.f)));

    ui->text3->setText(templateForCoeffs.arg(con3.getType(),
                                             QString::number(con3.a),
                                             QString::number(con3.b),
                                             QString::number(con3.c),
                                             QString::number(con3.d),
                                             QString::number(con3.e),
                                             QString::number(con3.f)));

}

void MainWindow::squareParamsUpdate()
{
    ui->openGLWidget->m_squareSize.setX(ui->squareWidth->value());
    ui->openGLWidget->m_squareSize.setY(ui->squareHeight->value());
    ui->openGLWidget->m_squareOffset.setX(ui->squareOffsetX->value());
    ui->openGLWidget->m_squareOffset.setY(ui->squareOffsetY->value());
}

