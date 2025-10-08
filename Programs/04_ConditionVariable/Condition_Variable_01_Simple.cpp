// ===========================================================================
// Condition_Variable_01_Simple.cpp // Condition Variable
// ===========================================================================

#include <iostream>
#include <thread> 
#include <mutex> 
#include <chrono>

#include "../Logger/Logger.h"

namespace SimpleConditionVariableDemo01
{
    std::mutex mutex{};

    std::condition_variable condition{};

    bool data{ false };   // Diese sind vorhanden // oder auch nicht: true // false

    static void consume()
    {
        Logger::log(std::cout, "Function Consume:");

        bool copyData{ false };

        {
            // RAII
            // ein std::lock_guard tut es hier nicht :)
            std::unique_lock<std::mutex> guard{ mutex };

            // Frage: Gibt es jetzt Daten oder nicht ..
            //while (data == false) {
            //    ;  // busy polling  // Noooooooooooooooo
            //}

            condition.wait(
                guard,
                // Frage an den Lambda: Gibt es jetzt Daten oder nicht ..
                [] () -> bool {
                    Logger::log(std::cout, "  ... check for data being present: ", data == true);
                    return data == true;  // keine Daten: false oder true: Es gibt jetzt Daten
                }
            );

            // Hier kann man nun GESCHÜTZT mit den Daten arbeiten:
            // copyData: Am Stack : Ist automatisch geschützt
            // data: Global: Ist nicht geschützt, deshalb Lesen innerhalb der Sperre

            copyData = data;
        }

        Logger::log(std::cout, "Data has been consumed ...", copyData);

        Logger::log(std::cout, "Done Thread ");
    }

    static void produce()
    {
        Logger::log(std::cout, "Function Produce:");

        // put into comment ... or not
       // std::this_thread::sleep_for(std::chrono::milliseconds{ 5000 });

        {
            std::lock_guard<std::mutex> guard{ mutex };

            data = true;   // Hmmm, es sind Daten da // true
        }

        Logger::log(std::cout, "Data has been produced ...");

        // Keine Sperre notwendig !!!!!!!!!!
        condition.notify_one();    // Signal  // Ein Ping an die Condition Queue

        Logger::log(std::cout, "Done Thread ");
    }

    static void test()
    {
        Logger::log(std::cout, "Function Main:");

        std::thread t1{ produce };
        std::thread t2{ consume };

        t1.join();
        t2.join();

        Logger::log(std::cout, "Done Main.");
    }
}

void test_simple_condition_variable_01()
{
    using namespace SimpleConditionVariableDemo01;
    test();
}

// ===========================================================================
// End-of-File
// ===========================================================================
