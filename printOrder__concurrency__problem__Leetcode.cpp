#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

class Foo {
private:
    mutex m;
    condition_variable cv;
    int turn;

public:
    Foo() {
        turn = 0;
    }

    void first(function<void()> printFirst) {
         printFirst();
        turn = 1;
        cv.notify_all();
    }

    void second(function<void()> printSecond) {
        unique_lock<mutex> lock(m);
        while (turn != 1) {
            cv.wait(lock);
        }
        printSecond();
        turn = 2;
        cv.notify_all();
    }

    void third(function<void()> printThird) {
        unique_lock<mutex> lock(m);
        while (turn != 2) {
            cv.wait(lock);
        }
         printThird();
    }
};

 int main() {
    Foo fooObj;

    auto printFirst = []() { cout << "first "; };
    auto printSecond = []() { cout << "second "; };
    auto printThird = []() { cout << "third "; };

    
    thread t1([&]() { fooObj.first(printFirst); });
    thread t2([&]() { fooObj.second(printSecond); });
    thread t3([&]() { fooObj.third(printThird); });

    t1.join();
    t2.join();
    t3.join();

    cout << endl;
    return 0;
}