#pragma once

#include <QMainWindow>
#include <QString>

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
    void on_addButton_clicked();
    void on_subtractButton_clicked();
    void on_multiplyButton_clicked();
    void on_derivativeButton_clicked();
    void on_evaluatePolynomialButton_clicked();
    void on_evaluateExpressionButton_clicked();

private:
    bool buildPolynomialFromInput(const QString &text, Polynomial &poly, QString &error) const;
    QString formatPolynomial(const Polynomial &poly) const;
    void showPolynomialError(const QString &message);

    Ui::MainWindow *ui;
};
