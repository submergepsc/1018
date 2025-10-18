#pragma once

#include <QMainWindow>
#include <QString>

#include <optional>

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
    void on_evaluateButton_clicked();
    void on_expressionEvalButton_clicked();

private:
    bool validatePolynomialInputs(bool requireSecond = true);
    bool validatePolynomialFormat(const QString &text) const;
    void showError(const QString &message);
    QString getPolynomialText(int index) const;
    std::optional<Polynomial> buildPolynomialFromInput(int index);
    void displayResult(const QString &title, const QString &content);
    QString polynomialToDisplayString(const Polynomial &poly) const;

private:
    Ui::MainWindow *ui;
    AlgorithmExpression expressionCalculator_;
};
