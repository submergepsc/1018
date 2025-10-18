#pragma once

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Polynomial;

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

private:
    Ui::MainWindow *ui;

    bool tryBuildPolynomial(const QString &text, Polynomial &poly);
    QString polynomialToDisplayString(const Polynomial &poly) const;
    void showPolynomialResult(const QString &operation, const Polynomial &result);
};
