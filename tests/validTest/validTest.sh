#!/bin/bash

if ./tests/validTest/validCodeTest | egrep -v ": 1$"; then
  echo "ERROR"
  exit 1
fi
