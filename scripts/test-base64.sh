#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
compiler="${CXX:-c++}"
sanitizers="${BASE64_SANITIZERS:-undefined}"
mkdir -p "$repo_root/.build"
binary="$repo_root/.build/base64-tests"

"$compiler" -std=c++11 -Wall -Wextra -Werror \
  "-fsanitize=$sanitizers" -fno-sanitize-recover=all -fno-omit-frame-pointer \
  -I "$repo_root/VastDevKit/src" \
  "$repo_root/tests/base64.cpp" "$repo_root/VastDevKit/src/core/encoding/Base64.cpp" \
  -o "$binary"
"$binary"

python3 - "$binary" "$sanitizers" <<'PY'
import base64
import random
import subprocess
import sys

binary = sys.argv[1]
random_source = random.Random(4648)
lengths = list(range(65)) + [127, 128, 255, 256, 257, 1023, 1024, 4095, 4096, 4097, 65536]
payloads = [bytes(range(256))]
payloads += [bytes(random_source.randrange(256) for _ in range(length)) for length in lengths]
for payload in payloads:
    expected = base64.b64encode(payload)
    actual = subprocess.check_output([binary, 'encode'], input=payload)
    if actual != expected:
        raise SystemExit('Encoding differs from Python for length ' + str(len(payload)))
    decoded = subprocess.check_output([binary, 'decode'], input=expected)
    if decoded != payload:
        raise SystemExit('Decoding differs from Python for length ' + str(len(payload)))
print('Base64 agrees with Python for', len(payloads), 'binary inputs; sanitizers:', sys.argv[2])
PY
