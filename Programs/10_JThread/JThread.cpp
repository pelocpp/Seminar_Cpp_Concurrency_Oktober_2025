// ===========================================================================
// JThread_01.cpp // class JThread 
// ===========================================================================

#include "../Logger/Logger.h"

#include <iostream>
#include <thread>
#include <format>
#include <chrono>

namespace JoinableThread {

    static void jthread_01()
    {
        std::thread t {
            [] () { Logger::log(std::cout, "Inside std::thread"); }
        };

        bool joinable{ t.joinable() };

        Logger::log(std::cout, "std::thread: joinable=", joinable);

        t.join();  // <== put into comments -- must not be missing
    }

    static void jthread_02()
    {
        std::jthread t {

            [] () { 
                Logger::log(std::cout, "Inside std::jthread"); 
                std::this_thread::sleep_for(std::chrono::seconds{ 5 });
            }
        };

        bool joinable{ t.joinable() };

        Logger::log(std::cout, "std::jthread: joinable=", joinable);

        //t.join();  // <== put into comments -- may be missing
    }  // Hier wird join im Destruktor aufgerufen ...
}

// =================================================================

void test_jthread()
{
    using namespace JoinableThread;

    jthread_01();
    jthread_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
