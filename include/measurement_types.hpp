#if !defined(MEASUREMENT_TYPES_HPP_)
#define MEASUREMENT_TYPES_HPP_

// #include <type_traits>
#include <Arduino.h>

class MeasurementTypeHelper
{
public:
    // Helper function for finding the amount of measurementTypes
    template <typename Enumeration>
    static constexpr auto as_integer(Enumeration const value) -> int
    // -> typename std::underlying_type<Enumeration>::type
    {
        return static_cast<int>(value);
    }
};

#endif // MEASUREMENT_TYPES_HPP_
