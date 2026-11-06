#include "shape.h"
#include "derived.h"   // 反序列化需要认识全部派生类
#include <sstream>
#include <stdexcept>

using namespace std;

// 静态工厂：按类型字符 new 出对应的派生类对象
Shape* Shape::deserialize(const string& line) {
    istringstream iss(line);
    char type = 0;
    iss >> type;

    switch (type) {
        case 'T': { // 三角形：三边长 a b c
            double a, b, c;
            if (!(iss >> a >> b >> c)) throw runtime_error("三角形参数不足");
            return new Triangle(a, b, c);
        }
        case 'R': { // 长方形：长 l 宽 w
            double l, w;
            if (!(iss >> l >> w)) throw runtime_error("长方形参数不足");
            return new Rectangle(l, w);
        }
        case 'S': { // 正方形：边长 a
            double a;
            if (!(iss >> a)) throw runtime_error("正方形参数不足");
            return new Square(a);
        }
        case 'P': { // 平行四边形：底 b 斜边 s 高 h
            double b, s, h;
            if (!(iss >> b >> s >> h)) throw runtime_error("平行四边形参数不足");
            return new Parallelogram(b, s, h);
        }
        case 'Z': { // 梯形：上底 a 下底 b 腰 c 腰 d 高 h
            double a, b, c, d, h;
            if (!(iss >> a >> b >> c >> d >> h)) throw runtime_error("梯形参数不足");
            return new Trapezoid(a, b, c, d, h);
        }
        default:
            throw runtime_error(string("未知图形类型: ") + type);
    }
}
