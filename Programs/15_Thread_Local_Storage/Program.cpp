// ===========================================================================
// Program.cpp //  Thread Local Storage
// ===========================================================================

#include <print>
#include <thread>

extern void test_thread_local_storage_01();

extern thread_local int x;

int main()
{
    std::println("main: {} ", std::this_thread::get_id());
    std::println("  &x: {:#010x} => {}", reinterpret_cast<intptr_t>(&x), x);

    test_thread_local_storage_01();

    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
