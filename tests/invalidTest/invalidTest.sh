#!/bin/bash

set -e  # Terminate on errors

./tests/invalidTest/__run_test.sh dy DyCheck &
./tests/invalidTest/__run_test.sh ndy notDyCheck &

./tests/invalidTest/__run_test.sh arith check_arith &
./tests/invalidTest/__run_test.sh array check_array_bound &
./tests/invalidTest/__run_test.sh free check_free &

./tests/invalidTest/__run_test.sh arith_array check_arith check_array_bound &
./tests/invalidTest/__run_test.sh free_array check_free	check_array_bound &
./tests/invalidTest/__run_test.sh free_arith check_free	check_arith &

./tests/invalidTest/__run_test.sh arith_array_free check_arith check_free	check_array_bound &

wait

