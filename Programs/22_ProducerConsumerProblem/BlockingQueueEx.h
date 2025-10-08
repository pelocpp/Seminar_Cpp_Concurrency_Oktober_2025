// ===========================================================================
// BlockingQueueEx.h
// ===========================================================================

#pragma once

#include "../Logger/Logger.h"

#include <chrono>
#include <iostream>
#include <mutex>
#include <semaphore>
#include <thread>

namespace ProducerConsumerQueue
{
    template<typename T, size_t QueueSize = 10>
    class BlockingQueue
    {
    private:
        T* m_data;   // Beispiel: User* m_data:     m_data + 1 == m_data + sizeof (User)

        size_t m_size;
        size_t m_pushIndex;
        size_t m_popIndex;

        std::counting_semaphore<QueueSize> m_emptySlots;  // maximaler Wert: 10
        std::counting_semaphore<QueueSize> m_fullSlots;   // maximaler Wert: 10

        std::mutex mutable m_mutex;

    public:
        // default c'tor
        BlockingQueue() :
            m_size{},
            m_pushIndex{},
            m_popIndex{},
            m_emptySlots{ QueueSize },   // Laufzeit: Vorbelegung mit 10: Es ist leerer Platz für 10 Elemente
            m_fullSlots{ 0 },            // Laufzeit: Vorbelegung mit 0: Es ist kein Element vorhanden
            m_data{ static_cast<T*>(std::malloc(sizeof(T) * QueueSize)) }
        {
            Logger::log(std::cout, "Using Blocking Queue with Semaphores");
        }

        // don't need other constructors or assignment operators
        BlockingQueue(const BlockingQueue&) = delete;
        BlockingQueue(BlockingQueue&&) = delete;

        BlockingQueue& operator= (const BlockingQueue&) = delete;
        BlockingQueue& operator= (BlockingQueue&&) = delete;

        // destructor
        ~BlockingQueue()
        {
            size_t n{ 0 };
            while (n != m_size)
            {
                m_data[m_popIndex].~T();

                ++m_popIndex;
                m_popIndex = m_popIndex % QueueSize;

                ++n;
            }

            std::free(m_data);
        }

        // public interface
        void push(const T& item)
        {
            m_emptySlots.acquire();  // 10 -> 9 (keine Blockade) -> 8 (keine Blockade) 
            
            {
                std::lock_guard<std::mutex> guard{ m_mutex };

                // Zeigerarithmetik: 
                new (m_data + m_pushIndex) T{ item };  // Placement new Technik

                ++m_pushIndex;
                m_pushIndex = m_pushIndex % QueueSize;

                ++m_size;

                Logger::log(std::cout, "    Size: ", m_size);
            }
            
            m_fullSlots.release();   // 0 -> 1 (keine Blockade)
        }




        void push(T&& item)
        {
            m_emptySlots.acquire();
            {
                std::lock_guard<std::mutex> guard{ m_mutex };

                new (m_data + m_pushIndex) T{ std::move(item) };

                ++m_pushIndex;
                m_pushIndex = m_pushIndex % QueueSize;

                ++m_size;

                Logger::log(std::cout, "    Size: ", m_size);
            }
            m_fullSlots.release();
        }

        void pop(T& item)
        {
            m_fullSlots.acquire();  // 0 : Blockade, weil keine Element
                                    // groesser 0: Keine Blockade
            
            {
                // RAII
                std::lock_guard<std::mutex> guard{ m_mutex };

                item = m_data[m_popIndex];  // ein item (Referenz) wird m_data[m_popIndex] RAUSKOPIERT
                
                // IM internen Speicherbereich wird das Objekt freigegeben !!!
                m_data[m_popIndex].~T();

                ++m_popIndex;
                m_popIndex = m_popIndex % QueueSize;

                --m_size;

                Logger::log(std::cout, "    Size: ", m_size);
            }

            m_emptySlots.release();
        }

        bool empty() const
        {
            std::lock_guard<std::mutex> guard{ m_mutex };
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
