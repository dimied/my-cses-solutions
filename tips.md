### g++, gcc
- __builtin_clz(x): the number of zeros at the beginning of the bit representation
- __builtin_ctz(x): the number of zeros at the end of the bit representation
- __builtin_popcount(x): the number of ones in the bit representation
- __builtin_parity(x): the parity (even or odd) of the number of ones in the bit representation
- Note that the above functions only support int numbers, but there are also long long versions of the functions available with the suffix ll.