#!/bin/bash

set -e  # Terminate on errors


# parallel --verbose ./__run_test.sh\
#       dy ::: DyCheck\
#       ndy ::: notDyCheck\
#       arith ::: check_arith\
#       array ::: check_array_bound\
#       free ::: check_free\
#       arith_array ::: check_arith ::: check_array_bound\
#       free_array ::: check_free ::: check_array_bound\
#       free_arith ::: check_free ::: check_arith\
#       arith_array_free ::: check_arith ::: check_free ::: check_array_bound
#



./tests/validTest/__run_test.sh dy DyCheck &
./tests/validTest/__run_test.sh ndy notDyCheck &

./tests/validTest/__run_test.sh arith check_arith &
./tests/validTest/__run_test.sh array check_array_bound &
./tests/validTest/__run_test.sh free check_free &

./tests/validTest/__run_test.sh arith_array check_arith check_array_bound &
./tests/validTest/__run_test.sh free_array check_free	check_array_bound &
./tests/validTest/__run_test.sh free_arith check_free	check_arith &

./tests/validTest/__run_test.sh arith_array_free check_arith check_free	check_array_bound &

wait

