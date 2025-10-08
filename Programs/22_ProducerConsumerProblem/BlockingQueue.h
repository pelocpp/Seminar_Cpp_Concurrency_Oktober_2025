// ===========================================================================
// BlockingQueue.h
// ===========================================================================

#pragma once

#include "../Logger/Logger.h"

#include <array>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

namespace ProducerConsumerQueue
{
    template<typename T, size_t QueueSize = 10>
    class BlockingQueue
    {
    private:
        std::array<T, QueueSize> m_data;  // array - considered as a queue

        size_t m_pushIndex;      // muss da noch darüber nachdenken ...
        size_t m_popIndex;
        size_t m_size;

        std::mutex mutable m_mutex;

        // Monitor Concept (Dijkstra)
        std::condition_variable m_conditionIsEmpty;
        std::condition_variable m_conditionIsFull;

    public:
        // default c'tor
        BlockingQueue() :
            m_data{}, m_pushIndex {}, m_popIndex{}, m_size{}
        {
            Logger::log(std::cout, "Using Blocking Queue with Condition Variables");
        }

        // don't need other constructors or assignment operators
        BlockingQueue(const BlockingQueue&) = delete;
        BlockingQueue(BlockingQueue&&) = delete;

        BlockingQueue& operator= (const BlockingQueue&) = delete;
        BlockingQueue& operator= (BlockingQueue&&) = delete;

        // public interface
        void push(const T& item)
        {
            {
                std::unique_lock<std::mutex> guard{ m_mutex };

                // is stack full? (Note: lost and spurious wakeups)
                m_conditionIsFull.wait(
                    guard,
                    [this] () -> bool { return m_size < QueueSize; }
                );

                // guard
                if (m_size < QueueSize) {  // könnte man weglassen

                    m_data.at(m_pushIndex) = item;  // item kopiert

                    ++m_pushIndex;
                    m_pushIndex = m_pushIndex % QueueSize;

                    ++m_size;

                    Logger::log(std::cout, "    Size: ", m_size);
                }
            }

            // wakeup any sleeping consuments
            m_conditionIsEmpty.notify_all();
        }

        void push(T&& item)   // RValue Referenz
        {
            {
                std::unique_lock<std::mutex> guard{ m_mutex };

                // is stack full? (Note: lost and spurious wakeups)
                m_conditionIsFull.wait(
                    guard,
                    [this] () -> bool { return m_size < QueueSize; }
                    );

                // guard
                if (m_size < QueueSize) {

                    m_data.at(m_pushIndex) = std::move(item);   // hier wird verschoben

                    ++m_pushIndex;
                    m_pushIndex = m_pushIndex % QueueSize;

                    ++m_size;

                    Logger::log(std::cout, "    Size: ", m_size);
                }
            }

            // wakeup any sleeping consuments
            m_conditionIsEmpty.notify_all();
        }

        void pop(T& item)
        {
            {
                std::unique_lock<std::mutex> guard{ m_mutex };

                // is stack empty? (Note: lost and spurious wakeups)
                m_conditionIsEmpty.wait(
                    guard,
                    [this]() -> bool { return m_size != 0; }
                );

                // guard
                if (m_size != 0) {

                    item = m_data[m_popIndex]; // an item (Referenz) wird m_data[m_popIndex] KOPIERT

                    // Beobachtung: Objekt vom Typ T wurde rauskopiert
                    //              Das originale Objekt liegt aber noch in m_data

                    ++m_popIndex;
                    m_popIndex = m_popIndex % QueueSize;

                    --m_size;

                    Logger::log(std::cout, "    Size: ", m_size);
                }
            }

            // wakeup any sleeping producers
            m_conditionIsFull.notify_all();
        }

        bool empty() const
        {
            std::lock_guard<std::mutex> guard{ m_mutex };   // lock
            return m_size == 0;
        }

        size_t size() const
        {
            std::lock_guard<std::mutex> guard{ m_mutex };
            return m_size;
        }
    };
}

// ===========================================================================
// End-of-File
// ===========================================================================
