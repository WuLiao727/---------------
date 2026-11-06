#ifndef DERIVED_H
#define DERIVED_H

#include "shape.h"
#include <string>

// 三角形：三条边长 a, b, c，面积用海伦公式
class Triangle : public Shape {
    double a_, b_, c_;
public:
    Triangle(double a, double b, double c);
    double area() const override;
    double perimeter() const override;
    std::string serialize() const override;
};

// 长方形：长 l，宽 w
class Rectangle : public Shape {
    double l_, w_;
public:
    Rectangle(double l, double w);
    double area() const override;
    double perimeter() const override;
    std::string serialize() const override;
};

// 正方形：边长 a
class Square : public Shape {
    double a_;
public:
    Square(double a);
    double area() const override;
    double perimeter() const override;
    std::string serialize() const override;
};

// 平行四边形：底 b，斜边 s，高 h
class Parallelogram : public Shape {
    double b_, s_, h_;
public:
    Parallelogram(double b, double s, double h);
    double area() const override;
    double perimeter() const override;
    std::string serialize() const override;
};

// 梯形：上底 a，下底 b，两腰 c、d，高 h
class Trapezoid : public Shape {
    double a_, b_, c_, d_, h_;
public:
    Trapezoid(double a, double b, double c, double d, double h);
    double area() const override;
    double perimeter() const override;
    std::string serialize() const override;
};

#endif // DERIVED_H
