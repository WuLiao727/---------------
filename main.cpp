// 小学多边形面积、周长计算练习
#include "shape.h"
#include "derived.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include <iomanip>
#include <fstream>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <stdexcept>

using namespace std;

// 函数模板：生成 [lo, hi] 之间的随机整数，int / double 都能用
template <typename T = int>
T randInRange(T lo, T hi) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<T> dist(lo, hi);
    return dist(gen);
}

// 一道题：图形指针 + 标准答案 + 用户作答
struct Question {
    Shape*   shape_    = nullptr;
    double   ansArea_  = 0.0;
    double   ansPeri_  = 0.0;
    double   userArea_ = -1.0;
    double   userPeri_ = -1.0;
    bool     correct_  = false;

    Question() = default;
    Question(Shape* s)
        : shape_(s), ansArea_(s->area()), ansPeri_(s->perimeter()) {}

    ~Question() { delete shape_; }

    // 成员是指针，禁止拷贝避免 double free，保留移动
    Question(const Question&) = delete;
    Question& operator=(const Question&) = delete;
    Question(Question&& rhs) noexcept
        : shape_(rhs.shape_), ansArea_(rhs.ansArea_), ansPeri_(rhs.ansPeri_),
          userArea_(rhs.userArea_), userPeri_(rhs.userPeri_), correct_(rhs.correct_) {
        rhs.shape_ = nullptr;
    }
    Question& operator=(Question&& rhs) noexcept {
        if (this != &rhs) {
            delete shape_;
            shape_ = rhs.shape_; rhs.shape_ = nullptr;
            ansArea_ = rhs.ansArea_; ansPeri_ = rhs.ansPeri_;
            userArea_ = rhs.userArea_; userPeri_ = rhs.userPeri_;
            correct_ = rhs.correct_;
        }
        return *this;
    }
};

// 题库：vector 里存指针，存对象的话 vector<Shape> 会发生对象切片，多态失效
class QuestionBank {
    vector<Question> qs_;          // 题库
    int  total_   = 0;             // 出过的题数
    int  correct_ = 0;             // 答对的题数
    double sumScore_ = 0.0;        // 累计得分

public:
    ~QuestionBank() = default;

    // 出题并作答
    void generate(int n) {
        cout << "\n========== 开始答题（共 " << n << " 题）==========\n";
        cout << fixed << setprecision(Shape::PREC);

        for (int i = 0; i < n; ++i) {
            // 随机选一种图形，0~4 对应五种图形
            int type = randInRange(0, 4);
            Shape* s = makeShape(type);
            qs_.emplace_back(s);
            total_ = (int)qs_.size();

            cout << "\n第 " << i + 1 << " 题：";
            printPrompt(s);
            double ua, up;
            cout << "  你的答案 → 面积 = ";
            if (!(cin >> ua)) { clearInput(); continue; }
            cout << "  周长 = ";
            if (!(cin >> up)) { clearInput(); continue; }

            Question& q = qs_.back();
            q.userArea_ = ua;
            q.userPeri_ = up;
            q.correct_  = checkAnswer(q, ua, up);
            if (q.correct_) { correct_++; sumScore_ += 100.0; }
        }
        printReport();
    }

    // 随机生成图形，参数范围保证能构成合法图形
    Shape* makeShape(int type) {
        switch (type) {
            case 0: { // 三角形：先定两边，第三边取在两边之差~和之间
                int a = randInRange(3, 9), b = randInRange(3, 9);
                int c = randInRange(abs(a - b) + 1, a + b - 1);
                return new Triangle(a, b, c);
            }
            case 1: return new Rectangle(randInRange(2, 12), randInRange(2, 12));
            case 2: return new Square(randInRange(2, 12));
            case 3: { // 平行四边形：斜边要比高长
                int b = randInRange(3, 12);
                int h = randInRange(2, 8);
                int s = randInRange(h + 1, h + 8);
                return new Parallelogram(b, s, h);
            }
            case 4: { // 梯形：两腰之和要大于上下底之差
                int a = randInRange(3, 8);
                int b = randInRange(3, 8);
                int diff = abs(a - b) + 1;
                int c = randInRange(diff, diff + 6);
                int d = randInRange(diff, diff + 6);
                int h = randInRange(2, 8);
                return new Trapezoid(a, b, c, d, h);
            }
            default: throw runtime_error("非法图形类型");
        }
    }

    // 判分：相对误差小于 1% 就算对，不用浮点直接比较
    bool checkAnswer(const Question& q, double ua, double up) {
        auto ok = [](double a, double b) {
            if (a == 0.0) return b == 0.0;
            return fabs(a - b) / fabs(a) < 0.01;
        };
        return ok(q.ansArea_, ua) && ok(q.ansPeri_, up);
    }

    // 打印图形信息，用 dynamic_cast 判断具体类型
    void printPrompt(Shape* s) {
        if      (dynamic_cast<Triangle*>(s))       cout << "三角形，三边长 a/b/c";
        else if (dynamic_cast<Rectangle*>(s))      cout << "长方形，长 l、宽 w";
        else if (dynamic_cast<Square*>(s))         cout << "正方形，边长 a";
        else if (dynamic_cast<Parallelogram*>(s))  cout << "平行四边形，底 b、斜边 s、高 h";
        else if (dynamic_cast<Trapezoid*>(s))      cout << "梯形，上底 a、下底 b、两腰 c/d、高 h";
        cout << "（请分别输入面积与周长，保留 2 位小数）\n";
    }

    // 成绩汇总
    void printReport() const {
        cout << "\n========== 本次成绩单 ==========\n";
        cout << "总题数：" << total_ << "  答对：" << correct_
             << "  得分：" << fixed << setprecision(1)
             << (total_ ? sumScore_ / total_ : 0.0) << " 分\n";
        cout << "正确率：" << setprecision(1)
             << (total_ ? 100.0 * correct_ / total_ : 0.0) << "%\n";
    }

    // 题目回顾
    void review() const {
        cout << "\n========== 题目回顾 ==========\n";
        cout << fixed << setprecision(Shape::PREC);
        for (size_t i = 0; i < qs_.size(); ++i) {
            const auto& q = qs_[i];
            cout << "#" << i + 1 << "  "
                 << (q.correct_ ? "[正确]" : "[错误]")
                 << "  标准: 面积=" << q.ansArea_ << " 周长=" << q.ansPeri_;
            if (!q.correct_)
                cout << "  你的: 面积=" << q.userArea_ << " 周长=" << q.userPeri_;
            cout << "\n";
        }
    }

    // 存盘：一行一个对象，首字符是类型标签，空格分隔参数
    void save(const string& path) const {
        ofstream ofs(path);
        if (!ofs) throw runtime_error("无法打开文件写入：" + path);
        ofs << fixed << setprecision(6); // 多存几位，避免精度丢失
        for (const auto& q : qs_) {
            ofs << q.shape_->serialize() << "\n";
        }
        cout << "已保存 " << qs_.size() << " 题到 " << path << "\n";
    }

    // 读档：每行按首字符派发，new 出对应的派生类对象
    void load(const string& path) {
        ifstream ifs(path);
        if (!ifs) throw runtime_error("无法打开文件：" + path);
        qs_.clear();
        correct_ = total_ = 0; sumScore_ = 0.0;
        string line;
        while (getline(ifs, line)) {
            if (line.empty()) continue;
            Shape* s = Shape::deserialize(line);
            qs_.emplace_back(s);
        }
        cout << "已从 " << path << " 读入 " << qs_.size() << " 题\n";
    }

public:
    // 输入出错时清空错误状态，丢掉本行剩余内容
    static void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "输入格式有误，已跳过本题\n";
    }
};

int main() {
    QuestionBank bank;
    const string dataFile = "questions.dat";

    cout << "========================================\n"
         << " 小学多边形面积·周长计算练习\n" 
         << " 面向对象程序设计 实验 3~6 综合项目\n"
         << "========================================\n";

    while (true) {
        cout << "\n[1] 开始答题  [2] 回顾错题  [3] 保存题库  [4] 读取题库  [0] 退出\n"
             << "请选择：";
        int choice = 0;
        cin >> choice;
        if (!cin) { QuestionBank::clearInput(); continue; }

        switch (choice) {
            case 1: {
                int n = 0;
                cout << "要出几道题？";
                cin >> n;
                if (cin && n > 0) bank.generate(n);
                else { cout << "题数无效\n"; QuestionBank::clearInput(); }
                break;
            }
            case 2: bank.review(); break;
            case 3: bank.save(dataFile); break;
            case 4: bank.load(dataFile); break;
            case 0: cout << "再见！\n"; return 0;
            default: cout << "无效选项\n";
        }
    }
}
