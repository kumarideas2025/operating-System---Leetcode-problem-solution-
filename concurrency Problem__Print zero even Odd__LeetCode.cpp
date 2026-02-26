 #include <iostream>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

using namespace std;

class ZeroEvenOdd {
private:
    int n;
    mutex m;
    condition_variable cv;
    int turn; // 0 = zero, 1 = odd, 2 = even
    int i;

public:
    ZeroEvenOdd(int n) {
        this->n = n;
        turn = 0;
        i = 1;
    }

    void zero(function<void(int)> printNumber){
        while(i <= n){
            unique_lock<mutex> lock(m);
            while(turn != 0 && i <= n){
                cv.wait(lock);
            }
            if(i > n) break;
            printNumber(0);
            turn = (i % 2 == 0) ? 2 : 1;
            cv.notify_all();
        }
    }

    void even(function<void(int)> printNumber){
        while(i <= n){
            unique_lock<mutex> lock(m);
            while(turn != 2 && i <= n){
                cv.wait(lock);
            }
            if(i > n) break;
            printNumber(i++);
            turn = 0;
            cv.notify_all();
        }
    }

    void odd(function<void(int)> printNumber){
        while(i <= n){
            unique_lock<mutex> lock(m);
            while(turn != 1 && i <= n){
                cv.wait(lock);
            }
            if(i > n) break;
            printNumber(i++);
            turn = 0;
            cv.notify_all();
        }
    }
};


int main() {
    int n = 5; 
    ZeroEvenOdd zeo(n);

    auto printNumber = [](int x){ cout << x; };

    thread t0([&](){ zeo.zero(printNumber); });
    thread t1([&](){ zeo.even(printNumber); });
    thread t2([&](){ zeo.odd(printNumber); });

    t0.join();
    t1.join();
    t2.join();

    cout << endl;
    return 0;
}