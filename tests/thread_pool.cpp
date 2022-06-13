#include <thread_pool.h>
#include <random>
#include <iostream>

using namespace std;
using namespace thread_pool;

void random_duration_task (unsigned int ms) {
    cout << "waiting "<< ms << " milliseconds on worker " << thread_pool::worker_id << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}


int main(){
    Thread_pool tp;
    for (int i = 0;i <100 ; i++) {
        auto &t = tp.run(random_duration_task, rand() % 10000);
        cout << "started worker " << tp.get_worker_id(t) << endl;
    }
}