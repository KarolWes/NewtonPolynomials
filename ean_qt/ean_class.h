#ifndef EAN_CLASS_H
#define EAN_CLASS_H

#include <vector>
#include <string>
#include <QString>
#include <QVector>
#include <QObject>
#include "Interval.h"

using namespace interval_arithmetic;

class ean_class : public QObject
{

    int len = 0;
    int st = 7;
    std::vector <double> poli = {0,0,0,0};
    std::vector <double> df;
    std::vector <Interval<double> > poli_i = {{0,0}};
    std::vector <Interval<double> > dfi;



    Q_OBJECT
public:
    explicit ean_class(QObject *parent = nullptr);

signals:


public slots:
    int sgn(double a);
    double horner(std::vector <double> poli, double x);
    std::vector <double> der();
    QString newton(double fg, double eps, int max_iter);
    void gen(int n);
    void add(int id, double val);
    int translate (QString n);
    void set_len(int n);
    QString get_poli();
    QString get_list();
    bool correction_test(QString n, bool to_int);
    Interval <double> horner_i (vector <Interval <double> > p, Interval <double> x);
    std::vector < Interval <double> > der_i();
    void add_intv(QString l, QString r, int id);
    void gen_intv(int n);
    QString get_left();
    QString get_right();
    int get_step();
    QVector <double>  convert_to_tab();
    QString newton_i(double fgl, double fgr, double eps, int max_iter, QVector <double> p);
    bool correction_compare(double l, double r);


};

#endif // EAN_CLASS_H



