#include "thread_pool.h"
#include <thread>
#include <random>
#include <iostream>
#include <functional>

using namespace std;
using namespace thread_pool;
using namespace std::placeholders;

struct Task_container {
    void task (int ms){
        wait_time = ms;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
    int wait_time;
};

int main(){
    Thread_pool tp;
    vector<Task_container> tasks (tp.workers.size() * 10);
    cout << "Running " << tasks.size() << " in " << tp.workers.size() << " parallel workers " << endl;
    for (auto &task:tasks) {
        auto &t = tp.run(bind (&Task_container::task, &task, _1), rand() % 2000);
        cout << "started worker " << tp.get_worker_id(t) << endl;
    }
    tp.wait_all();
    for (auto &task:tasks) {
        cout << "task waited for " << task.wait_time << " milliseconds " << endl;
    }

}