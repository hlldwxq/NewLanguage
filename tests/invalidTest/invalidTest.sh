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
  # dos2unix $FILE   # if ther is problem because of different OS, use this command

  REGEXP="$(grep '^ *# EXPECT ' $FILE | sed -re 's/^ *# EXPECT //')"

  if test ! "$REGEXP"; then fail "No # EXPECT in $FILE"; return; fi

  ./llvmir DyCheck "$FILE" >"$LOG" 2>&1 && fail "Compilation did succeed unexpectedly"

  grep -q "$REGEXP" "$LOG" || fail "unexpected exception"
}

for i in tests/invalidTest/typeConvert/*.q; do do_test "$i"; done
for i in tests/invalidTest/other/*.q; do do_test "$i"; done

if $FAILED; then exit 1; fi


