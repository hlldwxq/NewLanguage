#!/bin/bash

function fail() {
  echo "ERROR ($FILE): $1"
  exit 1
}
function toUnix(){
  FILE=$1
  dos2unix $FILE
}
function do_test() {
  FILE=$1
  LOG="$FILE.log"
  SOURCE=${FILE/.out/.q};

  REGEXP="$(grep '^ *# EXPECT ' $SOURCE | sed -re 's/^ *# EXPECT //')"

  if test ! "$REGEXP"; then fail "No # EXPECT in $SOURCE"; return; fi

  ./"$FILE" >"$LOG" 2>&1

  grep -q "$REGEXP" "$LOG" || fail "unexpected exception"

}
for i in tests/dynamicCheck/*.q; do toUnix "$i"; done
for i in tests/dynamicCheck/*.out; do do_test "$i"; done
