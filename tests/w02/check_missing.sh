#!/usr/bin/env bash

set -u

if [ "$#" -ne 2 ]; then
    echo "ERROR: expected 2 arguments" >&2
    exit 2
fi

PROGRAM="$1"
MISSING="$2"

if [ -e "$MISSING" ] || [ -L "$MISSING" ]; then
    echo "TEST_BLOCKED: path exists" >&2
    exit 1
fi

OUT="$(mktemp)" || exit 1

trap 'rm -f "$OUT"' EXIT

timeout 15s "$PROGRAM" "$MISSING" > "$OUT"

PROGRAM_RC=$?

STDOUT_BYTES="$(wc -c < "$OUT")"

echo "PROGRAM_RC=$PROGRAM_RC"
echo "STDOUT_BYTES=$STDOUT_BYTES"

# TODO A:
# 检查 PROGRAM_RC 是否等于预期错误码 3。
# 如果不等于 3，测试失败，返回 1。
if [ "$PROGRAM_RC" -ne 3 ]; then
    # TODO：输出错误并返回 1
    echo "PROGRAM_RC != 3"
exit 1
fi

# TODO B:
# 检查 STDOUT_BYTES 是否等于 0。
# 如果不是 0，测试失败，返回 1。
if [ "$STDOUT_BYTES" -ne 0 ]; then
    # TODO：输出错误并返回 1
    echo "STDOUT_BYTES != 0"
    exit 1
fi


# TODO C:
# 两项检查全部通过后，输出 MISSING_CHECK=PASS。
# 返回成功状态码 0。
echo "MISSING_CHECK=PASS"
exit 0

# 临时状态：测试断言尚未完成
exit 99
