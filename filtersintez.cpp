#include "filtersintez.h"
#include <QTextCodec>

const double PI=3.141592654;

FilterSintez::FilterSintez(QWidget *parent)
    : QMainWindow(parent)
{
    Nfil = 4;
    Fc = 1000;

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    lblInputData = new QLabel(tr("Входные данные"));
    lblA1 = new QLabel(tr("Затухание фильтра в полосе пропускания, A1"));
    lblA2 = new QLabel(tr("Затухание фильтра в полосе задерживания, A2"));
    lblF1 = new QLabel(tr("Частота среза фильтра в полосе пропускания, F1"));
    lblF2 = new QLabel(tr("Частота фильтра в полосе задерживания, F2"));
    lblKv = new QLabel(tr("Усиление фильтра, Kv"));

    edtA1 = new QLineEdit("3");
    QDoubleValidator *val1 = new QDoubleValidator(0,100000,3);
    edtA1->setValidator(val1);
    edtA2 = new QLineEdit("20");
    edtA2->setValidator(val1);
    edtF1 = new QLineEdit("1000");
    edtF1->setValidator(val1);
    edtF2 = new QLineEdit("1200");
    edtF2->setValidator(val1);
    edtKv = new QLineEdit("2");
    edtKv->setValidator(val1);

    lblTyp = new QLabel(tr("Рассчитать фильтр:"));
    cbxFilterFunc = new QComboBox;
    QStringList lst2;
    lst2<<tr("Фильтр Баттерворта")
        <<tr("Фильтр Чебышева")
        <<tr("Инверсный фильтр Чебышева")
        <<tr("Эллиптический фильтр");
    cbxFilterFunc->addItems(lst2);
    btnCalcFiltFunc = new QPushButton(tr("Рассчитать функцию фильтра"));
    connect(btnCalcFiltFunc,SIGNAL(clicked()),this,SLOT(slotCalcFilter()));
    btnCalcSchematic = new QPushButton(tr("Рассчитать элементы схемы фильтра"));
    connect(btnCalcSchematic,SIGNAL(clicked()),SLOT(slotCalcSchematic()));

    lblResult = new QLabel(tr("Результаты расчёта: "));
    txtResult = new QTextEdit;


    lblSch = new QLabel(tr("Схемная реализация фильтра"));
    btnHighPass = new QRadioButton(tr("ФВЧ"));
    btnLowPass = new QRadioButton(tr("ФНЧ"));
    QButtonGroup *grp1 = new QButtonGroup;
    grp1->addButton(btnHighPass);
    grp1->addButton(btnLowPass);
    btnLowPass->setChecked(true);
    connect(grp1,SIGNAL(buttonClicked(int)),this,SLOT(slotUpdateSchematic()));

    cbxFilterType = new QComboBox;
    QStringList lst;
    lst<<tr("Биквадратный")
      <<tr("С многопетлевой ОС")
      <<tr("Саллена-Кея")
      <<tr("Пассивный");
    cbxFilterType->addItems(lst);
    connect(cbxFilterType,SIGNAL(currentIndexChanged(int)),this,SLOT(slotUpdateSchematic()));

    sch_pic = new QLabel;
    QPixmap pix("Images/dblquad.png");
    sch_pic->resize(pix.size());
    sch_pic->setPixmap(pix);

    top = new QHBoxLayout;
    left = new QVBoxLayout;
    center = new QVBoxLayout;
    right = new QVBoxLayout;

    left->addWidget(lblInputData);
    left->addWidget(lblA1);
    left->addWidget(edtA1);
    left->addWidget(lblA2);
    left->addWidget(edtA2);
    left->addWidget(lblF1);
    left->addWidget(edtF1);
    left->addWidget(lblF2);
    left->addWidget(edtF2);
    left->addWidget(lblKv);
    left->addWidget(edtKv);
    left->addWidget(lblTyp);
    left->addWidget(cbxFilterFunc);


    center->addWidget(lblResult);
    center->addWidget(txtResult);

    left->addWidget(lblSch);
    left->addWidget(btnLowPass);
    left->addWidget(btnHighPass);
    left->addWidget(cbxFilterType);
    left->addWidget(btnCalcFiltFunc);
    left->addWidget(btnCalcSchematic);

    right->addWidget(sch_pic);

    top->addLayout(left);
    top->addLayout(center);
    top->addLayout(right);

    zenter = new QWidget;
    this->setCentralWidget(zenter);
    zenter->setLayout(top);
}

FilterSintez::~FilterSintez()
{
    
}


void FilterSintez::slotCalcFilter()
{
    switch (cbxFilterFunc->currentIndex()) {
        case 0 : Nfil = calcButterworth();
                 break;
        case 1 : Nfil = calcChebyshev();
                 break;
        case 2 : Nfil = calcInvChebyshev();
                 break;
        case 3 : Nfil = calcElliptic();
                 break;
        default: break;
    }

    //calcButterworth();
}

void FilterSintez::slotCalcSchematic()
{
    switch (cbxFilterType->currentIndex()) {
    case 0 : calcDblQuadHPF();
             break;
    case 1 : calcMultiloopHPF();
             break;
    case 2 : if (btnHighPass->isChecked()) calcSallenKeyHPF();
             else calcSallenKeyLPF();
             break;
    case 3 : calcPassive();
             break;
    default: break;
    }
}

void FilterSintez::slotUpdateSchematic()
{
    QString s;
    switch (cbxFilterType->currentIndex()) {
    case 0 : s = "Images/dblquad.png";
             break;
    case 1 : s= "Images/multiloop.png";
             break;
    case 2 : if (btnHighPass->isChecked()) s = "Images/high-pass1.png";
             else s = "Images/low-pass1.png";
             break;
    case 3 : s = "Images/passive.png";
             break;
    default: break;
    }

    QPixmap pix(s);
    sch_pic->resize(pix.size());
    sch_pic->setPixmap(pix);
}

int FilterSintez::calcChebyshev()
{
    float A1 = edtA1->text().toFloat();
    float A2 = edtA2->text().toFloat();
    float F1 = edtF1->text().toFloat();
    float F2 = edtF2->text().toFloat();
    Fc = F1;

    float W = F2/F1;
    float K4=pow(10,(0.1*A1));
    float K5=pow(10,(0.1*A2));
    float C=pow(((K5-1)/(K4-1)),0.5);
    float J4=log(C+pow((C*C-1),0.5))/log(W+pow((W*W-1),0.5));
    int N4=round(J4+1);

    QStringList lst;

    lst<<tr(" 1. Порядок фильтра Чебышева");
    lst<<QString::number(N4);
    lst<<tr(" 2. Неравномерность пропускания");
    float E0=pow((K4-1),0.5);
    lst<<QString::number(E0);

    float E8=1/E0;
    float X=E8+pow((E8*E8+1),0.5);
    float V4=(1/(J4+1))*log(X);
    float X1=log(E8+pow((E8*E8+1),0.5));
    float V5=(1/(J4+1))*X1;
    float G1=0.5*(exp(V5)-exp(-V5));
    float G2=0.5*(exp(V4)+exp(-V4));

    float R0[50],S4[50],O4[50];
    lst<<""<<tr(" 2. Полюса")<<"Sk=SIN+j*COS";
    for (int R=1;R<N4;R++) {
            R0[R]=PI*(2*R-1)/(2*N4);
            S4[R]=-1*G1*sin(R0[R]);
            O4[R] = G2*cos(R0[R]);
            lst<<QString::number(S4[R]) + "+ j*" + QString::number(O4[R]);
    }

    txtResult->setText(lst.join("\n"));
    return N4;
}

int FilterSintez::calcButterworth()
{
    //float R1,l,w,q,Wc,C5,K7;

    float A1 = edtA1->text().toFloat();
    float A2 = edtA2->text().toFloat();
    float F1 = edtF1->text().toFloat();
    float F2 = edtF2->text().toFloat();
    Fc = F1;

    float W=F1/F2;
    float K1 = pow(10,(0.1*A1));
    float K2 = pow(10,(0.1*A2));
    float C1=(K1-1)/(K2-1);
    float J2=log10(C1)/(2*log10(W));
    int N2 = round(J2+1);

    QStringList lst;
    lst<<tr(" 1. Порядок фильтра Баттерворта");
    lst<<QString::number(N2);
    int N3=(N2+1)/2;
    lst<<tr(" Номер звена второго порядка");
    lst<<QString::number(N3);

    double R0[50],S2[50],O2[50];
    lst<<"";
    lst<<tr(" 2. Полюса");
    lst<<"Sk=SIN+j*COS";
    //lst<<"SIN";

    for (int R=1;R<2*N2;R++) {
        R0[R]=PI*(2*R-1)/(2*N2);
        S2[R]=-1*sin(R0[R]);
        O2[R]=cos(R0[R]);
        lst<<QString::number(S2[R]) + " + j*" + QString::number(O2[R]);
    }

    lst<<"";
    lst<<tr(" 3. Передаточная функция");

    float E[50],F[50],H[51];

    lst<<tr("H(S)=П 1/{S^2+E[К]*S+1}, к=1...n/2, где n-четно  ");
    lst<<tr("         к ");
    lst<<tr("H(S)= 1/(S+1)*П 1/{S^2+E[К]*S+1}, к=1...(n-1)/2, где n-нечетно  ");
    lst<<tr("                        к ");
    lst<<tr("E[K] (Число сомножителей Е[К]считается для 2n)");

    for (int K=1;K<2*N2;K++) {
       F[K]=PI*(2*K-1)/(2*N2);
       E[K]=2*sin(F[K]);

       lst<<QString::number(E[K]);

    }

    txtResult->setText(lst.join("\n"));

    return N2;
}

int FilterSintez::calcInvChebyshev()
{
    float A1 = edtA1->text().toFloat();
    float A2 = edtA2->text().toFloat();
    float F1 = edtF1->text().toFloat();
    float F2 = edtF2->text().toFloat();
    Fc = F1;

    float W5=F2/F1;
    float K4=pow(10,(0.1*A1));
    float C5=pow((K4-1),0.5);

    float J5 = log(C5+pow((C5*C5-1),0.5))/log(W5+pow((W5*W5-1),0.5));
    int N5 = round(J5+1);

    QStringList lst;

    lst<<tr(" 1. Порядок  инверсного фильтра Чебышева")<<QString::number(N5);

    float E0=pow((K4-1),0.5);
    lst<<tr("  2. Неравномерность пропускания")<<QString::number(E0);

    float E8=1/E0;
    float X=E8+pow((E8*E8+1),0.5);
    float V4=(1/(J5+1))*log(X);
    float X1=log(E8+pow((E8*E8+1),0.5));
    float V5=(1/(J5+1))*X1;
    float G1=0.5*(exp(V5)-exp(-V5));
    float G2=0.5*(exp(V4)+exp(-V4));

    float R0[50],S4[50],O4[50];

    lst<<tr(" 3. Полюса")<<"Sk=SIN+j*COS";

    for (int R=1;R<N5;R++) {
      R0[R]=PI*(2*R-1)/(2*N5);
      S4[R]=-1*G1*sin(R0[R]);
      O4[R]=G2*cos(R0[R]);
      lst<<QString::number(S4[R]) + " + j*" + QString::number(O4[R]);
    }

    txtResult->setText(lst.join("\n"));

    return N5;
}

int FilterSintez::calcElliptic()
{
    float A1 = edtA1->text().toFloat();
    float A2 = edtA2->text().toFloat();
    float F1 = edtF1->text().toFloat();
    float F2 = edtF2->text().toFloat();
    Fc = F1;

    float W5=F2/F1;
    float K9=F1/F2;
    float K4=pow(10,(0.1*A1));
    float K1=pow((1-K9*K9),0.5);

    float J0=A2+12.16-20*log10(pow((K4-1),0.5));
    float J9=6.08+20*log10((1+pow(K1,0.5))/(1-pow(K1,0.5)));
    float J7=J0/J9;
    int N7=round(J7+1);

    QStringList lst;
    lst<<tr(" 1. Порядок  эллиптического фильтра")<<QString::number(N7);
    float E0=pow((K4-1),0.5);
    lst<<tr(" 2. Неравномерность пропускания")<<QString::number(E0);

    float E8=1/E0;
    float X=E8+pow((E8*E8+1),0.5);
    float V4=(1/(J7+1))*log(X);
    float X1=log(E8+pow((E8*E8+1),0.5));
    float V5=(1/(J7+1))*X1;
    float G1=0.5*(exp(V5)-exp(-V5));
    float G2=0.5*(exp(V4)+exp(-V4));
    float R0[50],S4[50],O4[50];

    lst<<tr(" 3. Полюса")<<"Sk=SIN+j*COS";

    for (int R=1;R<N7;R++)
            {
            R0[R]=PI*(2*R-1)/(2*N7);
            S4[R]=-1*G1*sin(R0[R]);
            O4[R]=G2*cos(R0[R]);
            lst<<QString::number(S4[R]) + " + j*" + QString::number(O4[R]);
    }

    txtResult->setText(lst.join("\n"));

    return N7;
}

void FilterSintez::calcDblQuadHPF()
{

}

void FilterSintez::calcDblQuadLPF()
{

}

void FilterSintez::calcMultiloopHPF()
{

}

void FilterSintez::calcMultiloopLPF()
{

}

void FilterSintez::calcSallenKeyHPF()
{
    calcSallenKeyLPF();
}

void FilterSintez::calcSallenKeyLPF()
{
    float C1[20],C2[20],R1[20],R2[20],R3[20],R4[20];

    float Kv = edtKv->text().toFloat();

    QStringList lst;
    lst<<"N C1 C2 R1 R2 R3 R4";

    for (int K=1; K <= Nfil/2; K++) {
        float B = 2*sin((2*K-1)*M_PI/(2*Nfil));
        const float C = 1;

        C2[K] = 10 / Fc;

        float Wc = 2*M_PI*Fc;

        C1[K] = (B*B+4*C*(Kv-1))*C2[K]/(4*C);

        R1[K] = 2/(Wc*(B*C2[K]+sqrt((B*B + 4*C*(Kv-1))*(C2[K]*C2[K])-4*C*C1[K]*C2[K])));

        R2[K] = 1/(C*C1[K]*C2[K]*R1[K]*Wc*Wc);

        if (Kv != 1.0) {
            R3[K] = Kv*(R1[K] + R2[K])/(Kv - 1);
            R4[K] = Kv*(R1[K] + R2[K]);
        } else {
            R3[K] = 999;
            R4[K] = 0;
        }

        lst<<QString::number(K)+"  "+QString::number(C1[K])+"  "+QString::number(C2[K])+
             "  "+QString::number(R1[K])+"  "+QString::number(R2[K])+"  "+QString::number(R3[K])+
             "  "+QString::number(R4[K]);
    }

    txtResult->setText(lst.join("\n"));
}

void FilterSintez::calcPassive()
{

}
