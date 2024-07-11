#pragma once

#include <boost/asio.hpp>

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace tuposoft {
    constexpr auto BYTE_SIZE = static_cast<unsigned char>(0x08U);
    constexpr auto FULL_BYTE = static_cast<unsigned char>(0xFFU);
    constexpr auto UPPER_TWO_BITS_MASK = static_cast<unsigned char>(0xC0U);
    constexpr auto LOWER_SIX_BITS_MASK = static_cast<unsigned char>(0x3FU);

    auto to_dns_label_format(const std::string &domain) -> std::vector<std::uint8_t>;

    auto from_dns_label_format(std::istream &input) -> std::string;

    template<typename T = unsigned short>
    auto read_big_endian(std::istream &input) -> T {
        T result{};
        for (int i = 0; i < sizeof(T); ++i) {
            result <<= BYTE_SIZE;
            result |= static_cast<T>(input.get()) & FULL_BYTE;
        }
        return result;
    }

    template<typename T = unsigned short>
    auto write_big_endian(std::ostream &output, T val) -> decltype(output) {
        std::array<char, sizeof(val)> buffer{};
        auto val_be = htons(val);
        std::memcpy(buffer.data(), &val_be, sizeof(val));
        output.write(buffer.data(), buffer.size());
        return output;
    }

    enum struct message_byte_offsets : std::uint8_t {
        HEADER = 0,
        ANCOUNT = 6,
        QUESTION = 12,
        ANSWERS = 30,
    };
} // namespace tuposoft
