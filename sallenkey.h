#ifndef SALLENKEY_H
#define SALLENKEY_H

#include <QtCore>
#include <complex>
#include <math.h>
#include "filter.h"


class SallenKey : public Filter
{

private:

    void calcHighPass();
    void calcLowPass();
    void createHighPassSchematic(QString &s);
    void createLowPassSchematic(QString &s);

public:
    SallenKey(QVector< std::complex<float> > poles_, Filter::FType type_, float Fcutoff, float Kv_=1.0);

    void createSchematic(QString &s);

};

#endif // SALLENKEY_H
