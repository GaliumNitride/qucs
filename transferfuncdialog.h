#ifndef TRANSFERFUNCDIALOG_H
#define TRANSFERFUNCDIALOG_H

#include <QWidget>
#include <QtGui>

class TransferFuncDialog : public QDialog
{
    Q_OBJECT

private:
    QLabel *lblA;
    QLabel *lblB;
    QTableWidget *tblA;
    QTableWidget *tblB;

    QPushButton *btnAccept;
    QPushButton *btnCancel;

    QHBoxLayout *top;
    QVBoxLayout *top1;
    QVBoxLayout *low1;
    QVBoxLayout *low2;
    QHBoxLayout *low3;

public:
    explicit TransferFuncDialog(QWidget *parent = 0);

    void getCoeffs(QVector<float> &a, QVector<float> &b);
    
signals:
    
public slots:
    
};

#endif // TRANSFERFUNCDIALOG_H
