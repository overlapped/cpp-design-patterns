#!/bin/bash

#!/bin/bash
set -euo pipefail

PRESET="${1:-release}"

cmake --preset "$PRESET"
cmake --build --preset "$PRESET" --parallel "$(nproc)"

# тесты есть только у пресетов с testPreset
if ctest --list-presets 2>/dev/null | grep -q "\"$PRESET\""; then
    ctest --preset "$PRESET"
fi
