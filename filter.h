#ifndef FILTER_H
#define FILTER_H

#include <QtCore>
#include <math.h>
#include <complex>

struct RC_elements {
    int   N;
    float R1;
    float R2;
    float R3;
    float R4;
    float C1;
    float C2;
};

class Filter
{

public:
    enum FType {HighPass, LowPass, BandPass};

protected:
    QVector< std::complex<float> > Poles;
    QVector< std::complex<float> > Zeros;
    QVector<RC_elements> Stages;

    Filter::FType ftype;
    int Nfil;
    float Fc,Kv;
    int Nr,Nc,Nopamp; // total number of R,C, opamp

    int Nr1,Nc1,Nop1; // number of R,C, opamp per stage

    void createFirstOrderComponentsHPF(QString &s,RC_elements stage, int dx);
    void createFirstOrderComponentsLPF(QString &s,RC_elements stage, int dx);
    void createFirstOrderWires(QString &s, int dx);
    float autoscaleCapacitor(float C, QString &suffix);
    virtual void calcHighPass();
    virtual void calcLowPass();
    virtual void createHighPassSchematic(QString &s);
    virtual void createLowPassSchematic(QString &s);

public:


    Filter(QVector< std::complex<float> > poles_, Filter::FType type_, float Fcutoff, float Kv_=1.0);
    virtual ~Filter();

    void calcFirstOrder();

    void createPartList(QStringList &lst);

    virtual void createSchematic(QString &s);


    virtual void calcFilter();

};

#endif // FILTER_H
