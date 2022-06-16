# Thread pool

Simple controlled concurrency.

---

**[Try it out on wandbox!](https://wandbox.org/permlink/sJYSXdsxhLAbuzp7)**

## Summary

Thread_pool limits the number of concurrent threads running. By default, it uses the number of available processors to prevent overloading the system.


## Build & Test

The recommended way to use any of this library is to just include it.

You can optionally build using [CMake](https://cmake.org/). Here is a guide of how to build and test using the CMake Makefile generator.

```bash
mkdir build
cd build
cmake ..
sudo make install  
```


## Examples


### Autodetect available processors:


```c++
#include "thread_pool.h"
#include <iostream>

using namespace std;
using namespace thread_pool;

int main(){
    Thread_pool tp;
    cout << "Thread_pool detected " << tp.workers.size() << " processors " << endl;
}
```


### Specify number of processors to use:

```c++
#include "thread_pool.h"
#include <iostream>

using namespace std;
using namespace thread_pool;

int main(){
    Thread_pool tp(4);
    cout << "Thread_pool initialized with " << tp.workers.size() << " parallel workers " << endl;
}
```

### Run parallel tasks - Lambda

```c++
#include "thread_pool.h"
#include <random>
#include <iostream>

using namespace std;
using namespace thread_pool;

int main(){
    Thread_pool tp;
    size_t task_count = tp.workers.size() * 10;
    cout << "Running " << task_count << " in " << tp.workers.size() << " parallel workers " << endl;
    for (size_t i = 0;i < task_count ; i++) {
        auto &t = tp.run([](unsigned int ms){
          std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }, rand() % 2000);
        cout << "started worker " << tp.get_worker_id(t) << endl;
    }
    tp.wait_all();
}
```

### Getting worker id from inside the worker thread:

```c++
#include "thread_pool.h"
#include <random>
#include <iostream>

using namespace std;
using namespace thread_pool;

int main(){
    Thread_pool tp;
    cout << "Thread_pool detected " << tp.workers.size() << " processors " << endl;
    for (size_t i = 0;i < tp.workers.size() * 2 ; i++) {
        auto &t = tp.run([](unsigned int ms){
          cout << "Worker id: " << thread_pool::worker_id << endl;
          std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }, rand() % 2000);
        cout << "started worker " << tp.get_worker_id(t) << endl;
    }
    tp.wait_all();
}
```



### Run parallel tasks - Function

```c++
#include "thread_pool.h"
#include <random>
#include <iostream>

using namespace std;
using namespace thread_pool;

void task (unsigned int ms){
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main(){
    Thread_pool tp;
    size_t task_count = tp.workers.size() * 10;
    cout << "Running " << task_count << " in " << tp.workers.size() << " parallel workers " << endl;
    for (size_t i = 0;i < task_count ; i++) {
        auto &t = tp.run(task, rand() % 2000);
        cout << "started worker " << tp.get_worker_id(t) << endl;
    }
    tp.wait_all();
}
```


### Run parallel tasks - instance member method

```c++
#include "thread_pool.h"
#include <thread>
#include <random>
#include <iostream>
#include <functional>

using namespace std;
using namespace thread_pool;

struct Task_container {
    void process (int ms){
        wait_time = ms+ms1;
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
    }
    int wait_time;
};

int main(){
    Thread_pool tp;
    vector<Task_container> tasks (tp.workers.size() * 3);
    cout << "Running " << tasks.size() << " in " << tp.workers.size() << " parallel workers " << endl;
    for (auto &task:tasks) {
        auto &t = tp.run(&Task_container::process, &task, rand() % 1000);
        cout << "started worker " << tp.get_worker_id(t) << endl;
    }
    tp.wait_all();
    for (auto &task:tasks) {
        cout << "task waited for " << task.wait_time << " milliseconds " << endl;
    }
}
```

### Passing parameters as reference - Function

```c++
#include "thread_pool.h"
#include <random>
#include <iostream>

using namespace std;
using namespace thread_pool;

void task (unsigned int &ms){
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main(){
    Thread_pool tp;
    size_t task_count = tp.workers.size() * 10;
    cout << "Running " << task_count << " in " << tp.workers.size() << " parallel workers " << endl;
    int ms = rand() % 2000;
    for (size_t i = 0;i < task_count ; i++) {
        auto &t = tp.run(task, std:ref(ms));
        cout << "started worker " << tp.get_worker_id(t) << endl;
    }
    tp.wait_all();
}
```

