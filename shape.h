// 抽象基类：所有图形的公共接口
#ifndef SHAPE_H
#define SHAPE_H

#include <fstream>
#include <string>

class Shape {
public:
    // 虚析构：通过基类指针 delete 派生类对象时，能正确调用派生类的析构
    virtual ~Shape() = default;

    // 三个纯虚函数，每个派生类都要自己实现
    virtual double area() const = 0;          // 面积
    virtual double perimeter() const = 0;     // 周长
    virtual std::string serialize() const = 0; // 序列化成一行文本，存盘用

    // 面积周长比，各图形共用，不需要虚
    double areaPerimeterRatio() const {
        double p = perimeter();
        return (p == 0.0) ? 0.0 : area() / p;
    }

    // 静态工厂：按行首类型字符（T/R/S/P/Z）恢复对应派生类，读档时用
    static Shape* deserialize(const std::string& line);

    // EPS：浮点比较容差；PREC：输出保留的小数位数
    static constexpr double EPS = 1e-9;
    static constexpr int    PREC = 2;
};

#endif // SHAPE_H
