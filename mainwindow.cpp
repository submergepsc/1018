#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "polynomial/polynomial.h"

#include <QMessageBox>
#include <QStringList>

namespace {
QString formatTerm(int coefficient, int exponent)
{
    if (coefficient == 0) {
        return {};
    }

    QString prefix;
    if (exponent == 0) {
        prefix = QString::number(coefficient);
    } else if (exponent == 1) {
        if (coefficient == 1) {
            prefix = QStringLiteral("x");
        } else if (coefficient == -1) {
            prefix = QStringLiteral("-x");
        } else {
            prefix = QString::number(coefficient) + QStringLiteral("x");
        }
    } else {
        if (coefficient == 1) {
            prefix = QStringLiteral("x^") + QString::number(exponent);
        } else if (coefficient == -1) {
            prefix = QStringLiteral("-x^") + QString::number(exponent);
        } else {
            prefix = QString::number(coefficient) + QStringLiteral("x^") + QString::number(exponent);
        }
    }

    return prefix;
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->resultTextEdit->setReadOnly(true);
    ui->valueSpinBox->setMinimum(-1000);
    ui->valueSpinBox->setMaximum(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::tryBuildPolynomial(const QString &text, Polynomial &poly)
{
    QString trimmed = text.trimmed();
    if (trimmed.isEmpty()) {
        return false;
    }

    QString sanitized = trimmed;
    sanitized.remove(' ');
    sanitized.remove('\t');
    sanitized.remove('\n');
    sanitized.remove('\r');

    poly = Polynomial(sanitized.toStdString());
    return true;
}

QString MainWindow::polynomialToDisplayString(const Polynomial &poly) const
{
    QStringList terms;
    for (int exponent = poly.size; exponent >= 0; --exponent) {
        int coefficient = poly.List[exponent];
        if (coefficient == 0) {
            continue;
        }

        QString term = formatTerm(coefficient, exponent);
        if (!term.isEmpty()) {
            terms.append(term);
        }
    }

    if (terms.isEmpty()) {
        return QStringLiteral("0");
    }

    QString result = terms.first();
    for (int i = 1; i < terms.size(); ++i) {
        if (terms[i].startsWith('-')) {
            result += QStringLiteral(" ") + terms[i];
        } else {
            result += QStringLiteral(" + ") + terms[i];
        }
    }
    return result;
}

void MainWindow::showPolynomialResult(const QString &operation, const Polynomial &result)
{
    QString display = polynomialToDisplayString(result);
    ui->resultTextEdit->append(QStringLiteral("%1: %2").arg(operation, display));
}

void MainWindow::on_addButton_clicked()
{
    Polynomial first;
    Polynomial second;

    if (!tryBuildPolynomial(ui->firstPolynomialEdit->text(), first) ||
        !tryBuildPolynomial(ui->secondPolynomialEdit->text(), second)) {
        QMessageBox::warning(this, tr("Invalid input"), tr("Please enter both polynomials in the format 3^0+2^1+1^2."));
        return;
    }

    Polynomial result = SumOfPolynomial(first, second);
    showPolynomialResult(tr("Sum"), result);
}

void MainWindow::on_subtractButton_clicked()
{
    Polynomial first;
    Polynomial second;

    if (!tryBuildPolynomial(ui->firstPolynomialEdit->text(), first) ||
        !tryBuildPolynomial(ui->secondPolynomialEdit->text(), second)) {
        QMessageBox::warning(this, tr("Invalid input"), tr("Please enter both polynomials in the format 3^0+2^1+1^2."));
        return;
    }

    Polynomial result = SubOfPolynomial(first, second);
    showPolynomialResult(tr("Difference"), result);
}

void MainWindow::on_multiplyButton_clicked()
{
    Polynomial first;
    Polynomial second;

    if (!tryBuildPolynomial(ui->firstPolynomialEdit->text(), first) ||
        !tryBuildPolynomial(ui->secondPolynomialEdit->text(), second)) {
        QMessageBox::warning(this, tr("Invalid input"), tr("Please enter both polynomials in the format 3^0+2^1+1^2."));
        return;
    }

    Polynomial result = MulOfPolynomial(first, second);
    showPolynomialResult(tr("Product"), result);
}

void MainWindow::on_derivativeButton_clicked()
{
    Polynomial first;

    if (!tryBuildPolynomial(ui->firstPolynomialEdit->text(), first)) {
        QMessageBox::warning(this, tr("Invalid input"), tr("Please enter a polynomial in the format 3^0+2^1+1^2."));
        return;
    }

    Polynomial result = DerOfPolynomial(first);
    showPolynomialResult(tr("Derivative"), result);
}

void MainWindow::on_evaluateButton_clicked()
{
    Polynomial first;
    if (!tryBuildPolynomial(ui->firstPolynomialEdit->text(), first)) {
        QMessageBox::warning(this, tr("Invalid input"), tr("Please enter a polynomial in the format 3^0+2^1+1^2."));
        return;
    }

    int xValue = ui->valueSpinBox->value();
    int evaluation = first.calculate_val(xValue);
    ui->resultTextEdit->append(tr("P(%1) = %2").arg(xValue).arg(evaluation));
}
