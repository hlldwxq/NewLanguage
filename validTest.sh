#!/bin/bash

if ./tests/validCodeTest | egrep -v ": 1$"; then
  echo "ERROR"
  exit 1
fi
