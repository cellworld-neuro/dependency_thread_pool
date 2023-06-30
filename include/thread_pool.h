#pragma once
#include <functional>
#include <future>
#include <algorithm>
#include <vector>
#include <iostream>

namespace thread_pool {
    template<int N>
    struct my_placeholder {
        static my_placeholder ph;
    };

    template<int N>
    my_placeholder<N> my_placeholder<N>::ph;

}

namespace std {
    template<int N>
    struct is_placeholder<thread_pool::my_placeholder<N>> : std::integral_constant<int, N> { };
}

namespace thread_pool {
    thread_local int worker_id = -1;
    struct Thread_pool {
        Thread_pool() : Thread_pool(std::thread::hardware_concurrency()) {};

        static unsigned int max_concurrency(){
            return std::thread::hardware_concurrency();
        }

        template<int... indices>
        static auto bind_all(auto f, auto val, std::integer_sequence<int, indices...>) {
            return std::bind(f, val, my_placeholder<indices+1>::ph...);
        }

        explicit Thread_pool(unsigned int worker_count) : workers(worker_count), available_worker(worker_count){
            for (auto &w:available_worker) w = true;
        }
        template<typename Callable, typename... Args,
                typename = std::_Require< std::__not_<std::is_same<std::__remove_cvref_t<Callable>, std::thread>>>>
        std::thread &run(Callable f, Args... args) {
            while (true){
                for (unsigned int w = 0; w < available_worker.size() ; w++) {
                    auto &available = available_worker[w];
                    if (available) {
                        available = false;
                        if (workers[w].joinable()) workers[w].join();

                        if constexpr( std::is_member_function_pointer<decltype(f)>::value) {
                            workers[w] = std::thread([w, &available, f](auto o, auto ...args) {
                                constexpr std::size_t n = sizeof...(args);
                                worker_id = w;
                                auto bf = Thread_pool::bind_all(f, o,  std::make_integer_sequence<int, sizeof...(args)>());
                                bf(args...);
                                available = true;
                            }, args...);
                        } else {
                            workers[w] = std::thread([w, &available, f](auto ...args) {
                                worker_id = w;
                                f(args...);
                                available = true;
                            }, args...);
                        }
                        if (set_affinity) {
                            cpu_set_t cpuset;
                            CPU_ZERO(&cpuset);
                            CPU_SET(w, &cpuset);
                            int rc = pthread_setaffinity_np(workers[w].native_handle(),
                                                            sizeof(cpu_set_t), &cpuset);
                        }
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
        bool set_affinity = false;
    };
}