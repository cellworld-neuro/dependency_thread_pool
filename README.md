# Thread pool

Simple controlled concurrency.

---

**[Try it out on wandbox!](https://wandbox.org/permlink/ufY2jT08MiblWNYN)**

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
#include <random>
#include <iostream>

using namespace std;
using namespace thread_pool;

int main(){
    Thread_pool tp;
    cout << "Thread_pool detected " << tp.workers.size() << " processors " << endl;
    for (size_t i = 0;i < tp.workers.size() * 2 ; i++) {
        auto &t = tp.run([](unsigned int ms){
            cout << "waiting " << ms << endl;
          std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }, rand() % 2000);
    }
    tp.wait_all();
}
```


### Specify number of processors to use:

```c++
#include "thread_pool.h"
#include <random>
#include <iostream>

using namespace std;
using namespace thread_pool;

int main(){
    Thread_pool tp(4);
    cout << "Thread_pool detected " << tp.workers.size() << " processors " << endl;
    for (size_t i = 0;i < tp.workers.size() * 2 ; i++) {
        auto &t = tp.run([](unsigned int ms){
          std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }, rand() % 2000);
        cout << "started worker " << tp.get_worker_id(t) << endl;
    }
    tp.wait_all();
}
```

### Specify number of processors to use:

```c++
#include "thread_pool.h"
#include <random>
#include <iostream>

using namespace std;
using namespace thread_pool;

int main(){
    Thread_pool tp(4);
    cout << "Thread_pool detected " << tp.workers.size() << " processors " << endl;
    for (size_t i = 0;i < tp.workers.size() * 2 ; i++) {
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
