#!/usr/bin/env bash

set -u

if [ "$#" -ne 3 ]; then
    echo "ERROR: expected 3 arguments" >&2
    exit 2
fi

PROGRAM="$1"
INPUT="$2"
EXPECTED="$3"

ACTUAL="$(mktemp)" || exit 1
ERRORS="$(mktemp)" || exit 1

trap 'rm -f "$ACTUAL" "$ERRORS"' EXIT

timeout 15s "$PROGRAM" "$INPUT" \
    > "$ACTUAL" 2> "$ERRORS"

RUN_RC=$?

if [ "$RUN_RC" -ne 0 ]; then
    echo "RUN_RC=$RUN_RC" >&2
    cat "$ERRORS" >&2
    exit 1
fi

if ! cmp -s "$EXPECTED" "$ACTUAL"; then
    echo "OUTPUT_CHECK=FAIL" >&2
    diff -u "$EXPECTED" "$ACTUAL" >&2 || true
    exit 1
fi

if [ -s "$ERRORS" ]; then
    echo "STDERR_CHECK=FAIL" >&2
    cat "$ERRORS" >&2
    exit 1
fi

echo "OUTPUT_CHECK=PASS"
exit 0
