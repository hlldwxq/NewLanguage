#!/bin/bash

function fail() {
  echo "ERROR: $1"
  exit 1
}

function do_test() {
  FILE=$1
  LOG="$FILE.log"

  ./"$FILE" > ./"$LOG" 2>&1 || fail "$FILE: Failed"
}

function isSuccess(){
  FILE=$1
  REGEXP="Success!"

  grep -q "$REGEXP" "$FILE" || fail "$FILE: Has error, do not pass the test"

}

for i in tests/validTest/*.out; do do_test "$i"; done
for i in tests/validTest/*.log; do isSuccess "$i"; done

