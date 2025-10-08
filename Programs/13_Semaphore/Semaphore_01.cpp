// ===========================================================================
// Semaphore // Semaphore_01.cpp
// ===========================================================================

#include "../Logger/Logger.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <semaphore>
#include <print>

namespace ConcurrencyBinarySemaphore {

    class Worker
    {
    private:
        std::binary_semaphore m_semaphore;

    public:
        Worker() : m_semaphore{ 0 } {}   // 0 == Nicht verfügbar

        void scheduleJob()
        {
            Logger::log(std::cout, "ScheduleJob: Data preparing ...");

            std::this_thread::sleep_for(std::chrono::seconds{ 8 });

            Logger::log(std::cout, "ScheduleJob: Data prepared!");

            m_semaphore.release(); // um 1 erhöhen: Bei uns von 0 auf 1
        }

        void executeJob()
        {
            Logger::log(std::cout, "ExecuteJob:  Waiting for data ...");

            m_semaphore.acquire();  // Wenn 0: Blockade // wird aufgelöst, wenn ungleich 0

            Logger::log(std::cout, "ExecuteJob:  Executing job ...");

            std::this_thread::sleep_for(std::chrono::seconds{ 3 });

            Logger::log(std::cout, "ExecuteJob:  Done.");
        }
    };
}

void test_binary_semaphore_01() {

    using namespace ConcurrencyBinarySemaphore;

    Worker worker{};

    std::jthread t1{ &Worker::scheduleJob, &worker };  // Job vorbereiten

    std::jthread t2{ &Worker::executeJob, &worker };   // Job ausführen

}   // Destruktor: join

// ===========================================================================
// End-of-File
// ===========================================================================
