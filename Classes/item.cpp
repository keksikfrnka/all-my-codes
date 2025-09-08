#include <iostream>
#include <string>
#include "item.hpp"


std::istream& operator>>(std::istream& in, Real& value) {
    double digit;
    in >> digit;

    value = digit;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Real& value) {
    if (value.is_negative_) {
        out << '-';
    }
    out << value.int_part_;
    if (value.fract_part_ > 0) {
        out << '.';
        out << value.fract_part_;
    }
    return out;
}

Real::Real() :
    int_part_(0),
    fract_part_(0),
    is_negative_(false)
{ }
Real::Real(double x) :
    int_part_(int(x)),
    fract_part_(int(x * 1000) % 1000),
    is_negative_(false)
   
{ 
    if (x < 0) {
        is_negative_ = true;
        int_part_ = (-1) * int_part_;
        fract_part_ = (-1) * fract_part_;
    }
}
Real::Real(int aint_part, int afract_part, bool ais_negative) :
    int_part_(aint_part),
    fract_part_(afract_part),
    is_negative_(ais_negative)
{ }
Real::Real(const Real& another) :
    int_part_(another.int_part_),
    fract_part_(another.fract_part_),
    is_negative_(another.is_negative_)
{ }

Real& Real::operator+=(const Real& another) {
    int_part_ += another.int_part_;
    fract_part_ += another.fract_part_;
    is_negative_ += another.is_negative_;
    return *this;
}
Real& Real::operator+=(int x) {
    int_part_ += x;
    return *this;
}

Real operator+(const Real& first, const Real& second) {
    Real copy(first);
    copy += second;
    return copy;
}
Real operator+(const Real& first, int x) {
    Real copy(first);
    copy += x;

    return copy;
}
Real& Real::operator*=(const Real& another) {
    int first = int_part_ * 1000 + fract_part_;
    int second = another.int_part_ * 1000 + another.fract_part_;
    first *= second;
    int_part_ = first / 1000000;
    fract_part_ = (first % 1000000) / 1000;
    is_negative_ = is_negative_ != another.is_negative_;
    return *this;
}
Real operator*(const Real& first, const Real& second) {
    Real copy(first);
    copy *= second;
    return copy;
}

bool Real::operator<(const Item& item) {
    bool flag;
    const Real& other = dynamic_cast<const Real&>(item);

    if (!is_negative_) {
        if (other.is_negative_) {
            flag = false;
        }
        else {
            if ((int_part_ < other.int_part_) || ((int_part_ == other.int_part_) && (fract_part_ < other.fract_part_))) {
                flag = true;
            }
            else {
                flag = false;
            }
        }
    }
    if (is_negative_) {
        if (!other.is_negative_) {
            flag = true;
        }
        else {
            if ((int_part_ > other.int_part_) || ((int_part_ == other.int_part_) && (fract_part_ > other.fract_part_))) {
                flag = true;
            }
            else {
                flag = false;
            }
        }
    }

    return flag;
}

bool Real::operator==(const Item& item) {
    bool flag;
    const Real& other = dynamic_cast<const Real&>(item);

    if (int_part_ == other.int_part_){
        if (fract_part_ == other.fract_part_) {
            if (is_negative_ == other.is_negative_)
            flag = true;
        }
    } else {
        flag = false;
    }

    return flag;
}

Real& Real:: operator=(const Real& another) {
    is_negative_ = another.is_negative_;
    int_part_ = another.int_part_;
    fract_part_ = another.fract_part_;
    return *this;
}

Real:: ~Real() {

}

Pair::Pair() :
    a(0, 0, false),
    b(0, 0, false)
{ }

Pair::Pair(double x, double y) :
    a(x),
    b(y)
{ }

Pair::Pair(const Pair& another) :
    a(another.a),
    b(another.b)
{ }

std::ostream& operator<<(std::ostream& out, const Pair& value) {
    out << "(" << value.a << ", " << value.b << ")";
    return out;
}

std::istream& operator>>(std::istream& in, Pair& value) {
    in >> value.a >> value.b;
    return in;
}

bool Pair::operator<(const Item& item) {
    bool flag;
    const Pair& other = dynamic_cast<const Pair&>(item);

    if (b < other.b)
        flag = true;
    else
        flag = false;

    return flag;
}

bool Pair::operator==(const Item& item) {
    bool flag;
    const Pair& other = dynamic_cast<const Pair&>(item);

    if (b == other.b) {
        flag = true;
    } else {
        flag = false;
    }
    return flag;
}

Pair& Pair::operator=(const Pair& another) {
    a = another.a;
    b = another.b;
    return *this;
}

Pair::~Pair() 
{ }