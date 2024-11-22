#include <chrono>
#include <thread>

int main() {
    using namespace std::literals::chrono_literals;

    std::atomic<bool> stop = false;
    int               a    = 0;
    std::thread       t1([&]() {
        do {
            for (size_t i = 0; i < 100000; ++i) {
                a = i;
            }
        } while (!stop.load());
    });
    std::thread       t2([&]() {
        do {
            for (size_t i = 0; i < 100000; ++i) {
                a = i;
            }
        } while (!stop.load());
    });
    std::this_thread::sleep_for(5s);
    stop.store(true);
    t1.join();
    t2.join();
}