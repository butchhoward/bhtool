#include <modern/lib.hpp>

#include <tuple>
#include <vector>
#include <algorithm>


std::tuple<double, double> first_two(const std::vector<double>& values) {

    return {values[0], values[1]};
}
