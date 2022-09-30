/*
 * Interval.h
 *
 *  Created on: 27 lut 2021
 *      Author: tomhof
 */

#ifndef INTERVAL_H_
#define INTERVAL_H_

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <fenv.h>
#include <stdlib.h>
#include <stdint.h>
#include <cmath>
#include <mpfr.h>
#include <boost/lexical_cast.hpp>
#include <string.h>
#include <iomanip>
#include <fstream>
#include <float.h>
#include <typeinfo>
#include <mpreal.h>

using namespace std;
using namespace mpfr;

namespace interval_arithmetic {

enum IAPrecision {
	LONGDOUBLE_PREC = 63, DOUBLE_PREC = 53, FLOAT_PREC = 32, MPREAL_PREC = 40
};

enum IAOutDigits {
	LONGDOUBLE_DIGITS = 17, DOUBLE_DIGITS = 16, FLOAT_DIGITS = 7
};

enum IAMode {
	DINT_MODE, PINT_MODE
};

template<typename T> class Interval;

template<typename T> Interval<T> IntRead(const string &sa);
template<typename T> T LeftRead(const string &sa);
template<typename T> T RightRead(const string &sa);
template<typename T> T DIntWidth(const Interval<T> &x);
template<typename T> T IntWidth(const Interval<T> &x);
template<typename T> Interval<T> IAdd(const Interval<T> &x,
		const Interval<T> &y);
template<typename T> Interval<T> ISub(const Interval<T> &x,
		const Interval<T> &y);
template<typename T> Interval<T> IDiv(const Interval<T> &x,
		const Interval<T> &y);
template<typename T> Interval<T> IMul(const Interval<T> &x,
		const Interval<T> &y);
template<typename T> Interval<T> ISin(const Interval<T> &x);
template<typename T> Interval<T> ICos(const Interval<T> &x);
template<typename T> Interval<T> IExp(const Interval<T> &x);

template<typename T> Interval<T> DIAdd(const Interval<T> &x,
		const Interval<T> &y);
template<typename T> Interval<T> DISub(const Interval<T> &x,
		const Interval<T> &y);
template<typename T> Interval<T> DIDiv(const Interval<T> &x,
		const Interval<T> &y);
template<typename T> Interval<T> DIMul(const Interval<T> &x,
		const Interval<T> &y);
template<typename T> Interval<T> DISin(const Interval<T> &x);
template<typename T> Interval<T> DICos(const Interval<T> &x);
template<typename T> Interval<T> DIExp(const Interval<T> &x);
template<typename T> Interval<T> Hull(const Interval<T> &x,
		const Interval<T> &y);
template<typename T> Interval<T> IAbs(const Interval<T> &x);

template<typename T> int SetRounding(int rounding);
template<> Interval<mpreal> IntRead(const string &sa);

template<typename T> class Interval {
private:
	static IAPrecision precision;
	static IAOutDigits outdigits;

public:
	static IAMode mode;
	T a;
	T b;
	Interval();
	Interval(Interval const &copy);
	Interval(T a, T b);
	virtual ~Interval();
	Interval& operator=(const Interval<T> i);
	Interval operator+(const Interval<T> &i);
	Interval operator-(const Interval<T> &i);
	Interval operator*(const Interval<T> &i);
	Interval operator*(const long double &l);
	Interval operator*(const int &i);
	Interval operator/(const Interval<T> &i);
	Interval Projection();
	Interval Opposite();
	Interval Dual();
	Interval Inverse();
	T Mid();
	T GetWidth();
	static void Initialize();
	static Interval<T> ISqr2();
	static Interval<T> ISqr3();
	static Interval<T> IPi();
	static void SetMode(IAMode m) {
		mode = m;
	}
	static IAMode GetMode();
	static void SetPrecision(IAPrecision p);
	static IAPrecision GetPrecision();
	static void SetOutDigits(IAOutDigits o);
	static IAOutDigits GetOutDigits();
	static T GetEpsilon();

	void IEndsToStrings(string &left, string &right);

	friend T DIntWidth<T>(const Interval &x);
	friend T IntWidth<T>(const Interval &x);
	friend Interval IAdd<T>(const Interval &x, const Interval &y);
	friend Interval ISub<T>(const Interval &x, const Interval &y);
	friend Interval IDiv<T>(const Interval &x, const Interval &y);
	friend Interval IMul<T>(const Interval &x, const Interval &y);
	friend Interval ISin<T>(const Interval<T> &x);
	friend Interval ICos<T>(const Interval<T> &x);
	friend Interval IExp<T>(const Interval<T> &x);
	friend Interval IntRead<T>(const string &sa);
	friend T LeftRead<T>(const string &sa);
	friend T RightRead<T>(const string &sa);

	friend Interval DIAdd<T>(const Interval &x, const Interval &y);
	friend Interval DISub<T>(const Interval &x, const Interval &y);
	friend Interval DIDiv<T>(const Interval &x, const Interval &y);
	friend Interval DIMul<T>(const Interval &x, const Interval &y);
	friend Interval DISin<T>(const Interval &x);
	friend Interval DICos<T>(const Interval &x);
	friend Interval DIExp<T>(const Interval &x);

	friend Interval IAbs<T>(const Interval &x);
	friend Interval Hull<T>(const Interval &x, const Interval &y);

	friend int SetRounding<T>(int rounding);
};

template<typename T>
inline Interval<T>::~Interval() {
}

template<typename T>
Interval<T>::Interval() {
	this->a = 0;
	this->b = 0;
}

template<typename T>
Interval<T>::Interval(Interval const &copy) {
	this->a = copy.a;
	this->b = copy.b;
}

template<typename T>
inline Interval<T>::Interval(T a, T b) {
	this->a = a;
	this->b = b;
}

template<typename T>
inline IAMode Interval<T>::GetMode() {
	return Interval<T>::mode;
}

//template<>
//inline int SetRounding<mpreal>(int rounding) {
//	if (rounding == FE_UPWARD) {
//		mpreal::set_default_rnd(MPFR_RNDU);
//	} else if (rounding == FE_DOWNWARD) {
//		mpreal::set_default_rnd(MPFR_RNDD);
//	} else {
//		mpreal::set_default_rnd(MPFR_RNDN);
//	}
//	return rounding;
//}

template<typename T>
int SetRounding(int rounding) {
	fesetround(rounding);
	return rounding;
}

template<typename T>
inline Interval<T>& Interval<T>::operator =(Interval<T> i) {
	std::swap(this->a, i.a);
	std::swap(this->b, i.b);
	return *this;
}

template<typename T>
inline void Interval<T>::SetPrecision(IAPrecision p) {
	mpreal::set_default_prec(p);
	Interval<T>::precision = p;
}

template<typename T>
inline IAPrecision Interval<T>::GetPrecision() {
	return Interval<T>::precision;
}

template<typename T>
inline void Interval<T>::SetOutDigits(IAOutDigits o) {
	Interval<T>::outdigits = LONGDOUBLE_DIGITS;
}

template<typename T>
inline IAOutDigits Interval<T>::GetOutDigits() {
	return Interval<T>::outdigits;
}

template<typename T>
inline T Interval<T>::GetEpsilon() {
	return std::numeric_limits<T>::epsilon();
}

template<typename T>
inline Interval<T> IntRead(const string &sa) {
	Interval<T> r;
	mpfr_t rop;
	mpfr_init2(rop, Interval<T>::precision);
	mpfr_set_str(rop, sa.c_str(), 10, MPFR_RNDD);
	T le = 0.0;
	if (strcmp(typeid(T).name(), typeid(long double).name()) == 0) {
		le = mpfr_get_ld(rop, MPFR_RNDD);
	}
	if (strcmp(typeid(T).name(), typeid(double).name()) == 0) {
		le = mpfr_get_d(rop, MPFR_RNDD);
	}
	if (strcmp(typeid(T).name(), typeid(float).name()) == 0) {
		le = mpfr_get_flt(rop, MPFR_RNDD);
	}

	mpfr_set_str(rop, sa.c_str(), 10, MPFR_RNDU);
	T re = 0.0;
	if (strcmp(typeid(T).name(), typeid(long double).name()) == 0) {
		re = mpfr_get_ld(rop, MPFR_RNDU);
	}
	if (strcmp(typeid(T).name(), typeid(double).name()) == 0) {
		re = mpfr_get_d(rop, MPFR_RNDU);
	}
	if (strcmp(typeid(T).name(), typeid(float).name()) == 0) {
		re = mpfr_get_flt(rop, MPFR_RNDU);
	}

	SetRounding<T>(FE_TONEAREST);

	r.a = le;
	r.b = re;
	return r;
}

template<>
inline Interval<mpreal> IntRead(const string &sa) {
	Interval<mpreal> r;
	mpfr_t rop;
	mpfr_init2(rop, Interval<mpreal>::precision);
	mpfr_set_str(rop, sa.c_str(), 10, MPFR_RNDD);
	mpreal le = rop;

	mpfr_set_str(rop, sa.c_str(), 10, MPFR_RNDU);
	mpreal re = rop;

	r.a = le;
	r.b = re;
	return r;
}

template<typename T>
inline void Interval<T>::IEndsToStrings(string &left, string &right) {
	mpfr_t rop;
	mpfr_exp_t exponent;
	mpfr_init2(rop, precision);
	char *str = NULL;
	char *buffer = new char(precision + 3);
	mpfr_set_ld(rop, this->a, MPFR_RNDD);

	mpfr_get_str(buffer, &exponent, 10, outdigits, rop, MPFR_RNDD);
	str = buffer;

	stringstream ss;
	int prec = std::numeric_limits<T>::digits10;
	ss.setf(std::ios_base::scientific);
	bool minus = (str[0] == '-');
	int splitpoint = minus ? 1 : 0;
	string sign = minus ? "-" : "";

	ss << std::setprecision(prec) << sign << str[splitpoint] << "."
			<< &str[splitpoint + 1] << "E" << exponent - 1;
	left = ss.str();
	ss.str(std::string());

	mpfr_set_ld(rop, this->b, MPFR_RNDU);
	mpfr_get_str(buffer, &exponent, 10, outdigits, rop, MPFR_RNDU);
	str = buffer;
	splitpoint = (str[0] == '-') ? 1 : 0;
	ss << std::setprecision(prec) << sign << str[splitpoint] << "."
			<< &str[splitpoint + 1] << "E" << exponent - 1;
	right = ss.str();
	ss.clear();
}

template<typename T>
inline Interval<T> Interval<T>::Projection() {
	Interval<T> x(this->a, this->b);
	Interval<T> r;
	r = x;
	if (x.a > x.b) {
		r.a = x.b;
		r.b = x.a;
	}
	return r;
}

template<typename T>
inline Interval<T> Interval<T>::Opposite() {
	Interval<T> x(this->a, this->b);
	Interval<T> r;
	r.a = -x.a;
	r.b = -x.b;
	return r;
}

template<typename T>
inline Interval<T> Interval<T>::Dual() {
	Interval<T> x(this->a, this->b);
	Interval<T> r;
	r.a = x.b;
	r.b = x.a;
	return r;
}

template<typename T>
inline Interval<T> Interval<T>::Inverse() {
	Interval<T> x(this->a, this->b);
	Interval<T> z1, z2;

	SetRounding<T>(FE_DOWNWARD);
	z1.a = 1 / x.a;
	z2.b = 1 / x.b;
	SetRounding<T>(FE_UPWARD);
	z1.b = 1 / x.b;
	z2.a = 1 / x.a;
	SetRounding<T>(FE_TONEAREST);
	if (DIntWidth(z1) >= DIntWidth(z2))
		return z1;
	else
		return z2;
}

template<typename T>
inline T LeftRead(const string &sa) {
	Interval<T> int_number;
	int_number = IntRead<T>(sa);
	return int_number.a;
}

template<typename T>
inline T Interval<T>::GetWidth() {
	Interval<T> x(this->a, this->b);
	switch (mode) {
	case PINT_MODE:
		return IntWidth(x);
	case DINT_MODE:
		return DIntWidth(x);
	default:
		return IntWidth(x);
	}
}

template<typename T>
inline T Interval<T>::Mid() {
	return (this->b + this->a) / 2.0;
}

template<typename T>
inline Interval<T> Interval<T>::ISqr2() {
	string i2;
	Interval<T> r;
	i2 = "1.414213562373095048";
	r.a = LeftRead<T>(i2);
	i2 = "1.414213562373095049";
	r.b = RightRead<T>(i2);
	return r;
}

template<typename T>
inline Interval<T> Interval<T>::ISqr3() {
	string i2;
	Interval<T> r;
	i2 = "1.732050807568877293";
	r.a = LeftRead<T>(i2);
	i2 = "1.732050807568877294";
	r.b = RightRead<T>(i2);
	return r;
}

template<typename T>
inline Interval<T> Interval<T>::IPi() {
	string i2;
	Interval<T> r;
	i2 = "3.141592653589793238";
	r.a = LeftRead<T>(i2);
	i2 = "3.141592653589793239";
	r.b = RightRead<T>(i2);
	return r;
}

template<typename T>
inline void Interval<T>::Initialize() {
	if (strcmp(typeid(T).name(), typeid(long double).name()) == 0) {
		Interval<T>::SetPrecision(LONGDOUBLE_PREC);
		Interval<T>::SetOutDigits(LONGDOUBLE_DIGITS);
	}

	if (strcmp(typeid(T).name(), typeid(double).name()) == 0) {
		Interval<T>::SetPrecision(DOUBLE_PREC);
		Interval<T>::SetOutDigits(DOUBLE_DIGITS);
	}

	if (strcmp(typeid(T).name(), typeid(float).name()) == 0) {
		Interval<T>::SetPrecision(FLOAT_PREC);
		Interval<T>::SetOutDigits(FLOAT_DIGITS);
	}

	if (strcmp(typeid(T).name(), typeid(mpreal).name()) == 0) {
		Interval<T>::SetPrecision(MPREAL_PREC);
		Interval<T>::SetOutDigits(FLOAT_DIGITS);
	}

}

template<typename T>
inline T RightRead(const string &sa) {
	Interval<T> int_number;
	int_number = IntRead<T>(sa);
	return int_number.b;
}

template<typename T>
T IntWidth(const Interval<T> &x) {
	SetRounding<T>(FE_UPWARD);
	T w = x.b - x.a;
	SetRounding<T>(FE_TONEAREST);
	return w;
}

template<typename T>
T DIntWidth(const Interval<T> &x) {
	long double w1, w2;

	SetRounding<T>(FE_UPWARD);
	w1 = x.b - x.a;
	if (w1 < 0)
		w1 = -w1;
	SetRounding<T>(FE_DOWNWARD);
	w2 = x.b - x.a;
	if (w2 < 0)
		w2 = -w2;
	SetRounding<T>(FE_TONEAREST);
	if (w1 > w2)
		return w1;
	else
		return w2;
}

template<typename T>
Interval<T> ISin(const Interval<T> &x) {
	bool is_even, finished;
	int k;
	int st = 0;
	Interval<T> d, s, w, w1, x2, tmp;
	Interval<T> izero(0, 0);
	string left, right;
	T eps = 1E-18; //Interval<T>::GetEpsilon();
	T diff = std::numeric_limits<T>::max();
	if (x.a > x.b)
		st = 1;
	else {
		s = x;
		w = x;
		x2 = IMul(x, x);
		k = 1;
		is_even = true;
		finished = false;
		st = 0;

		do {
			d.a = (k + 1) * (k + 2);
			d.b = d.a;
			s = IMul(s, IDiv(x2, d));
			if (is_even)
				w1 = ISub(w, s);
			else
				w1 = IAdd(w, s);

//			T oldMid = (w.a + w.b) / 2;
//			T newMid = (w1.a + w1.b) / 2;
//			T currDiff = abs(oldMid - newMid);
//			finished = (currDiff > diff);
//			diff = currDiff;

			if ((w.a == 0) && (w.b == 0)) {
				return izero;
			}

			if ((w.a != 0) && (w.b != 0)) {
				if ((abs(w.a - w1.a) / abs(w.a) < eps)
						&& (abs(w.b - w1.b) / abs(w.b) < eps))
					finished = true;
				else
					;
			} else if ((w.a == 0) && (w.b != 0)) {
				if ((abs(w.a - w1.a) < eps)
						&& (abs(w.b - w1.b) / abs(w.b) < eps))
					finished = true;
				else
					;
			} else if (w.a != 0) {
				if ((abs(w.a - w1.a) / abs(w.a) < eps)
						& (abs(w.b - w1.b) < eps))
					finished = true;
				else if ((abs(w.a - w1.a) < eps) & (abs(w.b - w1.b) < eps))
					finished = true;
			}

			if (finished) {
				if (w1.b > 1) {
					w1.b = 1;
					if (w1.a > 1)
						w1.a = 1;
				}
				if (w1.a < -1) {
					w1.a = -1;
					if (w1.b < -1)
						w1.b = -1;
				}
				return w1;
			} else {
				w = w1;
				k = k + 2;
				is_even = !is_even;
			}
		} while (!(finished || (k > INT_MAX / 2)));
	}
	if (!finished)
		st = 2;

	Interval<T> r;
	r.a = 0;
	r.b = 0;
	return r;
}

template<typename T>
Interval<T> ICos(const Interval<T> &x) {
	Interval<T> c, d, w, w1, x2;
	int k, st;
	bool is_even, finished;

	c.a = 1;
	c.b = 1;
	w = c;
	x2 = IMul(x, x);
	k = 1;
	is_even = true;
	finished = false;
	st = 0;

	do {
		d.a = k * (k + 1);
		d.b = d.a;
		c = IMul(c, IDiv(x2, d));
		if (is_even) {
			w1 = ISub(w, c);
		} else {
			w1 = IAdd(w, c);
		}
		if ((w.a != 0) && (w.b != 0)) {
			if (((abs(w.a - w1.a) / abs(w.a)) < 1e-18)
					&& (abs(w.b - w1.b) / abs(w.b) < 1e-18))
				finished = true;
		} else {
			if ((w.a == 0) && (w.b != 0)) {
				if ((abs(w.a - w1.a) < 1e-18)
						&& (abs(w.b - w1.b) / abs(w.b) < 1e-18)) {
					finished = true;
				}
			} else if (w.a != 0) {
				if ((abs(w.a - w1.a) / abs(w.a) < 1e-18)
						&& (abs(w.b - w1.b) < 1e-18))
					finished = true;
			} else if ((abs(w.a - w1.a) < 1e-18) && (abs(w.b - w1.b) < 1e-18))
				finished = true;

		}
		if (finished) {
			if (w1.b > 1) {
				w1.b = 1;
				if (w1.a > 1)
					w1.a = 1;
			}
			if (w1.a < -1) {
				w1.a = -1;
				if (w1.b < -1)
					w1.b = -1;
			}
			return w1;
		} else {
			w = w1;
			k = k + 2;
			is_even = !is_even;
		}

	} while (!finished || (k > INT_MAX / 2));

	if (!finished)
		st = 2;

	Interval<T> r;
	r.a = 0;
	r.b = 0;
	return r;
}

template<typename T>
Interval<T> IExp(const Interval<T> &x) {
	bool finished;
	int k;
	int st = 0;
	Interval<T> d, e, w, w1;
	string left, right;
	Interval<T> tmp = x;
	T eps = 1E-18; //2*Interval<T>::GetEpsilon();
	T diff = std::numeric_limits<T>::max();
	if ((x.a < 0) && (x.b > 0))
		return {1,1};
	if (x.a > x.b)
		st = 1;
	else {
		e.a = 1;
		e.b = 1;
		w = e;
		k = 1;
		finished = false;
		st = 0;
		do {
			d.a = k;
			d.b = k;
			e = IMul(e, IDiv(x, d));
			w1 = IAdd(w, e);
			T oldMid = (w.a + w.b) / 2;
			T newMid = (w1.a + w1.b) / 2;
			T currDiff = abs(oldMid - newMid);
			//finished = (currDiff >= diff);
			T tmpDiff = diff - currDiff;
			diff = currDiff;
			if ((abs(w.a - w1.a) / abs(w.a) < eps)
					&& (abs(w.b - w1.b) / abs(w.b) < eps)) {
				finished = true;
				return w1;
			} else {
				w = w1;
				k = k + 1;
				if (k > 100000) {
					T wdth = w.GetWidth();

					tmp.IEndsToStrings(left, right);
					cout << "x=[" << left << "," << right << "]" << endl;
					w.IEndsToStrings(left, right);
					cout << "[" << left << "," << right << "]" << endl;
					cout << "      width =  " << std::setprecision(17) << wdth
							<< endl << " diff = " << diff << endl
							<< " tmpDiff = " << tmpDiff << endl << "eps = "
							<< eps << endl;
				}
			}
		} while (!(finished || (k > INT_MAX / 2)));
		if (!finished)
			st = 2;
	}
	Interval<T> r;
	r.a = 0;
	r.b = 0;
	return r;
}

template<typename T>
Interval<T> ISqr(const Interval<T> &x, int &st) {
	long double minx, maxx;
	Interval<T> r;
	r.a = 0;
	r.b = 0;
	if (x.a > x.b)
		st = 1;
	else {
		st = 0;
		if ((x.a <= 0) && (x.b >= 0))
			minx = 0;
		else if (x.a > 0)
			minx = x.a;
		else
			minx = x.b;
		if (abs(x.a) > abs(x.b))
			maxx = abs(x.a);
		else
			maxx = abs(x.b);
		SetRounding<T>(FE_DOWNWARD);
		r.a = minx * minx;
		SetRounding<T>(FE_UPWARD);
		r.b = maxx * maxx;
		SetRounding<T>(FE_TONEAREST);
	}
	return r;
}

template<typename T>
Interval<T> ISqrt(const Interval<T> &x, int &st) {
	Interval<T> r;
	r.a = 0;
	r.b = 0;
	if (x.a > x.b) {
		st = 1;
	} else if (x.a < 0) {
		st = 2;
	} else {
		st = 0;
		SetRounding<T>(FE_DOWNWARD);
		r.a = std::sqrt(x.a);
		SetRounding<T>(FE_UPWARD);
		r.b = std::sqrt(x.b);
		SetRounding<T>(FE_TONEAREST);
	}

	return r;
}

template<typename T>
Interval<T> IAdd(const Interval<T> &x, const Interval<T> &y) {
	Interval<T> r;
	SetRounding<T>(FE_DOWNWARD);
	r.a = x.a + y.a;
	SetRounding<T>(FE_UPWARD);
	r.b = x.b + y.b;
	SetRounding<T>(FE_TONEAREST);
	return r;
}

template<typename T>
Interval<T> ISub(const Interval<T> &x, const Interval<T> &y) {
	Interval<T> r;
	SetRounding<T>(FE_DOWNWARD);
	r.a = x.a - y.b;
	SetRounding<T>(FE_UPWARD);
	r.b = x.b - y.a;
	SetRounding<T>(FE_TONEAREST);
	return r;
}

template<typename T>
Interval<T> IMul(const Interval<T> &x, const Interval<T> &y) {
	Interval<T> r(0, 0);
	T x1y1, x1y2, x2y1;

	SetRounding<T>(FE_DOWNWARD);
	x1y1 = x.a * y.a;
	x1y2 = x.a * y.b;
	x2y1 = x.b * y.a;
	r.a = x.b * y.b;
	if (x2y1 < r.a)
		r.a = x2y1;
	if (x1y2 < r.a)
		r.a = x1y2;
	if (x1y1 < r.a)
		r.a = x1y1;

	SetRounding<T>(FE_UPWARD);
	x1y1 = x.a * y.a;
	x1y2 = x.a * y.b;
	x2y1 = x.b * y.a;

	r.b = x.b * y.b;
	if (x2y1 > r.b)
		r.b = x2y1;
	if (x1y2 > r.b)
		r.b = x1y2;
	if (x1y1 > r.b)
		r.b = x1y1;
	SetRounding<T>(FE_TONEAREST);
	return r;
}

template<typename T>
Interval<T> IDiv(const Interval<T> &x, const Interval<T> &y) {
	Interval<T> r;
	T x1y1, x1y2, x2y1, t;

	if ((y.a <= 0) && (y.b >= 0)) {
		throw runtime_error("Division by an interval containing 0.");
	} else {
		SetRounding<T>(FE_DOWNWARD);
		x1y1 = x.a / y.a;
		x1y2 = x.a / y.b;
		x2y1 = x.b / y.a;
		r.a = x.b / y.b;
		t = r.a;
		if (x2y1 < t)
			r.a = x2y1;
		if (x1y2 < t)
			r.a = x1y2;
		if (x1y1 < t)
			r.a = x1y1;

		SetRounding<T>(FE_UPWARD);
		x1y1 = x.a / y.a;
		x1y2 = x.a / y.b;
		x2y1 = x.b / y.a;

		r.b = x.b / y.b;
		t = r.b;
		if (x2y1 > t)
			r.b = x2y1;
		if (x1y2 > t)
			r.b = x1y2;
		if (x1y1 > t)
			r.b = x1y1;

	}
	SetRounding<T>(FE_TONEAREST);
	return r;
}

template<typename T>
Interval<T> DIAdd(const Interval<T> &x, const Interval<T> &y) {
	Interval<T> z1, z2;
	if ((x.a <= x.b) && (y.a <= y.b)) {
		return IAdd<T>(x, y);
	} else {
		SetRounding<T>(FE_DOWNWARD);
		z1.a = x.a + y.a;
		z2.b = x.b + y.b;
		SetRounding<T>(FE_UPWARD);
		z1.b = x.b + y.b;
		z2.a = x.a + y.a;
		SetRounding<T>(FE_TONEAREST);
		if (z1.GetWidth() >= z2.GetWidth())
			return z1;
		else
			return z2;
	}
}

template<typename T>
Interval<T> DISub(const Interval<T> &x, const Interval<T> &y) {
	Interval<T> z1, z2;
	if ((x.a <= x.b) && (y.a <= y.b)) {
		return ISub(x, y);
	} else {
		SetRounding<T>(FE_DOWNWARD);
		z1.a = x.a - y.b;
		z2.b = x.b - y.a;
		SetRounding<T>(FE_UPWARD);
		z1.b = x.b - y.a;
		z2.a = x.a - y.b;
		SetRounding<T>(FE_TONEAREST);
		if (z1.GetWidth() >= z2.GetWidth())
			return z1;
		else
			return z2;
	}
}

template<typename T>
Interval<T> DIMul(const Interval<T> &x, const Interval<T> &y) {
	Interval<T> z1, z2, r;
	T z;
	bool xn, xp, yn, yp, zero;

	if ((x.a <= x.b) && (y.a <= y.b))
		r = IMul(x, y);
	else {
		xn = (x.a < 0) and (x.b < 0);
		xp = (x.a > 0) and (x.b > 0);
		yn = (y.a < 0) and (y.b < 0);
		yp = (y.a > 0) and (y.b > 0);
		zero = false;
		// A, B in H-T
		if ((xn || xp) && (yn || yp))
			if (xp && yp) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.a * y.a;
				z2.b = x.b * y.b;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.b * y.b;
				z2.a = x.a * y.a;
			} else if (xp && yn) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.b * y.a;
				z2.b = x.a * y.b;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.a * y.b;
				z2.a = x.b * y.a;
			} else if (xn && yp) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.a * y.b;
				z2.b = x.b * y.a;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.b * y.a;
				z2.a = x.a * y.b;
			} else {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.b * y.b;
				z2.b = x.a * y.a;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.a * y.a;
				z2.a = x.b * y.b;
			}
		// A in H-T, B in T
		else if ((xn || xp)
				&& (((y.a <= 0) && (y.b >= 0)) || ((y.a >= 0) && (y.b <= 0))))
			if (xp && (y.a <= y.b)) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.b * y.a;
				z2.b = x.b * y.b;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.b * y.b;
				z2.a = x.b * y.a;
			} else if (xp && (y.a > y.b)) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.a * y.a;
				z2.b = x.a * y.b;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.a * y.b;
				z2.a = x.a * y.a;
			} else if (xn && (y.a <= y.b)) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.a * y.b;
				z2.b = x.a * y.a;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.a * y.a;
				z2.a = x.a * y.b;
			} else {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.b * y.b;
				z2.b = x.b * y.a;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.b * y.a;
				z2.a = x.b * y.b;
			}
		// A in T, B in H-T
		else if ((((x.a <= 0) && (x.b >= 0)) || ((x.a >= 0) && (x.b <= 0)))
				&& (yn || yp))
			if ((x.a <= x.b) && yp) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.a * y.b;
				z2.b = x.b * y.b;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.b * y.b;
				z2.a = x.a * y.b;
			} else if ((x.a <= 0) && yn) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.b * y.a;
				z2.b = x.a * y.a;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.a * y.a;
				z2.a = x.b * y.a;
			} else if ((x.a > x.b) && yp) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.a * y.a;
				z2.b = x.b * y.a;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.b * y.a;
				z2.a = x.a * y.a;
			} else {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.b * y.b;
				z2.b = x.a * y.b;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.a * y.b;
				z2.a = x.b * y.b;
			}
		// A, B in Z-
		else if ((x.a >= 0) && (x.b <= 0) && (y.a >= 0) && (y.b <= 0)) {
			SetRounding<T>(FE_DOWNWARD);
			z1.a = x.a * y.a;
			z = x.b * y.b;
			if (z1.a < z)
				z1.a = z;
			z2.b = x.a * y.b;
			z = x.b * y.a;
			if (z < z2.b)
				z2.b = z;
			SetRounding<T>(FE_UPWARD);
			z1.b = x.a * y.b;
			z = x.b * y.a;
			if (z < z1.b)
				z1.b = z;
			z2.a = x.a * y.a;
			z = x.b * y.b;
			if (z2.a < z)
				z2.a = z;
		}
		// A in Z and B in Z- or A in Z- and B in Z
		else
			zero = true;
		if (zero) {
			r.a = 0;
			r.b = 0;
		} else if (z1.GetWidth() >= z2.GetWidth())
			r = z1;
		else
			r = z2;
	}

	SetRounding<T>(FE_TONEAREST);
	return r;
}

template<typename T>
Interval<T> DIDiv(const Interval<T> &x, const Interval<T> &y) {
	Interval<T> z1, z2, r;
	bool xn, xp, yn, yp, zero;

	if ((x.a <= x.b) && (y.a <= y.b))
		r = IDiv(x, y);
	else {
		xn = (x.a < 0) && (x.b < 0);
		xp = (x.a > 0) && (x.b > 0);
		yn = (y.a < 0) && (y.b < 0);
		yp = (y.a > 0) && (y.b > 0);
		zero = false;
		// A, B in H-T
		if ((xn || xp) && (yn || yp))
			if (xp && yp) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.a / y.b;
				z2.b = x.b / y.a;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.b / y.a;
				z2.a = x.a / y.b;
			} else if (xp && yn) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.b / y.b;
				z2.b = x.a / y.a;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.a / y.a;
				z2.a = x.b / y.b;
			} else if (xn && yp) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.a / y.a;
				z2.b = x.b / y.b;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.b / y.b;
				z2.a = x.a / y.a;
			} else {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.b / y.a;
				z2.b = x.a / y.b;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.a / y.b;
				z2.a = x.b / y.a;
			}
		// A in T, B in H-T
		else if (((x.a <= 0) && (x.b >= 0))
				|| (((x.a >= 0) && (x.b <= 0)) && (yn || yp)))
			if ((x.a <= x.b) && yp) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.a / y.a;
				z2.b = x.b / y.a;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.b / y.a;
				z2.a = x.a / y.a;
			} else if ((x.a <= x.b) && yn) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.b / y.b;
				z2.b = x.a / y.b;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.a / y.b;
				z2.a = x.b / y.b;
			} else if ((x.a > x.b) && yp) {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.a / y.b;
				z2.b = x.b / y.b;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.b / y.b;
				z2.a = x.a / y.b;
			} else {
				SetRounding<T>(FE_DOWNWARD);
				z1.a = x.b / y.a;
				z2.b = x.a / y.a;
				SetRounding<T>(FE_UPWARD);
				z1.b = x.a / y.a;
				z2.a = x.b / y.a;
			}
		else
			zero = true;
		if (zero)
			throw runtime_error("Division by an interval containing 0.");
		else if (z1.GetWidth() >= z2.GetWidth())
			r = z1;
		else
			r = z2;
		SetRounding<T>(FE_TONEAREST);
	}
	return r;
}

template<typename T>
Interval<T> DISin(const Interval<T> &x) {
	bool is_even, finished;
	int k;
	int st = 0;
	Interval<T> d, s, w, w1, x2;
	if (x.a > x.b)
		st = 1;
	else {
		s = x;
		w = x;
		x2 = DIMul(x, x);
		k = 1;
		is_even = true;
		finished = false;
		st = 0;

		do {
			d.a = (k + 1) * (k + 2);
			d.b = d.a;
			s = DIMul(s, DIDiv(x2, d));
			if (is_even)
				w1 = DISub(w, s);
			else
				w1 = DIAdd(w, s);
			if ((w.a != 0) && (w.b != 0)) {
				if ((abs(w.a - w1.a) / abs(w.a) < 1e-18)
						&& (abs(w.b - w1.b) / abs(w.b) < 1e-18))
					finished = true;
				else
					;
			} else if ((w.a == 0) && (w.b != 0)) {
				if ((abs(w.a - w1.a) < 1e-18)
						&& (abs(w.b - w1.b) / abs(w.b) < 1e-18))
					finished = true;
				else
					;
			}

			else if (w.a != 0) {
				if ((abs(w.a - w1.a) / abs(w.a) < 1e-18)
						& (abs(w.b - w1.b) < 1e-18))
					finished = true;
				else if ((abs(w.a - w1.a) < 1e-18) & (abs(w.b - w1.b) < 1e-18))
					finished = true;
			}

			if (finished) {
				if (w1.b > 1) {
					w1.b = 1;
					if (w1.a > 1)
						w1.a = 1;
				}
				if (w1.a < -1) {
					w1.a = -1;
					if (w1.b < -1)
						w1.b = -1;
				}
				return w1;
			} else {
				w = w1;
				k = k + 2;
				is_even = !is_even;
			}
		} while (!(finished || (k > INT_MAX / 2)));
	}
	if (!finished)
		st = 2;

	Interval<T> r;
	r.a = 0;
	r.b = 0;
	return r;
}

template<typename T>
Interval<T> DICos(const Interval<T> &x) {
	bool is_even, finished;
	int k, st;
	Interval<T> d, c, w, w1, x2;
	if (x.a > x.b)
		st = 1;
	else {
		c.a = 1;
		c.b = 1;
		w = c;
		x2 = DIMul(x, x);
		k = 1;
		is_even = true;
		finished = false;
		st = 0;

		do {
			d.a = k * (k + 1);
			d.b = d.a;
			c = DIMul(c, DIDiv(x2, d));
			if (is_even)
				w1 = DISub(w, c);
			else
				w1 = DIAdd(w, c);

			if ((w.a != 0) && (w.b != 0)) {
				if ((abs(w.a - w1.a) / abs(w.a) < 1e-18)
						&& (abs(w.b - w1.b) / abs(w.b) < 1e-18))
					finished = true;
				else
					;
			} else if ((w.a == 0) && (w.b != 0)) {
				if ((abs(w.a - w1.a) < 1e-18)
						&& (abs(w.b - w1.b) / abs(w.b) < 1e-18))
					finished = true;
				else
					;
			}

			else if (w.a != 0) {
				if ((abs(w.a - w1.a) / abs(w.a) < 1e-18)
						& (abs(w.b - w1.b) < 1e-18))
					finished = true;
				else if ((abs(w.a - w1.a) < 1e-18) & (abs(w.b - w1.b) < 1e-18))
					finished = true;
			}

			if (finished) {
				if (w1.b > 1) {
					w1.b = 1;
					if (w1.a > 1)
						w1.a = 1;
				}
				if (w1.a < -1) {
					w1.a = -1;
					if (w1.b < -1)
						w1.b = -1;
				}
				return w1;
			} else {
				w = w1;
				k = k + 2;
				is_even = !is_even;
			}
		} while (!(finished || (k > INT_MAX / 2)));
	}
	if (!finished)
		st = 2;

	Interval<T> r;
	r.a = 0;
	r.b = 0;
	return r;
}

template<typename T>
Interval<T> DIExp(const Interval<T> &x) {
	bool finished;
	int k;
	int st = 0;
	Interval<T> d, e, w, w1;
	if (x.a > x.b)
		st = 1;
	else {
		e.a = 1;
		e.b = 1;
		w = e;
		k = 1;
		finished = false;
		st = 0;
		do {
			d.a = k;
			d.b = k;
			e = IMul(e, DIDiv(x, d));
			w1 = DIAdd(w, e);
			if ((abs(w.a - w1.a) / abs(w.a) < 1e-18)
					&& (abs(w.b - w1.b) / abs(w.b) < 1e-18)) {
				finished = true;
				return w1;
			} else {
				w = w1;
				k = k + 1;
			}
		} while (!(finished || (k > INT_MAX / 2)));
		if (!finished)
			st = 2;
	}
	Interval<T> r;
	r.a = 0;
	r.b = 0;
	return r;
}

template<typename T>
Interval<T> DISqr(const Interval<T> &x) {
	long double minx, maxx;
	int st = 0;
	Interval<T> r;
	r.a = 0;
	r.b = 0;
	if (x.a > x.b)
		st = 1;
	else {
		st = 0;
		if ((x.a <= 0) && (x.b >= 0))
			minx = 0;
		else if (x.a > 0)
			minx = x.a;
		else
			minx = x.b;
		if (abs(x.a) > abs(x.b))
			maxx = abs(x.a);
		else
			maxx = abs(x.b);
		SetRounding<T>(FE_DOWNWARD);
		r.a = minx * minx;
		SetRounding<T>(FE_UPWARD);
		r.b = maxx * maxx;
		SetRounding<T>(FE_TONEAREST);
	}
	return r;
}

template<typename T>
inline Interval<T> Interval<T>::operator +(const Interval<T> &y) {
	Interval<T> x(this->a, this->b);
	Interval<T> r = { 0, 0 };
	switch (mode) {
	case PINT_MODE:
		r = IAdd<T>(x, y);
		break;
	case DINT_MODE:
		r = DIAdd<T>(x, y);
		break;
	default:
		r = IAdd<T>(x, y);
		break;
	}

	return r;
}

template<typename T>
inline Interval<T> operator +(Interval<T> x, const Interval<T> &y) {
	switch (Interval<T>::mode) {
	case PINT_MODE:
		return IAdd<T>(x, y);
	case DINT_MODE:
		return DIAdd<T>(x, y);
	default:
		return IAdd<T>(x, y);
	}
}

template<typename T>
inline Interval<T> Interval<T>::operator -(const Interval<T> &y) {
	Interval<T> x(this->a, this->b);
	Interval<T> r = { 0, 0 };
	switch (mode) {
	case PINT_MODE:
		r = ISub<T>(x, y);
		break;
	case DINT_MODE:
		r = DISub<T>(x, y);
		break;
	default:
		r = ISub<T>(x, y);
		break;
	}

	return r;
}

template<typename T>
inline Interval<T> operator -(Interval<T> x, const Interval<T> &y) {
	switch (Interval<T>::mode) {
	case PINT_MODE:
		return ISub<T>(x, y);
	case DINT_MODE:
		return DISub<T>(x, y);
	default:
		return ISub<T>(x, y);
	}
}

template<typename T>
inline Interval<T> Interval<T>::operator *(const Interval<T> &y) {
	Interval<T> x(this->a, this->b);
	Interval<T> r = { 0, 0 };
	switch (mode) {
	case PINT_MODE:
		r = IMul<T>(x, y);
		break;
	case DINT_MODE:
		r = DIMul<T>(x, y);
		break;
	default:
		r = IMul<T>(x, y);
		break;
	}

	return r;
}

template<typename T>
inline Interval<T> operator *(int i, const Interval<T> &y) {
	Interval<T> x = { i, i };
	switch (Interval<T>::mode) {
	case PINT_MODE:
		return IMul<T>(x, y);
	case DINT_MODE:
		return DIMul<T>(x, y);
	default:
		return IMul<T>(x, y);
	}
}

template<typename T>
inline Interval<T> operator *(const Interval<T> &y, int i) {
	Interval<T> x = { i, i };
	switch (Interval<T>::mode) {
	case PINT_MODE:
		return IMul<T>(x, y);
	case DINT_MODE:
		return DIMul<T>(x, y);
	default:
		return IMul<T>(x, y);
	}
}

template<typename T>
inline Interval<T> operator *(long double i, const Interval<T> &y) {
	Interval<T> x = { i, i };
	switch (Interval<T>::mode) {
	case PINT_MODE:
		return IMul<T>(x, y);
	case DINT_MODE:
		return DIMul<T>(x, y);
	default:
		return IMul<T>(x, y);
	}
}

template<typename T>
inline Interval<T> operator *(const Interval<T> &y, long double i) {
	Interval<T> x = { i, i };
	switch (Interval<T>::mode) {
	case PINT_MODE:
		return IMul<T>(x, y);
	case DINT_MODE:
		return DIMul<T>(x, y);
	default:
		return IMul<T>(x, y);
	}
}

template<typename T>
inline Interval<T> operator *(Interval<T> x, const Interval<T> &y) {
	switch (Interval<T>::mode) {
	case PINT_MODE:
		return IMul<T>(x, y);
	case DINT_MODE:
		return DIMul<T>(x, y);
	default:
		return IMul<T>(x, y);
	}
}

template<typename T>
inline Interval<T> Interval<T>::operator *(const long double &l) {
	Interval<T> x(this->a, this->b);
	Interval<T> y = { l, l };
	Interval<T> r = { 0, 0 };
	switch (mode) {
	case PINT_MODE:
		r = IMul<T>(x, y);
		break;
	case DINT_MODE:
		r = DIMul<T>(x, y);
		break;
	default:
		r = IMul<T>(x, y);
		break;
	}

	return r;
}

template<typename T>
inline Interval<T> Interval<T>::operator *(const int &i) {
	Interval<T> x(this->a, this->b);
	Interval<T> y = { i, i };
	Interval<T> r = { 0, 0 };
	switch (mode) {
	case PINT_MODE:
		r = IMul<T>(x, y);
		break;
	case DINT_MODE:
		r = DIMul<T>(x, y);
		break;
	default:
		r = IMul<T>(x, y);
		break;
	}

	return r;
}

template<typename T>
inline Interval<T> Interval<T>::operator /(const Interval<T> &y) {
	Interval<T> x(this->a, this->b);
	Interval<T> r = { 0, 0 };
	switch (mode) {
	case PINT_MODE:
		r = IDiv<T>(x, y);
		break;
	case DINT_MODE:
		r = DIDiv<T>(x, y);
		break;
	default:
		r = IDiv<T>(x, y);
		break;
	}

	return r;
}

template<typename T>
inline Interval<T> operator /(Interval<T> x, const Interval<T> &y) {
	switch (Interval<T>::mode) {
	case PINT_MODE:
		return IDiv<T>(x, y);
	case DINT_MODE:
		return DIDiv<T>(x, y);
	default:
		return IDiv<T>(x, y);
	}
}

template<typename T>
Interval<T> Hull(const Interval<T> &x, const Interval<T> &y) {
	Interval<T> r = { 0, 0 };
	r.a = min(x.a, x.b);
	r.a = min(r.a, y.a);
	r.a = min(r.a, y.b);

	r.b = max(x.a, x.b);
	r.b = max(r.b, y.a);
	r.b = max(r.b, y.b);

	return r;
}

template<>
inline void Interval<mpreal>::IEndsToStrings(string &left, string &right) {
	mpfr_t rop;
	mpfr_exp_t exponent;
	mpfr_init2(rop, precision);
	char *str = NULL;
	char *buffer = new char(precision + 3);
	mpfr_set(rop, this->a.mpfr_ptr(), MPFR_RNDD);
	mpfr_get_str(buffer, &exponent, 10, outdigits, rop, MPFR_RNDD);
	str = buffer;

	stringstream ss;
	int prec = std::numeric_limits<mpreal>::digits10();
	ss.setf(std::ios_base::scientific);
	bool minus = (str[0] == '-');
	int splitpoint = minus ? 1 : 0;
	string sign = minus ? "-" : "";

	ss << std::setprecision(prec) << sign << str[splitpoint] << "."
			<< &str[splitpoint + 1] << "E" << exponent - 1;
	left = ss.str();
	ss.str(std::string());

	mpfr_set(rop, this->b.mpfr_ptr(), MPFR_RNDU);
	mpfr_get_str(buffer, &exponent, 10, outdigits, rop, MPFR_RNDU);
	str = buffer;
	splitpoint = (str[0] == '-') ? 1 : 0;
	ss << std::setprecision(prec) << sign << str[splitpoint] << "."
			<< &str[splitpoint + 1] << "E" << exponent - 1;
	right = ss.str();
	ss.clear();
}

template<>
inline mpreal DIntWidth<mpreal>(const Interval<mpreal> &x) {
	mpreal w1, w2;

	mpreal::set_default_rnd(MPFR_RNDU);
	w1 = x.b - x.a;
	if (w1 < 0)
		w1 = -w1;
	mpreal::set_default_rnd(MPFR_RNDD);
	w2 = x.b - x.a;
	if (w2 < 0)
		w2 = -w2;
	mpreal::set_default_rnd(MPFR_RNDN);
	if (w1 > w2)
		return w1;
	else
		return w2;
}

template<typename T>
Interval<T> IAbs(const Interval<T> &x) {
	T tmp = 0;
	Interval<T> r = { 0, 0 };
	r.a = abs(x.a);
	r.b = abs(x.b);
	if (r.b < r.a) {
		tmp = r.a;
		r.a = r.b;
		r.b = tmp;
	}

	return r;
}

//template<>
//inline mpreal Interval<mpreal>::GetWidth() {
//	Interval<mpreal> x(this->a, this->b);
//	switch (mode) {
//	case PINT_MODE:
//		return IntWidth(x);
//	case DINT_MODE:
//		return DIntWidth(x);
//	default:
//		return IntWidth(x);
//	}
//}

//The explicit instantiation part
template long double DIntWidth(const Interval<long double> &x);

//template mpreal DIntWidth(const Interval<mpreal>& x);
//template<>

template class Interval<long double> ;
template class Interval<double> ;
template class Interval<float> ;
template class Interval<mpreal> ;

template<typename T> IAMode Interval<T>::mode = PINT_MODE;
template<typename T> IAOutDigits Interval<T>::outdigits = LONGDOUBLE_DIGITS;

//template<> IAPrecision Interval<long double>::precision = LONGDOUBLE_PREC;
//template<> IAPrecision Interval<double>::precision = DOUBLE_PREC;
//template<> IAPrecision Interval<float>::precision = FLOAT_PREC;

template<> IAPrecision Interval<mpreal>::precision = MPREAL_PREC;
template<typename T> IAPrecision Interval<T>::precision = LONGDOUBLE_PREC;
//template IAOutDigits Interval<mpreal>::outdigits = LONGDOUBLE_DIGITS;

//-------------------------------------------------------------------------------------

} /* namespace interval_arithmetic */

#endif /* INTERVAL_H_ */
