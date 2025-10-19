#pragma once

#include <QMainWindow>

#include "AlgorithmExpression.h"
#include "polynomial.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onAddPolynomials();
    void onSubtractPolynomials();
    void onMultiplyPolynomials();
    void onDerivativePolynomial();
    void onEvaluatePolynomial();
    void onEvaluateExpression();

private:
    QString polynomialToDisplayString(const Polynomial &polynomial) const;
    bool validatePolynomialInput(bool requireSecondPolynomial);
    Ui::MainWindow *ui;
};
