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
  dos2unix $FILE 2>/dev/null  # if ther is problem because of different OS, use this command

  echo -n "Testing $1, $CFGNAME: "
  REGEXP="$(grep '^ *# EXPECT ' $FILE | sed -re 's/^ *# EXPECT //')"

  if test ! "$REGEXP"; then fail "No # EXPECT in $FILE"; return; fi

  ./llvmir "$FLAGS" "$FILE">"$LOG" 2>&1 && fail "Compilation did succeed unexpectedly"

  grep -q "$REGEXP" "$LOG" || fail "unexpected exception"

  echo "Success!"
}


function do_tests() {
  FLAGS=$1
  CFGNAME=$2

  for i in tests/invalidTest/typeConvert/*.q; do do_test "$i"; done
  for i in tests/invalidTest/other/*.q; do do_test "$i"; done
  
  if $FAILED; then exit 1; fi
}

do_tests DyCheck dy
do_tests notDyCheck ndy