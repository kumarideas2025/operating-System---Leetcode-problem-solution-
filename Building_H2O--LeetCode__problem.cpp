 #include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

class H2O
{
private:
    mutex m;
    condition_variable cv;
    int turn;

public:
    H2O()
    {
        turn = 0;
    }

    void hydrogen(function<void()> releasehydrogen)
    {
        unique_lock<mutex> lock(m);
        while (turn == 2)
        {
            cv.wait(lock);
        }
        releasehydrogen();
        ++turn;
        cv.notify_all();
    }

    void oxygen(function<void()> releaseOxygen)
    {
        unique_lock<mutex> lock(m);
        while (turn < 2)
        {
            cv.wait(lock);
        }
        releaseOxygen();
        turn = 0;
        cv.notify_all();
    }
};

 


int main()
{
    H2O h2o;

    auto releaseH = []()
    { cout << "H"; };
    auto releaseO = []()
    { cout << "O"; };

    thread threads[3];

    threads[0] = thread([&]()
                        { h2o.hydrogen(releaseH); });
    threads[1] = thread([&]()
                        { h2o.oxygen(releaseH); });

    threads[2] = thread([&]()
                        { h2o.hydrogen(releaseO); });

    for (int i = 0; i < 3; i++)
        threads[i].join();

    cout << endl;  

    return 0;
}