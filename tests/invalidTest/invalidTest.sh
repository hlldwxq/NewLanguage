#!/bin/bash
FAILED=false
function fail() {
  echo "###ERROR $FILE: $1"
#   echo "Log in $LOG"
  FAILED=true
}

function do_test() {
  FILE=$1
  LOG="$FILE.log"


  REGEXP="$(grep '^ *# EXPECT Error' $FILE | sed -re 's/^ *# EXPECT //')"

  if test ! "$REGEXP"; then fail "No #EXPECT in $FILE"; return; fi

  ./llvmir DyCheck "$FILE" >"$LOG" 2>&1 | true # Silence the segfault

  grep -q "$REGEXP" "$LOG" || fail "Not yielded expected output"
}

for i in tests/invalidTest/test_*.q; do do_test "$i"; done

if $FAILED; then exit 1; fi


