/**
 * @file spi.h
 * @author FernandezKA (fernandes.kir@yandex.ru)
 * @brief  spi class over POSIX API
 * @version 0.1
 * @date 2025-09-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
 #pragma once

#include <cstdint>
#include <string>
#include <span>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

class SPI {
public:
    // Constructor/destructor
    SPI(const std::string& device, uint32_t speed, uint8_t mode, uint8_t bits);
    ~SPI();

    // Delete copy constructor and assignment
    SPI(const SPI&) = delete;
    SPI& operator=(const SPI&) = delete;

    // Move constructor and assignment
    SPI(SPI&&) noexcept = default;
    SPI& operator=(SPI&&) noexcept = default;

    // Basic SPI operations using std::span (C++23)
    bool write(std::span<const uint8_t> data);
    bool read(std::span<uint8_t> data); 
    bool transfer(std::span<const uint8_t> tx_data, std::span<uint8_t> rx_data);

    // Configuration methods
    [[nodiscard]] bool setBitsPerWord(uint8_t bits);
    [[nodiscard]] bool setSpeed(uint32_t speed);
    [[nodiscard]] bool setMode(uint8_t mode);

    // Status check
    [[nodiscard]] bool isOpen() const;

private:
    int fd_;                                   // File descriptor
    std::string device_;                       // Device path
    uint32_t speed_;                           // SPI clock speed
    uint8_t mode_;                             // SPI mode
    uint8_t bits_per_word_;                    // Bits per word
    bool is_open_;                             // Device open status

    // Helper method for initialization
    [[nodiscard]] bool init();
};

