#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QString>

#include <algorithm>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddPolynomials);
    connect(ui->subtractButton, &QPushButton::clicked, this, &MainWindow::onSubtractPolynomials);
    connect(ui->multiplyButton, &QPushButton::clicked, this, &MainWindow::onMultiplyPolynomials);
    connect(ui->derivativeButton, &QPushButton::clicked, this, &MainWindow::onDerivativePolynomial);
    connect(ui->evaluateButton, &QPushButton::clicked, this, &MainWindow::onEvaluatePolynomial);
    connect(ui->evaluateExpressionButton, &QPushButton::clicked, this, &MainWindow::onEvaluateExpression);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAddPolynomials()
{
    if (!validatePolynomialInput(true)) {
        return;
    }

    Polynomial first(ui->polynomialInput1->text().toStdString());
    Polynomial second(ui->polynomialInput2->text().toStdString());
    Polynomial result = SumOfPolynomial(first, second);

    ui->polynomialResultLabel->setText(tr("结果：%1").arg(polynomialToDisplayString(result)));
}

void MainWindow::onSubtractPolynomials()
{
    if (!validatePolynomialInput(true)) {
        return;
    }

    Polynomial first(ui->polynomialInput1->text().toStdString());
    Polynomial second(ui->polynomialInput2->text().toStdString());
    Polynomial result = SubOfPolynomial(first, second);

    ui->polynomialResultLabel->setText(tr("结果：%1").arg(polynomialToDisplayString(result)));
}

void MainWindow::onMultiplyPolynomials()
{
    if (!validatePolynomialInput(true)) {
        return;
    }

    Polynomial first(ui->polynomialInput1->text().toStdString());
    Polynomial second(ui->polynomialInput2->text().toStdString());
    Polynomial result = MulOfPolynomial(first, second);

    ui->polynomialResultLabel->setText(tr("结果：%1").arg(polynomialToDisplayString(result)));
}

void MainWindow::onDerivativePolynomial()
{
    if (!validatePolynomialInput(false)) {
        return;
    }

    Polynomial first(ui->polynomialInput1->text().toStdString());
    Polynomial result = DerOfPolynomial(first);

    ui->polynomialResultLabel->setText(tr("导数：%1").arg(polynomialToDisplayString(result)));
}

void MainWindow::onEvaluatePolynomial()
{
    if (!validatePolynomialInput(false)) {
        return;
    }

    bool ok = false;
    int xValue = ui->xValueInput->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, tr("输入错误"), tr("请输入有效的整数 x 值"));
        return;
    }

    Polynomial first(ui->polynomialInput1->text().toStdString());
    int result = first.calculate_val(xValue);

    ui->polynomialResultLabel->setText(tr("P(%1) = %2").arg(xValue).arg(result));
}

void MainWindow::onEvaluateExpression()
{
    const QString expression = ui->expressionInput->text().trimmed();
    if (expression.isEmpty()) {
        QMessageBox::warning(this, tr("输入错误"), tr("请输入需要计算的表达式"));
        return;
    }

    AlgorithmExpression calculator;
    // 重新初始化栈，确保每次计算都从空栈开始
    while (!calculator.o_sta.empty()) {
        calculator.o_sta.pop();
    }
    while (!calculator.n_sta.empty()) {
        calculator.n_sta.pop();
    }

    const int result = calculator.calculate_expression(expression.toStdString());
    ui->expressionResultLabel->setText(tr("结果：%1").arg(result));
}

QString MainWindow::polynomialToDisplayString(const Polynomial &polynomial) const
{
    QString result;
    bool firstTerm = true;

    for (int i = polynomial.size; i >= 0; --i) {
        const int coefficient = polynomial.List[i];
        if (coefficient == 0) {
            continue;
        }

        const bool positive = coefficient > 0;
        const int absCoeff = std::abs(coefficient);

        if (firstTerm) {
            if (!positive) {
                result.append("-");
            }
            firstTerm = false;
        } else {
            result.append(positive ? " + " : " - ");
        }

        if (i == 0 || absCoeff != 1) {
            result.append(QString::number(absCoeff));
        }

        if (i >= 1) {
            result.append("x");
            if (i > 1) {
                result.append("^");
                result.append(QString::number(i));
            }
        }
    }

    if (result.isEmpty()) {
        result = QStringLiteral("0");
    }

    return result;
}

bool MainWindow::validatePolynomialInput(bool requireSecondPolynomial)
{
    const QString first = ui->polynomialInput1->text().trimmed();
    const QString second = ui->polynomialInput2->text().trimmed();

    if (first.isEmpty()) {
        QMessageBox::warning(this, tr("输入错误"), tr("请输入第一个多项式，格式如 3^0+2^1+1^2"));
        return false;
    }

    if (requireSecondPolynomial && second.isEmpty()) {
        QMessageBox::warning(this, tr("输入错误"), tr("请输入第二个多项式"));
        return false;
    }

    return true;
}
