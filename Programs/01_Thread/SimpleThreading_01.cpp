// ===========================================================================
// Simple Threading Demo (std::thread basics) // SimpleThreading_01.cpp
// ===========================================================================

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG


#include <iostream>
#include <thread> 
#include <chrono>

#include "../Logger/Logger.h"


//std::thread copyT1;
//std::thread copyT2;

std::vector<std::thread> myThreads;


static void myFunction(int value) {

    std::thread::id tid{ std::this_thread::get_id() };
    Logger::log(std::cout, "tid:  ", tid);

    for (size_t i{}; i != 5; ++i) {
        Logger::log(std::cout, "in thread ", value);
        std::this_thread::sleep_for(std::chrono::seconds{ 1 });
    }

    Logger::log(std::cout, "Done Thread.");
}

namespace SimpleThreading01 {

    /*
     *  std::thread basics
     */

    constexpr size_t NumIterations{ 5 };

    // Signatur: Kann beliebig sein !!!!
    /*static*/ void function(int value) {

        std::thread::id tid{ std::this_thread::get_id() };
        Logger::log(std::cout, "tid:  ", tid);

        for (size_t i{}; i != NumIterations; ++i) {
            Logger::log(std::cout, "in thread ", value);
            std::this_thread::sleep_for(std::chrono::seconds{ 1 });
        }

        Logger::log(std::cout, "Done Thread.");
    }

    static void test_01() {

        Logger::log(std::cout, "Begin");

        std::thread::id mainTID{ std::this_thread::get_id() };
        Logger::log(std::cout, "main: ", mainTID);

        std::thread t1{ function, 1 };    // Fehlermeldungen: Reagierend  // (Concepts): Proaktives Verhalten
        std::thread t2{ function, 2 };

        //t1.detach();
        //t2.detach();

        //copyT1 = std::move(t1);
        //copyT2 = std::move(t2);

        myThreads.push_back(std::move(t1));
        myThreads.push_back(std::move(t2));

        Logger::log(std::cout, "Done.");
    
    }  // Determistisches: Destruktor std::thread

    static void test_02() {

        Logger::log(std::cout, "Begin test_02");

        std::thread::id mainTID{ std::this_thread::get_id() };
        Logger::log(std::cout, "main: ", mainTID);

        std::thread t1( function, 1 );  // nur ein Objekt anlegen  // start: No

        std::thread t2{ function, 2 };

        //t1.detach();
        //t2.detach();

        Logger::log(std::cout, "Done.");

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(6s);

        Logger::log(std::cout, "Done Again.");
    }
}

void test_simple_threading_01()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    using namespace SimpleThreading01;
   // test_01();
    test_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
