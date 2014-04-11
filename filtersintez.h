#ifndef FILTERSINTEZ_H
#define FILTERSINTEZ_H

#include <QtGui>
#include <QMainWindow>

class FilterSintez : public QMainWindow
{
    Q_OBJECT

private:
    QLabel *lblInputData;
    QLabel *lblA1;
    QLabel *lblA2;
    QLabel *lblF1;
    QLabel *lblF2;
    QLabel *lblTyp;

    QLineEdit *edtA1; // passband attenuation A1
    QLineEdit *edtA2; // stopband attenuation A2
    QLineEdit *edtF1; // passband cutoff frequency F1
    QLineEdit *edtF2; // stopband cutoff frequency F2

    QComboBox *cbxFilterFunc;
    QPushButton *btnCalcFiltFunc;

    QLabel *lblResult;
    QTextEdit *txtResult;

    QLabel *lblSch;

    QRadioButton *btnLowPass;
    QRadioButton *btnHighPass;

    QComboBox *cbxFilterType;

    QPushButton *btnElements;
    //QPushButton *btnPassive;

    QHBoxLayout *top;
    QVBoxLayout *left;
    QVBoxLayout *center;
    QVBoxLayout *right;
    
    QLabel *sch_pic;

    QWidget *zenter;

    void calcChebyshev();
    void calcButterworth();
    void calcInvChebyshev();
    void calcElliptic();


private slots:

    void slotCalcFilter();

public:
    FilterSintez(QWidget *parent = 0);
    ~FilterSintez();
};

#endif // FILTERSINTEZ_H
