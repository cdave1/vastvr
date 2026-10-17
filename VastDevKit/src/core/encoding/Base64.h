// Copyright (c) 2026 David Petrie
// SPDX-License-Identifier: MIT

#ifndef _VAST_BASE64_H_
#define _VAST_BASE64_H_

#include <string>

namespace hd {
namespace Encoding {
namespace Base64 {

// Encode arbitrary bytes using the standard Base64 alphabet and '=' padding.
std::string Encode(const std::string &bindata);

// Preserve the legacy decoder's permissive format: ignore ASCII whitespace and
// '=' anywhere, accept unpadded input, and emit complete bytes from a final
// partial group. Return an empty string if any other character is invalid.
std::string Decode(const std::string &ascdata);

}
}
}

#endif
