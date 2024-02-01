#if !defined(MEASUREMENT_TYPES_HPP_)
#define MEASUREMENT_TYPES_HPP_

#include <type_traits>

enum class MeasurementType
{
    // Undifened specified for debugging
    kMeasurementTypeUndefined = 0,
    // All specified measurement types declared in communication module
    kMeasurementTypeTemperature = 1,
    kMeasurementTypeVwc = 2,
    kMeasurementTypeEcBulk = 3,
    kMeasurementTypeEcPore = 4,
    // Count used for amount of sensor types
    kCount
};

class MeasurementTypeHelper
{
public:
    // Helper function for finding the amount of measurementTypes
    template <typename Enumeration>
    static constexpr auto as_integer(Enumeration const value)
        -> typename std::underlying_type<Enumeration>::type
    {
        return static_cast<typename std::underlying_type<Enumeration>::type>(value);
    }
};

#endif // MEASUREMENT_TYPES_HPP_
