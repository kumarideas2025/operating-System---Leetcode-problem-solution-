#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

// ----------------- semaphore -----------------
class semaphore {
private:
    mutex mtx;
    condition_variable cv;
    int count;

public:
    semaphore() : count(0) {}
    semaphore(int c) : count(c) {}

    void setcount(int a) {
        count = a;
    }

    inline void sigma() { // increment (signal)
        unique_lock<mutex> lock(mtx);
        count++;
        if (count <= 0) {
            cv.notify_one();
        }
    }

    inline void wait() { // decrement (wait)
        unique_lock<mutex> lock(mtx);
        count--;
        while (count < 0) {
            cv.wait(lock);
        }
    }
};

// ----------------- Dining Philosophers -----------------
class DiningPhilosophers {
private:
    semaphore fork[5];
    mutex m;

public:
    DiningPhilosophers() {
        for (int i = 0; i < 5; ++i) {
            fork[i].setcount(1);
        }
    }

    void wantsToEat(int philosopher,
        function<void()> pickLeftFork,
        function<void()> pickRightFork,
        function<void()> eat,
        function<void()> putLeftFork,
        function<void()> putRightFork
    ) {
        // lock to avoid deadlocks when picking forks
        lock_guard<mutex> lock(m);

        fork[(philosopher + 1) % 5].wait();
        fork[philosopher].wait();

        pickLeftFork();
        pickRightFork();
        eat();
        putLeftFork();
        fork[(philosopher + 1) % 5].sigma();  
        putRightFork();
        fork[philosopher].sigma();            
    }
};

 int main() {
    DiningPhilosophers dp;

    auto pickLeft = []() { cout << "pickLeftFork "; };
    auto pickRight = []() { cout << "pickRightFork "; };
    auto eat = []() { cout << "eat "; };
    auto putLeft = []() { cout << "putLeftFork "; };
    auto putRight = []() { cout << "putRightFork "; };

    thread philosophers[5];

    for (int i = 0; i < 5; ++i) {
        philosophers[i] = thread([&dp, i, &pickLeft, &pickRight, &eat, &putLeft, &putRight]() {
            dp.wantsToEat(i, pickLeft, pickRight, eat, putLeft, putRight);
        });
    }

    for (int i = 0; i < 5; ++i)
        philosophers[i].join();

    cout << endl;

    return 0;
}