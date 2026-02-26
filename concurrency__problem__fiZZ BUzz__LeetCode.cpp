#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

class FizzBuzz {
private:
    int n;
    mutex m;
    condition_variable c;
    int i;

public:
    FizzBuzz(int n) {
        this->n = n;
        this->i = 1;
    }

    void fizz(function<void()> printFizz) {
        while (i <= n) {
            unique_lock<mutex> lock(m);   
            while (i <= n && !((i % 3 == 0) && (i % 5 != 0))) {
                c.wait(lock);
            }
            if (i <= n) {
                printFizz();
                ++i;
            }
            c.notify_all();
        }
    }

    void buzz(function<void()> printBuzz) {
        while (i <= n) {
            unique_lock<mutex> lock(m);   
            while (i <= n && !((i % 5 == 0) && (i % 3 != 0))) {
                c.wait(lock);
            }
            if (i <= n) {
                printBuzz();
                ++i;
            }
            c.notify_all();
        }
    }

    void fizzbuzz(function<void()> printFizzBuzz) {
        while (i <= n) {
            unique_lock<mutex> lock(m);   
            while (i <= n && !((i % 3 == 0) && (i % 5 == 0))) {
                c.wait(lock);
            }
            if (i <= n) {
                printFizzBuzz();
                ++i;
            }
            c.notify_all();
        }
    }

    void number(function<void(int)> printNumber) {
        while (i <= n) {
            unique_lock<mutex> lock(m);  // fixed syntax
            while (i <= n && ((i % 3 == 0) || (i % 5 == 0))) {
                c.wait(lock);
            }
            if (i <= n) {
                printNumber(i++);
            }
            c.notify_all();
        }
    }
};

 int main() {
    int n = 15;
    FizzBuzz fb(n);

    auto printFizz = []() { cout << "Fizz "; };
    auto printBuzz = []() { cout << "Buzz "; };
    auto printFizzBuzz = []() { cout << "FizzBuzz "; };
    auto printNumber = [](int x) { cout << x << " "; };

    thread t1([&](){ fb.fizz(printFizz); });
    thread t2([&](){ fb.buzz(printBuzz); });
    thread t3([&](){ fb.fizzbuzz(printFizzBuzz); });
    thread t4([&](){ fb.number(printNumber); });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << endl;
    return 0;
}