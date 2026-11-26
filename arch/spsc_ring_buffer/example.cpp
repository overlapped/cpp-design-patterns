#include "spsc_ring_buffer.hpp"

#include <cstdint>
#include <iostream>
#include <thread>

struct SensorPacket {
    std::uint32_t sequence_id{0};
    double value{0.0};
};

int main() {
    // Буфер на 8 слотов (полезная ёмкость — 7 элементов)
    arch::SpscRingBuffer<SensorPacket, 8> ring_buffer;
    constexpr std::uint32_t kTotalPackets = 15;

    // Поток-производитель (например, поток чтения данных с АЦП или сокета)
    std::thread producer([&ring_buffer]() {
        for (std::uint32_t i = 1; i <= kTotalPackets; ++i) {
            SensorPacket pkt{i, static_cast<double>(i) * 1.5};

            // Spin-wait при временном заполнении буфера
            while (!ring_buffer.try_push(pkt)) {
                std::this_thread::yield();
            }
            std::cout << "[Producer] Sent packet #" << pkt.sequence_id << '\n';
        }
    });

    // Поток-потребитель (обработчик данных)
    std::thread consumer([&ring_buffer]() {
        std::uint32_t received = 0;
        while (received < kTotalPackets) {
            if (auto pkt = ring_buffer.try_pop()) {
                std::cout << "  [Consumer] Received packet #" << pkt->sequence_id
                          << " -> value=" << pkt->value << '\n';
                ++received;
            } else {
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    consumer.join();

    std::cout << "All " << kTotalPackets << " packets transferred lock-free.\n";
    return 0;
}
