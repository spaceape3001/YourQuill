mathtest(fraction_empty)

mt_math(fraction_add)
add_test(NAME fraction_add_1_2_1_2 COMMAND test_fraction_add 1 2 1 2 1 1)
add_test(NAME fraction_add_1_2_1_4 COMMAND test_fraction_add 1 2 1 4 3 4)
add_test(NAME fraction_add_1_3_1_2 COMMAND test_fraction_add 1 3 1 2 5 6)

mt_math(fraction_divide)
add_test(NAME fraction_divide_1_2_1_2 COMMAND test_fraction_divide 1 2 1 2 1 1)
add_test(NAME fraction_divide_1_2_1_3 COMMAND test_fraction_divide 1 2 1 3 3 2)

mt_math(fraction_multiply)
add_test(NAME fraction_multiply_1_2_1_2 COMMAND test_fraction_multiply 1 2 1 2 1 4)
add_test(NAME fraction_multiply_1_2_1_3 COMMAND test_fraction_multiply 1 2 1 3 1 6)


mt_math(fraction_subtract)
add_test(NAME fraction_subtract_1_2_1_2 COMMAND test_fraction_subtract 1 2 1 2 0 1)
add_test(NAME fraction_subtract_1_4_1_2 COMMAND test_fraction_subtract 1 4 1 2 -1 4)

