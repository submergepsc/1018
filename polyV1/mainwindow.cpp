#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QRegularExpression>
#include <QStringList>

#include <cmath>
#include <exception>
#include <optional>

namespace {
QString trimmedPolynomial(const QString &input)
{
    QString normalized = input;
    normalized.remove(' ');
    return normalized;
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->resultTextEdit->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    if (!validatePolynomialInputs()) {
        return;
    }

    auto p1 = buildPolynomialFromInput(1);
    if (!p1) {
        return;
    }
    auto p2 = buildPolynomialFromInput(2);
    if (!p2) {
        return;
    }

    try {
        Polynomial result = SumOfPolynomial(*p1, *p2);
        displayResult(tr("加法结果"), polynomialToDisplayString(result));
    } catch (const std::exception &ex) {
        showError(tr("加法计算失败：%1").arg(QString::fromUtf8(ex.what())));
    }
}

void MainWindow::on_subtractButton_clicked()
{
    if (!validatePolynomialInputs()) {
        return;
    }

    auto p1 = buildPolynomialFromInput(1);
    if (!p1) {
        return;
    }
    auto p2 = buildPolynomialFromInput(2);
    if (!p2) {
        return;
    }

    try {
        Polynomial result = SubOfPolynomial(*p1, *p2);
        displayResult(tr("减法结果"), polynomialToDisplayString(result));
    } catch (const std::exception &ex) {
        showError(tr("减法计算失败：%1").arg(QString::fromUtf8(ex.what())));
    }
}

void MainWindow::on_multiplyButton_clicked()
{
    if (!validatePolynomialInputs()) {
        return;
    }

    auto p1 = buildPolynomialFromInput(1);
    if (!p1) {
        return;
    }
    auto p2 = buildPolynomialFromInput(2);
    if (!p2) {
        return;
    }

    try {
        Polynomial result = MulOfPolynomial(*p1, *p2);
        displayResult(tr("乘法结果"), polynomialToDisplayString(result));
    } catch (const std::exception &ex) {
        showError(tr("乘法计算失败：%1").arg(QString::fromUtf8(ex.what())));
    }
}

void MainWindow::on_derivativeButton_clicked()
{
    if (!validatePolynomialInputs(false)) {
        return;
    }

    auto poly = buildPolynomialFromInput(1);
    if (!poly) {
        return;
    }

    try {
        Polynomial result = DerOfPolynomial(*poly);
        displayResult(tr("一阶导数"), polynomialToDisplayString(result));
    } catch (const std::exception &ex) {
        showError(tr("求导计算失败：%1").arg(QString::fromUtf8(ex.what())));
    }
}

void MainWindow::on_evaluateButton_clicked()
{
    if (!validatePolynomialInputs(false)) {
        return;
    }

    bool ok = false;
    int xValue = ui->xValueLineEdit->text().trimmed().toInt(&ok);
    if (!ok) {
        showError(tr("请输入一个整数形式的 x 值"));
        return;
    }

    auto poly = buildPolynomialFromInput(1);
    if (!poly) {
        return;
    }

    try {
        int value = poly->calculate_val(xValue);
        displayResult(tr("计算结果"), tr("P(%1) = %2").arg(xValue).arg(value));
    } catch (const std::exception &ex) {
        showError(tr("代入计算失败：%1").arg(QString::fromUtf8(ex.what())));
    }
}

void MainWindow::on_expressionEvalButton_clicked()
{
    const QString expressionText = ui->expressionLineEdit->text().trimmed();
    if (expressionText.isEmpty()) {
        showError(tr("请输入要计算的算术表达式"));
        return;
    }

    try {
        int result = expressionCalculator_.calculate_expression(expressionText.toStdString());
        displayResult(tr("算术表达式结果"),
                      tr("%1 = %2").arg(expressionText, QString::number(result)));
    } catch (const std::exception &ex) {
        showError(tr("表达式无效：%1").arg(QString::fromUtf8(ex.what())));
    }
}

bool MainWindow::validatePolynomialInputs(bool requireSecond)
{
    QString p1Text = getPolynomialText(1);
    if (p1Text.isEmpty()) {
        showError(tr("请输入第一个多项式"));
        return false;
    }

    if (!validatePolynomialFormat(p1Text)) {
        showError(tr("第一个多项式格式不正确，请使用形如 3^0+2^1+1^2 的格式"));
        return false;
    }

    if (requireSecond) {
        QString p2Text = getPolynomialText(2);
        if (p2Text.isEmpty()) {
            showError(tr("请输入第二个多项式"));
            return false;
        }
        if (!validatePolynomialFormat(p2Text)) {
            showError(tr("第二个多项式格式不正确，请使用形如 3^0+2^1+1^2 的格式"));
            return false;
        }
    }

    return true;
}

bool MainWindow::validatePolynomialFormat(const QString &text) const
{
    QRegularExpression pattern(QStringLiteral("^[-+]?\\d+\\^\\d+(?:[-+]\\d+\\^\\d+)*$"));
    return pattern.match(text).hasMatch();
}

void MainWindow::showError(const QString &message)
{
    QMessageBox::warning(this, tr("输入错误"), message);
}

QString MainWindow::getPolynomialText(int index) const
{
    QString text;
    if (index == 1) {
        text = ui->poly1LineEdit->text();
    } else {
        text = ui->poly2LineEdit->text();
    }
    return trimmedPolynomial(text);
}

std::optional<Polynomial> MainWindow::buildPolynomialFromInput(int index)
{
    try {
        return Polynomial(getPolynomialText(index).toStdString());
    } catch (const std::exception &ex) {
        showError(tr("解析第%1个多项式失败：%2").arg(index).arg(QString::fromUtf8(ex.what())));
        return std::nullopt;
    }
}

void MainWindow::displayResult(const QString &title, const QString &content)
{
    ui->resultTextEdit->appendPlainText(QStringLiteral("[%1]\n%2\n").arg(title, content));
}

QString MainWindow::polynomialToDisplayString(const Polynomial &poly) const
{
    QStringList terms;
    if (poly.term_count() == 0) {
        return QStringLiteral("0");
    }

    for (int i = poly.degree(); i >= 0; --i) {
        int coef = poly.coefficient(static_cast<std::size_t>(i));
        if (coef == 0) {
            continue;
        }

        QString term;
        if (!terms.isEmpty()) {
            term += coef > 0 ? QStringLiteral("+") : QStringLiteral("-");
        } else if (coef < 0) {
            term += QStringLiteral("-");
        }

        int absCoef = std::abs(coef);
        if (i == 0 || absCoef != 1) {
            term += QString::number(absCoef);
        }

        if (i >= 1) {
            term += QStringLiteral("x");
            if (i > 1) {
                term += QStringLiteral("^") + QString::number(i);
            }
        }

        terms.append(term);
    }

    if (terms.isEmpty()) {
        return QStringLiteral("0");
    }

    return terms.join(QString());
}
