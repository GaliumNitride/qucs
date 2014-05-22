#include "transferfuncdialog.h"

TransferFuncDialog::TransferFuncDialog(QVector<long double> &a, QVector<long double> &b, QWidget *parent) :
    QDialog(parent)
{
    lblB = new QLabel(tr("Numenator b[i]="));
    lblA = new QLabel(tr("Denomenator a[i]="));

    QStringList indexes;
    for (int i=0;i<50;i++) {
        indexes<<QString::number(i);
    }

    tblA = new QTableWidget;
    QStringList head1;
    head1<<tr("a[i]");
    tblA->setColumnCount(head1.count());
    tblA->setRowCount(50);
    tblA->setHorizontalHeaderLabels(head1);
    tblA->setVerticalHeaderLabels(indexes);

    if (!a.isEmpty()) {
        long double num;
        int i = a.count()-1;

        foreach (num,a) {
            QTableWidgetItem *it = new QTableWidgetItem(QString::number((double)num));
            tblA->setItem(i,0,it);
            i--;
        }
    }


    tblB = new QTableWidget;
    QStringList head2;
    head2<<tr("b[i]");
    tblB->setColumnCount(head2.count());
    tblB->setRowCount(50);
    tblB->setHorizontalHeaderLabels(head2);
    tblB->setVerticalHeaderLabels(indexes);

    if (!b.isEmpty()) {
        long double num;
        int i = b.count()-1;

        foreach (num,b) {
            QTableWidgetItem *it = new QTableWidgetItem(QString::number((double)num));
            tblB->setItem(i,0,it);
            i--;
        }
    }

    btnAccept = new QPushButton(tr("Accept"));
    connect(btnAccept,SIGNAL(clicked()),this,SLOT(accept()));
    btnCancel = new QPushButton(tr("Cancel"));
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(reject()));

    low1 = new QVBoxLayout;
    low1->addWidget(lblB);
    low1->addWidget(tblB);

    low2 = new QVBoxLayout;
    low2->addWidget(lblA);
    low2->addWidget(tblA);

    low3 = new QHBoxLayout;
    low3->addWidget(btnAccept);
    low3->addWidget(btnCancel);

    top = new QHBoxLayout;
    top->addLayout(low1);
    top->addLayout(low2);

    top1 = new QVBoxLayout;
    top1->addLayout(top);
    top1->addLayout(low3);


    this->setLayout(top1);

}

void TransferFuncDialog::getCoeffs(QVector<long double> &a, QVector<long double> &b)
{
    a.clear();
    b.clear();

   // bool a0 = false;

    for (int i=tblA->rowCount()-1;i>=0;i--) {
        QTableWidgetItem *itm = tblA->item(i,0);
        if (itm!=0) {

            //if (!a0) a0 = true;

            QString str = itm->text();
            if ((str.isEmpty())) break;
            bool ok;
            long double n = (long double) str.toDouble(&ok);
            if (ok) a.append(n);
        }
        //else if (a0) a.append(0.0);
    }


    //bool b0 = true;

    for (int i=tblB->rowCount()-1;i>=0;i--) {
        QTableWidgetItem *itm = tblB->item(i,0);
        if (itm!=0) {

            //if (!b0) b0 = true;

            QString str = itm->text();
            if ((str.isEmpty())) break;
            bool ok;
            long double n = (long double) str.toDouble(&ok);
            if (ok) b.append(n);
        }
        //else if (b0) b.append(0.0);
    }
}
