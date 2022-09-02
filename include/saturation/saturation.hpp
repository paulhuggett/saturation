/// \file saturation.hpp
/// \brief A collection of functions which provide saturating arithmetic
/// (addition, subtraction, multiplication, and division) for signed and
/// unsigned integer types.
///
/// Types can range from 4 to 64 bits. Operations using integers of width
/// matching target registers are likely to be branchless.

#ifndef SATURATION_HPP
#define SATURATION_HPP

#include "saturation/add.hpp"
#include "saturation/div.hpp"
#include "saturation/mul.hpp"
#include "saturation/sub.hpp"

#endif  // SATURATION_HPP
