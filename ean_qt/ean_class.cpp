#include "ean_class.h"

ean_class::ean_class(QObject *parent) : QObject(parent)
{

}


#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QVector>


using namespace std;
using namespace interval_arithmetic;
int counter = 0;

int ean_class::sgn(double a)
{
    return (a > 0) ? 1 : ((a < 0) ? -1 : 0);
}

bool ean_class::correction_test(QString n, bool to_int)
{
    if (to_int == 0)
    {
        auto parts = n.split(QLatin1Char('e'));
        string nump = parts[0].toUtf8().constData();
        if (nump == "")
        {
            return false;
        }
        int point = 0;
        int corr = 1;
        int start = 0;
        if(nump[0] == '-')
        {
            start++;
            if(nump.size() == 1)
            {
                return false;
            }
            else{
                if (nump[1] >= '0' && nump[1] <= '9')
                {
                    start++;
                }
                else{
                    return false;
                }
            }
        }
        else if(nump[0] == '.')
        {
            return false;
        }
        for(int i = start; i < nump.size(); i++)
        {
            if(nump[i] == '.')
            {
                point++;
            }
            else{
                corr = corr && (nump[i] >= '0' && nump[i] <= '9');
            }
        }
        if(point<2 && corr == 0)
        {
            return false;
        }
        if(parts.size() == 1)
        {return true;}
        nump = parts[1].toUtf8().constData();
        if(nump.size() == 1)
        {
            if (nump[0] >= '0' && nump[0] <= '9')
            {return true;}
            else{
                return false;
            }
        }
        if(nump[0] == '-' || (nump[0] >= '0' && nump[0] <= '9'))
        {
            for(int i = 1; i < nump.size(); i++)
            {
                if(nump[i] < '0' || nump [i] > '9')
                {
                    return false;
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else {
        string nump = n.toUtf8().constData();
        for(int i = 0; i < nump.size(); i++)
        {
            if(nump[i] < '0' || nump [i] > '9')
            {
                return false;
            }
        }
        return true;
    }

}

double ean_class::horner(vector <double> poli, double x)
{
    double res = poli[0];
    for(int i = 1; i < poli.size(); i++)
    {
        res*=x;
        res+=poli[i];
    }
    return res;

}

vector <double> ean_class::der()
{
    vector <double> res = {};
    int st = poli.size()-1;
    for(int i = 0; i <  poli.size()-1; i++)
    {
        res.push_back(poli[i]*st);
        st--;
    }
    return res;
}

QString ean_class::newton(double fg, double eps, int max_iter)
{
    st = 0;
    df = der();
    double res = fg;
    double f = horner(poli, res);
    double prim = horner(df, res);
    int tmp = 0;
    while (tmp < max_iter && fabs(f) >= eps)
    {
        while(fabs(prim) < eps)
        {
            res = fg-sgn(fg-res)*0.001;
            prim = horner(df, res);
        }
        res = res - f/prim;
        f = horner(poli, res);
        prim = horner(df, res);
        tmp++;
    }
    cout << "steps: "<< tmp << endl;
    st = tmp;
    ostringstream ostr;
    ostr << setprecision(15);
    ostr << scientific;
    ostr << res;
    QString qans = QString::fromStdString(ostr.str());

    return qans;

}

void ean_class::gen(int n)
{
    poli ={};
    for(int i = 0; i <= n; i++)
    {
        poli.push_back(0);
    }
}

void ean_class::add(int id, double val)
{
    poli[id] = val;
}

int ean_class::translate (QString n)
{
    return n.toInt();
}

void ean_class::set_len(int n)
{
    len = n;
}

QString ean_class::get_poli()
{
    QString res = "";
    for(int i = 0; i <= len; i++)
    {
        res.append(QString::number(poli[i]));
        res.append("\n");
    }
    return res;
}

QString ean_class::get_list()
{
    QString res = "";
    int tmp;
    for(int i = 0; i <= len; i++)
    {
        tmp = len - i;
        res.append("x^");
        res.append(QString::number(tmp));
        res.append("\n");
    }
    return res;
}

Interval <double> ean_class::horner_i (vector <Interval <double> > p, Interval <double> x)
{
    Interval <double> res = p[0];
    for(int i = 1; i < p.size(); i++)
    {
        res= x * res;
        res= p[i] + res;
    }
    return res;
}

vector < Interval <double> > ean_class::der_i()
{
    vector < Interval <double> > res = {};
    int st = poli_i.size()-1;
    for(int i = 0; i <  poli_i.size()-1; i++)
    {
        res.push_back(poli_i[i]*st);
        st--;
    }
    return res;
}

void ean_class::gen_intv(int n)
{
    poli_i ={};
    for(int i = 0; i <= n; i++)
    {
        Interval <double> tmp = {0,0};
        poli_i.push_back(tmp);
    }
}

QString ean_class::get_left()
{
    QString res = "";
    for(int i = 0; i <= len; i++)
    {
        res.append("[");
        res.append(QString::number(poli_i[i].a));
        res.append("\n");
    }
    return res;
}

QString ean_class::get_right()
{
    QString res = "";
    for(int i = 0; i <= len; i++)
    {
        res.append(QString::number(poli_i[i].b));
        res.append("]\n");
    }
    return res;
}

QString ean_class::newton_i(double fgl, double fgr, double eps, int max_iter, QVector <double> p)
{
    vector <Interval <double> > p_i;
    for(int i = 0; i < p.size(); i+=2)
    {
        p_i.push_back({p[i], p[i+1]});
    }
    vector <Interval <double> > dfi = {};
    int wyk = p_i.size()-1;
    for(int i = 0; i < p_i.size()-1; i++)
    {
        dfi.push_back(p_i[i]*wyk);
        wyk--;
    }
    Interval <double> fg = {fgl, fgr};
    Interval <double> res = fg;
    Interval <double> f;
    Interval <double> prim;
    int steps = 0;
    int sg = 0;
    Interval <double> comp;
    //schemat Hornera
    f = p_i[0];
    for(int i = 1; i < p_i.size(); i++)
    {
        f = res * f;
        f = p_i[i] + f;
    }
    prim = dfi[0];
    for(int i = 1; i < dfi.size(); i++)
    {
        prim = res * prim;
        prim = dfi[i] + prim;
    }

    QString ans;
    while (steps < max_iter && (f.a * f.b > 0 || fabs(f.a) > eps || fabs(f.b) > eps))
    {
        while(prim.a * prim.b <= 0 || fabs(prim.a) < eps || fabs(prim.b) < eps)
        {

            if(res.b < fg.a)
            {
                sg = 1;
            }
            else{
                sg = -1;
            }
            comp = {sg*0.01, sg*0.01};
            res = fg - comp;
            fg = res;
            f = p_i[0];
            for(int i = 1; i < p_i.size(); i++)
            {
                f = res * f;
                f = p_i[i] + f;
            }
            prim = dfi[0];
            for(int i = 1; i < dfi.size(); i++)
            {
                prim = res * prim;
                prim = dfi[i] + prim;
            }
        }
        comp = f/prim;
        if(comp.a*comp.b <= 0 && fabs(comp.a) < eps && fabs(comp.b) < eps)
        {
            break;
        }
        res = res - f/prim;
        f = p_i[0];
        for(int i = 1; i < p_i.size(); i++)
        {
            f = res * f;
            f = p_i[i] + f;
        }
        prim = dfi[0];
        for(int i = 1; i < dfi.size(); i++)
        {
            prim = res * prim;
            prim = dfi[i] + prim;
        }
        steps++;
    }
    cout << "steps: "<< steps << endl;
    ans = "[";
    string l, r;
    res.IEndsToStrings(l,r);
    ans.append(QString::fromStdString(l));
    ans.append(" ; ");
    ans.append(QString::fromStdString(r));
    ans.append("]   Steps: ");
    ans.append(QString::number(steps));
    return ans;
}

bool ean_class::correction_compare(double l, double r)
{
    return l<=r;
}

int ean_class::get_step()
{
    return st;
}

QVector <double> ean_class::convert_to_tab()
{
    QVector <double> res = {};
    for (int i = 0; i < poli_i.size(); i++)
    {
        res.append(poli_i[i].a);
        res.append(poli_i[i].b);
    }
    return res;
}

void ean_class::add_intv(QString l, QString r, int id)
{
    Interval <double> tmp;
    string val = l.toUtf8().constData();
    tmp.a = LeftRead<double>(val);
    val = r.toUtf8().constData();
    tmp.b = RightRead<double>(val);
    poli_i[id] = tmp;
}


