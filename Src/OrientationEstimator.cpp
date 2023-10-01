#include "OrientationEstimator.h"

namespace mart {

OrientationEstimator::OrientationEstimator() {}

State OrientationEstimator::process(const State& state)
{
    return state;
}

Measurement OrientationEstimator::measurement(const State& state)
{
    return state;
}


}
