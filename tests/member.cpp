#include "thread_pool.h"
#include <thread>
#include <random>
#include <iostream>
#include <functional>

using namespace std;
using namespace thread_pool;
using namespace placeholders;

struct Task_container {
    void process (int ms,int &ms1){
        wait_time = ms+ms1;
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
    }
    int wait_time;
};

int main(){
    Thread_pool tp;
    vector<Task_container> tasks (tp.workers.size() * 3);
    cout << "Running " << tasks.size() << " in " << tp.workers.size() << " parallel workers " << endl;
    int ms1 = rand() % 1000;
    for (auto &task:tasks) {
//        auto &t = tp.run([task = &task](int ms) { task->process(ms); }, rand() % 1000);
//        auto &t = tp.run(instance_method(task, process), rand() % 1000);

//        auto f = std::bind(&Task_container::process, &task, _1);
//        auto &t = tp.run( f, rand() % 1000);

        auto &t = tp.run(&Task_container::process, &task, rand() % 1000, std::ref(ms1));
        cout << "started worker " << tp.get_worker_id(t) << endl;
    }
    tp.wait_all();
    for (auto &task:tasks) {
        cout << "task waited for " << task.wait_time << " milliseconds " << endl;
    }
}