#include "derived.h"
#include <cmath>
#include <sstream>
#include <stdexcept>

using namespace std;

// 三角形
Triangle::Triangle(double a, double b, double c)
    : a_(a), b_(b), c_(c) {
    if (a <= 0 || b <= 0 || c <= 0)
        throw invalid_argument("三角形边长必须为正");
    // 两边之和要大于第三边
    if (a + b <= c || a + c <= b || b + c <= a)
        throw invalid_argument("三边长不满足三角不等式");
}

double Triangle::area() const {
    double p = (a_ + b_ + c_) / 2.0; // 半周长
    return sqrt(p * (p - a_) * (p - b_) * (p - c_)); // 海伦公式
}

double Triangle::perimeter() const {
    return a_ + b_ + c_;
}

string Triangle::serialize() const {
    ostringstream oss;
    oss << "T " << a_ << " " << b_ << " " << c_; // 行首 T 表示三角形
    return oss.str();
}

double Triangle::getA() const { return a_; }
double Triangle::getB() const { return b_; }
double Triangle::getC() const { return c_; }

// 长方形
Rectangle::Rectangle(double l, double w) : l_(l), w_(w) {
    if (l <= 0 || w <= 0) throw invalid_argument("长宽必须为正");
}

double Rectangle::area() const { return l_ * w_; }
double Rectangle::perimeter() const { return 2.0 * (l_ + w_); }

string Rectangle::serialize() const {
    ostringstream oss;
    oss << "R " << l_ << " " << w_;
    return oss.str();
}

double Rectangle::getL() const { return l_; }
double Rectangle::getW() const { return w_; }

// 正方形
Square::Square(double a) : a_(a) {
    if (a <= 0) throw invalid_argument("边长必须为正");
}

double Square::area() const { return a_ * a_; }
double Square::perimeter() const { return 4.0 * a_; }

string Square::serialize() const {
    ostringstream oss;
    oss << "S " << a_;
    return oss.str();
}

double Square::getA() const { return a_; }

// 平行四边形
Parallelogram::Parallelogram(double b, double s, double h)
    : b_(b), s_(s), h_(h) {
    if (b <= 0 || s <= 0 || h <= 0) throw invalid_argument("底/斜边/高必须为正");
}

double Parallelogram::area() const { return b_ * h_; } // 底 × 高
double Parallelogram::perimeter() const { return 2.0 * (b_ + s_); }

string Parallelogram::serialize() const {
    ostringstream oss;
    oss << "P " << b_ << " " << s_ << " " << h_;
    return oss.str();
}

double Parallelogram::getB() const { return b_; }
double Parallelogram::getS() const { return s_; }
double Parallelogram::getH() const { return h_; }

// 梯形
Trapezoid::Trapezoid(double a, double b, double c, double d, double h)
    : a_(a), b_(b), c_(c), d_(d), h_(h) {
    if (a <= 0 || b <= 0 || c <= 0 || d <= 0 || h <= 0)
        throw invalid_argument("梯形各边与高必须为正");
}

double Trapezoid::area() const {
    return (a_ + b_) * h_ / 2.0; // (上底 + 下底) × 高 ÷ 2
}

double Trapezoid::perimeter() const {
    return a_ + b_ + c_ + d_;
}

string Trapezoid::serialize() const {
    ostringstream oss;
    oss << "Z " << a_ << " " << b_ << " " << c_ << " " << d_ << " " << h_;
    return oss.str();
}

double Trapezoid::getA() const { return a_; }
double Trapezoid::getB() const { return b_; }
double Trapezoid::getC() const { return c_; }
double Trapezoid::getD() const { return d_; }
double Trapezoid::getH() const { return h_; }
