#!/usr/bin/env bash
#
# DevContainer 環境の想定値を検証するスクリプト。
# ローカル（Rebuild Container 後）でも GitHub Actions（devcontainers/ci）でも
# 同じものを実行し、検証ロジックを一箇所に集約する。
#
# 使い方:
#   bash scripts/verify-env.sh
#
set -o pipefail

# ------------------------------------------------------------
# 期待値（Dockerfile / CMakeLists.txt と揃えること）
# ------------------------------------------------------------
EXPECTED_GCC_MAJOR="13"
EXPECTED_CMAKE="3.28"
EXPECTED_NINJA="1.11"
EXPECTED_GTEST="v1.18.0"
EXPECTED_LANG="ja_JP.UTF-8"
EXPECTED_TZ="Asia/Tokyo"

fail=0

# assert LABEL EXPECTED_SUBSTRING ACTUAL
assert() {
  local label="$1" expected="$2" actual="$3"
  if printf '%s' "$actual" | grep -qF "$expected"; then
    printf '  \033[32m✅ %-16s\033[0m %s\n' "$label" "$actual"
  else
    printf '  \033[31m❌ %-16s\033[0m expected to contain "%s" but got: %s\n' "$label" "$expected" "$actual"
    fail=1
  fi
}

echo "=== 1. Compiler ==="
assert "g++ major" ") ${EXPECTED_GCC_MAJOR}." "$(g++ --version | head -1)"

echo "=== 2. CMake ==="
assert "cmake" "version ${EXPECTED_CMAKE}" "$(cmake --version | head -1)"

echo "=== 3. Ninja ==="
assert "ninja" "${EXPECTED_NINJA}" "$(ninja --version)"

echo "=== 4. GoogleTest pin ==="
# 実際に取得されるのはコミットハッシュなので、ピン留めのコメントを静的に検証する。
assert "gtest pin" "${EXPECTED_GTEST}" "$(grep -E '^\s+GIT_TAG' CMakeLists.txt)"

echo "=== 5. Locale ==="
assert "LANG" "$EXPECTED_LANG" "${LANG:-unset}"

echo "=== 6. Timezone ==="
assert "TZ" "$EXPECTED_TZ" "${TZ:-unset}"

echo "=== 7. Build & Test ==="
if cmake -S . -B build > /tmp/cmake-build.log 2>&1 \
  && cmake --build build >> /tmp/cmake-build.log 2>&1 \
  && ctest --test-dir build --output-on-failure >> /tmp/cmake-build.log 2>&1; then
  printf '  \033[32m✅ %-16s\033[0m cmake configure / build / ctest\n' "BUILD"
else
  printf '  \033[31m❌ %-16s\033[0m cmake configure / build / ctest failed\n' "BUILD"
  tail -30 /tmp/cmake-build.log
  fail=1
fi

echo
if [ "$fail" -eq 0 ]; then
  echo "🎉 すべての検証項目に合格しました。"
else
  echo "💥 検証に失敗した項目があります（上記 ❌ を確認してください）。"
fi
exit "$fail"
