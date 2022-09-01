# Saturation

“Saturation” is a C++, dependency free, header-only library of functions that
perform (usually) branchless saturation arithmetic. It supports addition,
subtraction, multiplication, and division of signed and unsigned values anywhere
from 4 to 64 bits wide. That’s a total of 480 functions!

Functions are templated according the number of bits they use and named after
their operation (`add`, `sub`, `mul`, `div`) their sign (either `s` or `u` for
signed or unsigned respectively). For example:

    subs<24>(x, y)

This is a function that subtracts two signed 24 bit numbers in `x` and `y`
returning a 24 bit result.

## Why is this Useful?

Saturating arithmetic is useful in digital signal processing algorithms such as
a mixer which consumes two or more input signals and adds them to produce a
single output. See [this example mixer](mixer/index.html) for more information.