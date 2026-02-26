#include <iostream>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

using namespace std;

class FooBar {
private:
    int n;
    mutex m;
    condition_variable cv;
    bool turn;

public:
    FooBar(int n) {
        this->n = n;
        turn = 0;   
    }

    void foo(function<void()> printFoo)
    {
        for (int i = 0; i < n; i++) {
            unique_lock<mutex> lock(m);

            while (turn == 1) {
                cv.wait(lock);
            }

            printFoo();
            turn = 1;
            cv.notify_all();
        }
    }

    void bar(function<void()> printBar)
    {
        for (int i = 0; i < n; i++) {
            unique_lock<mutex> lock(m);

            while (turn == 0) {
                cv.wait(lock);
            }

            printBar();
            turn = 0;
            cv.notify_all();
        }
    }
};

// -------------------- main --------------------
int main() {
    FooBar fb(5); 

    thread t1([&]() { fb.foo([](){ cout << "foo"; }); });
    thread t2([&]() { fb.bar([](){ cout << "bar"; }); });

    t1.join();
    t2.join();

    cout << endl;
    return 0;
}