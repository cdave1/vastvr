// Copyright (c) 2026 David Petrie
// SPDX-License-Identifier: MIT

#include "core/encoding/Base64.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>

static void require(bool condition, const char *message) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", message);
        std::exit(1);
    }
}

int main(int argc, char **argv) {
    using hd::Encoding::Base64::Encode;
    using hd::Encoding::Base64::Decode;

    // Binary stdin/stdout lets the runner compare against Python's codec.
    if (argc == 2) {
        const std::string input((std::istreambuf_iterator<char>(std::cin)),
                                std::istreambuf_iterator<char>());
        const std::string mode(argv[1]);
        if (mode != "encode" && mode != "decode") return 2;
        const std::string output = mode == "encode" ? Encode(input) : Decode(input);
        std::cout.write(output.data(), static_cast<std::streamsize>(output.size()));
        return std::cout ? 0 : 1;
    }
    require(argc == 1, "usage: base64-tests [encode|decode]");

    // RFC 4648 section 10 interoperability vectors.
    const char *plain[] = {"", "f", "fo", "foo", "foob", "fooba", "foobar"};
    const char *encoded[] = {"", "Zg==", "Zm8=", "Zm9v", "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"};
    for (std::size_t i = 0; i < sizeof(plain) / sizeof(plain[0]); ++i) {
        require(Encode(plain[i]) == encoded[i], "encode RFC vector");
        require(Decode(encoded[i]) == plain[i], "decode RFC vector");
    }
    require(Encode(std::string("\0\xff\x80", 3)) == "AP+A", "encode binary bytes and alphabet endpoints");
    require(Decode("AP+A") == std::string("\0\xff\x80", 3), "decode embedded NUL and high bytes");
    require(Decode("////") == std::string(3, static_cast<char>(255)), "decode slash alphabet endpoint");
    require(Decode(" \tZ=m\r9=v\n\f\v=== ") == "foo", "preserve ignored whitespace and padding");
    require(Decode("Zg") == "f", "accept two unpadded symbols");
    require(Decode("Zm8") == "fo", "accept three unpadded symbols");
    require(Decode("Z") == "", "ignore a single incomplete symbol");
    require(Decode("Zm9vZ") == "foo", "retain complete bytes before a trailing symbol");
    require(Decode("Zm9v!") == "", "invalid suffix discards decoded bytes");
    require(Decode("Zg-_=") == "", "reject the URL-safe alphabet");
    require(Decode(std::string("Zg\0", 3)) == "", "reject NUL in encoded data");
    for (unsigned int value = 128; value <= 255; ++value) {
        require(Decode("Zm9v" + std::string(1, static_cast<char>(value))) == "",
                "reject non-ASCII input without signed-character indexing");
    }
    std::puts("Base64 vectors, binary data, legacy formatting, and invalid-input checks passed.");
}
