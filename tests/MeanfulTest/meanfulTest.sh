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

for i in tests/MeanfulTest/*.out; do do_test "$i"; done


