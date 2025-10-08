// ===========================================================================
// ThreadLocalStorage.cpp // Thread Local Storage
// ===========================================================================

#include <mutex>
#include <print>
#include <thread>

thread_local int x{ 123 };

static thread_local int xx{ 0 }; // static: xx ist nur in dieser Datei ansprechbar.

namespace ThreadLocalStorage {

    // =======================================================================

    std::mutex mutex{};

    static void function()
    {
        thread_local int y{ 0 };

        {
            std::lock_guard<std::mutex> guard{ mutex };

            x++;
            y++;

            std::println("TID:  {} ", std::this_thread::get_id());
            std::println("  &x: {:#010x} => {}", reinterpret_cast<intptr_t>(&x), x);
            std::println("  &y: {:#010x} => {}", reinterpret_cast<intptr_t>(&y), y);
        }
    }
}

void test_thread_local_storage_01() {

    using namespace ThreadLocalStorage;

    std::println("test_thread_local_storage_01: {} ", std::this_thread::get_id());
    std::println("  &x: {:#010x} => {}", reinterpret_cast<intptr_t>(&x), x);

    function();
    std::jthread worker1{ function };
    function();
    std::jthread worker2{ function };
    function();
}

// ===========================================================================
// End-of-File
// ===========================================================================
