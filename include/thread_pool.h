#pragma once
#include <functional>
#include <future>
#include <algorithm>
#include <vector>

namespace thread_pool {

    thread_local int worker_id = -1;

    struct Thread_pool {
        Thread_pool() : Thread_pool(std::thread::hardware_concurrency()) {};

        explicit Thread_pool(unsigned int worker_count) : workers(worker_count), available_worker(worker_count){
            for (auto &w:available_worker) w = true;
        }
        template<typename _Callable, typename... _Args,
                typename = std::_Require< std::__not_<std::is_same<std::__remove_cvref_t<_Callable>, std::thread>>>>
        std::thread &run(_Callable&& __f, _Args&&... __args) {
            while (true){
                for (unsigned int w = 0; w < available_worker.size() ; w++) {
                    auto &available = available_worker[w];
                    if (available) {
                        available = false;
                        if (workers[w].joinable()) workers[w].join();
                        workers[w] = std::thread([w, &available, &__f, __args...] () { worker_id = w;(*__f) (__args...); available = true;});
                        return workers[w];
                    }
                }
                if (off_time_ms) std::this_thread::sleep_for(std::chrono::milliseconds(off_time_ms));
            }
        }

        int get_worker_id(std::thread &t){
            int worker_id = 0;
            for (auto const &w:workers){
                if (&w == &t) return worker_id;
                worker_id++;
            }
            throw std::runtime_error("worker not found.");
        }

        void set_off_time_ms(unsigned int t){
            off_time_ms = t;
        }

        void wait_all(){
            for (auto &w:workers) if (w.joinable()) w.join();
        }

        ~Thread_pool(){
            wait_all();
        }

        unsigned int off_time_ms = 10;
        std::vector<std::thread> workers;
        std::vector<std::atomic<bool>> available_worker;
    };
}