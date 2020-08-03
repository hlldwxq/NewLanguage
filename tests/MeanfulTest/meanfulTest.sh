#!/bin/bash

set -e  # Terminate on errors

./tests/MeanfulTest/__run_test.sh dy DyCheck &
./tests/MeanfulTest/__run_test.sh ndy notDyCheck &

./tests/MeanfulTest/__run_test.sh arith check_arith &
./tests/MeanfulTest/__run_test.sh array check_array_bound &
./tests/MeanfulTest/__run_test.sh free check_free &

./tests/MeanfulTest/__run_test.sh arith_array check_arith check_array_bound &
./tests/MeanfulTest/__run_test.sh free_array check_free	check_array_bound &
./tests/MeanfulTest/__run_test.sh free_arith check_free	check_arith &

./tests/MeanfulTest/__run_test.sh arith_array_free check_arith check_free	check_array_bound &

wait

