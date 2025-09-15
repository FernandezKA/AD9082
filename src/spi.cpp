#include "../inc/spi.h"

SPI::SPI(const std::string& device, uint32_t speed, uint8_t mode, uint8_t bits)
    : fd_(-1), device_(device), speed_(speed), mode_(mode), bits_per_word_(bits), is_open_(false) {
    init();
}

SPI::~SPI() {
    if (is_open_) {
        close(fd_);
    }
}

bool SPI::init() {
    fd_ = open(device_.c_str(), O_RDWR);
    if (fd_ < 0) return false;

    if (ioctl(fd_, SPI_IOC_WR_MODE, &mode_) < 0 ||
        ioctl(fd_, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word_) < 0 ||
        ioctl(fd_, SPI_IOC_WR_MAX_SPEED_HZ, &speed_) < 0) {
        close(fd_);
        return false;
    }

    is_open_ = true;
    return true;
}

bool SPI::write(std::span<const uint8_t> data) {
    if (!is_open_) return false;
    return ::write(fd_, data.data(), data.size()) == static_cast<ssize_t>(data.size());
}

bool SPI::read(std::span<uint8_t> data) {
    if (!is_open_) return false;
    return ::read(fd_, data.data(), data.size()) == static_cast<ssize_t>(data.size());
}

bool SPI::transfer(std::span<const uint8_t> tx_data, std::span<uint8_t> rx_data) {
    if (!is_open_ || tx_data.size() != rx_data.size()) return false;

    spi_ioc_transfer tr = {};
    tr.tx_buf = reinterpret_cast<uintptr_t>(tx_data.data());
    tr.rx_buf = reinterpret_cast<uintptr_t>(rx_data.data());
    tr.len = tx_data.size();
    tr.speed_hz = speed_;
    tr.bits_per_word = bits_per_word_;

    return ioctl(fd_, SPI_IOC_MESSAGE(1), &tr) >= 0;
}

bool SPI::setBitsPerWord(uint8_t bits) {
    if (!is_open_) return false;
    if (ioctl(fd_, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) return false;
    bits_per_word_ = bits;
    return true;
}

bool SPI::setSpeed(uint32_t speed) {
    if (!is_open_) return false;
    if (ioctl(fd_, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) return false;
    speed_ = speed;
    return true;
}

bool SPI::setMode(uint8_t mode) {
    if (!is_open_) return false;
    if (ioctl(fd_, SPI_IOC_WR_MODE, &mode) < 0) return false;
    mode_ = mode;
    return true;
}

bool SPI::isOpen() const {
    return is_open_;
}