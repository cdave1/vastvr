// Copyright (c) 2026 David Petrie
// SPDX-License-Identifier: MIT
// Original implementation of the byte grouping described in RFC 4648 section 4.

#include "Base64.h"

namespace {

char encodeDigit(unsigned int value) {
    if (value < 26) return static_cast<char>('A' + value);
    if (value < 52) return static_cast<char>('a' + value - 26);
    if (value < 62) return static_cast<char>('0' + value - 52);
    return value == 62 ? '+' : '/';
}

int decodeDigit(unsigned char character) {
    if (character >= 'A' && character <= 'Z') return character - 'A';
    if (character >= 'a' && character <= 'z') return character - 'a' + 26;
    if (character >= '0' && character <= '9') return character - '0' + 52;
    if (character == '+') return 62;
    if (character == '/') return 63;
    return -1;
}

bool isFormatting(unsigned char character) {
    switch (character) {
        case '=': case ' ': case '\t': case '\n': case '\r': case '\f': case '\v':
            return true;
        default:
            return false;
    }
}

void appendBytes(std::string &output, const unsigned int *digits, unsigned int count) {
    if (count >= 2) output.push_back(static_cast<char>((digits[0] << 2) | (digits[1] >> 4)));
    if (count >= 3) output.push_back(static_cast<char>(((digits[1] & 15) << 4) | (digits[2] >> 2)));
    if (count == 4) output.push_back(static_cast<char>(((digits[2] & 3) << 6) | digits[3]));
}

}

std::string hd::Encoding::Base64::Encode(const std::string &bindata) {
    std::string output;
    const std::size_t groups = bindata.size() / 3 + (bindata.size() % 3 != 0);
    if (groups > output.max_size() / 4) return output;
    output.reserve(groups * 4);

    std::size_t offset = 0;
    while (offset < bindata.size()) {
        const std::size_t remaining = bindata.size() - offset;
        const unsigned int first = static_cast<unsigned char>(bindata[offset]);
        const unsigned int second = remaining > 1 ? static_cast<unsigned char>(bindata[offset + 1]) : 0;
        const unsigned int third = remaining > 2 ? static_cast<unsigned char>(bindata[offset + 2]) : 0;

        output.push_back(encodeDigit(first >> 2));
        output.push_back(encodeDigit(((first & 3) << 4) | (second >> 4)));
        output.push_back(remaining > 1 ? encodeDigit(((second & 15) << 2) | (third >> 6)) : '=');
        output.push_back(remaining > 2 ? encodeDigit(third & 63) : '=');
        offset += remaining < 3 ? remaining : 3;
    }
    return output;
}

std::string hd::Encoding::Base64::Decode(const std::string &ascdata) {
    std::string output;
    unsigned int digits[4] = {};
    unsigned int count = 0;

    for (unsigned char character : ascdata) {
        if (isFormatting(character)) continue;
        const int value = decodeDigit(character);
        if (value < 0) return std::string();
        digits[count++] = static_cast<unsigned int>(value);
        if (count == 4) {
            appendBytes(output, digits, count);
            count = 0;
        }
    }
    appendBytes(output, digits, count);
    return output;
}
