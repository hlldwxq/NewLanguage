#!/bin/bash

function fail() {
  echo "ERROR: $1"
  exit 1
}

function do_test() {
  FILE=$1
  LOG="$FILE.log"

  ./"$FILE" > ./"$LOG" 2>&1 | true
  grep -q "overflow" "$LOG" || fail "$FILE: Not yielded expected output"

}

for i in tests/dynamicCheck/*.out; do do_test "$i"; done
