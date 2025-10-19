#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QRegularExpression>

#include <cstdlib>

namespace {
QString sanitizeInput(const QString &text)
{
    QString sanitized = text;
    QString whitespace = QStringLiteral(" \t\n\r");
    for (QChar ch : whitespace) {
        sanitized.remove(ch);
    }
    return sanitized;
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->polyResultTextEdit->setReadOnly(true);
    ui->expressionResultLabel->setText(tr("表达式结果: "));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    Polynomial first;
    Polynomial second;
    QString error;

    if (!buildPolynomialFromInput(ui->polyInput1->text(), first, error)) {
        showPolynomialError(error);
        return;
    }
    if (!buildPolynomialFromInput(ui->polyInput2->text(), second, error)) {
        showPolynomialError(error);
        return;
    }

    Polynomial result = SumOfPolynomial(first, second);
    ui->polyResultTextEdit->setPlainText(tr("加法结果: %1").arg(formatPolynomial(result)));
}

void MainWindow::on_subtractButton_clicked()
{
    Polynomial first;
    Polynomial second;
    QString error;

    if (!buildPolynomialFromInput(ui->polyInput1->text(), first, error)) {
        showPolynomialError(error);
        return;
    }
    if (!buildPolynomialFromInput(ui->polyInput2->text(), second, error)) {
        showPolynomialError(error);
        return;
    }

    Polynomial result = SubOfPolynomial(first, second);
    ui->polyResultTextEdit->setPlainText(tr("减法结果: %1").arg(formatPolynomial(result)));
}

void MainWindow::on_multiplyButton_clicked()
{
    Polynomial first;
    Polynomial second;
    QString error;

    if (!buildPolynomialFromInput(ui->polyInput1->text(), first, error)) {
        showPolynomialError(error);
        return;
    }
    if (!buildPolynomialFromInput(ui->polyInput2->text(), second, error)) {
        showPolynomialError(error);
        return;
    }

    Polynomial result = MulOfPolynomial(first, second);
    ui->polyResultTextEdit->setPlainText(tr("乘法结果: %1").arg(formatPolynomial(result)));
}

void MainWindow::on_derivativeButton_clicked()
{
    Polynomial poly;
    QString error;

    if (!buildPolynomialFromInput(ui->polyInput1->text(), poly, error)) {
        showPolynomialError(error);
        return;
    }

    Polynomial result = DerOfPolynomial(poly);
    ui->polyResultTextEdit->setPlainText(tr("求导结果: %1").arg(formatPolynomial(result)));
}

void MainWindow::on_evaluatePolynomialButton_clicked()
{
    Polynomial poly;
    QString error;

    if (!buildPolynomialFromInput(ui->polyInput1->text(), poly, error)) {
        showPolynomialError(error);
        return;
    }

    int xValue = ui->xValueSpinBox->value();
    int value = poly.calculate_val(xValue);
    ui->polyResultTextEdit->setPlainText(tr("在 x = %1 时的值: %2").arg(xValue).arg(value));
}

void MainWindow::on_evaluateExpressionButton_clicked()
{
    QString expression = sanitizeInput(ui->expressionLineEdit->text());
    if (expression.isEmpty()) {
        QMessageBox::warning(this, tr("输入错误"), tr("请输入需要计算的表达式，例如 (2+3)*4"));
        return;
    }

    QRegularExpression pattern(QStringLiteral("^[0-9+\-*/^()]+$"));
    if (!pattern.match(expression).hasMatch()) {
        QMessageBox::warning(this, tr("输入错误"), tr("表达式仅支持数字、+、-、*、/、^ 以及括号。"));
        return;
    }

    AlgorithmExpression evaluator;
    int result = evaluator.calculate_expression(expression.toStdString());
    ui->expressionResultLabel->setText(tr("表达式结果: %1").arg(result));
}

bool MainWindow::buildPolynomialFromInput(const QString &text, Polynomial &poly, QString &error) const
{
    QString sanitized = sanitizeInput(text);
    if (sanitized.isEmpty()) {
        error = tr("请输入一个多项式，例如 3^0+2^1+1^2。");
        return false;
    }

    QRegularExpression pattern(QStringLiteral(R"(^[+\-]?\d+\^\d+([+\-]\d+\^\d+)*$)"));
    if (!pattern.match(sanitized).hasMatch()) {
        error = tr("多项式格式不正确，请使用形如 3^0+2^1+1^2 的格式。");
        return false;
    }

    Polynomial parsed(sanitized.toStdString());
    poly = parsed;
    return true;
}

QString MainWindow::formatPolynomial(const Polynomial &poly) const
{
    QString result;
    bool firstTerm = true;

    for (int i = poly.size; i >= 0; --i) {
        int coefficient = poly.List[i];
        if (coefficient == 0) {
            continue;
        }

        QString term;
        if (firstTerm) {
            if (coefficient < 0) {
                term += "-";
            }
            firstTerm = false;
        } else {
            term += (coefficient < 0) ? " - " : " + ";
        }

        int absCoeff = std::abs(coefficient);
        if (i == 0 || absCoeff != 1) {
            term += QString::number(absCoeff);
        }

        if (i > 0) {
            term += "x";
            if (i > 1) {
                term += QStringLiteral("^") + QString::number(i);
            }
        }

        result += term;
    }

    if (result.isEmpty()) {
        return QStringLiteral("0");
    }

    return result;
}

void MainWindow::showPolynomialError(const QString &message)
{
    QMessageBox::warning(this, tr("输入错误"), message);
}
