#!/bin/bash

set -e  # Terminate on errors

CFGNAME=$1
shift
FLAGS="$@"

FAILED=false
function fail() {
  echo "ERROR $FILE: $1"
#   echo "Log in $LOG"
  FAILED=true
}

function prepare() {
  FILE="$1"
  
  BASENAME="${FILE%.q}"
  STEM=${BASENAME##*/}

  DIRNAME="${BASENAME%/*}"
  OUTDIR=$DIRNAME/out
  
  OUTBASE=$OUTDIR/$STEM.$CFGNAME
  LOG="$OUTBASE.log"

}

function do_test() {
  prepare $1

  dos2unix $FILE 2>/dev/null  # if ther is problem because of different OS, use this command

  echo -n "Testing $1, $CFGNAME: "
  REGEXP="$(grep '^ *# EXPECT ' $FILE | sed -re 's/^ *# EXPECT //')"

  if test ! "$REGEXP"; then fail "No # EXPECT in $FILE"; return; fi

  ./llvmir $FLAGS "$FILE">"$LOG" 2>&1 && fail "Compilation did succeed unexpectedly"

  grep -q "$REGEXP" "$LOG" || fail "unexpected exception"

  echo "Success!"
}

for i in tests/invalidTest/typeConvert/*.q; do do_test "$i"; done
for i in tests/invalidTest/other/*.q; do do_test "$i"; done

