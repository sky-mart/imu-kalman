#ifndef ORIENTATIONESTIMATOR_H
#define ORIENTATIONESTIMATOR_H

#include "extkalman.h"

namespace mart
{

namespace orient
{

constexpr uint16_t VEC_SIZE = 3;

class OrientationEstimator
{
public:
    static constexpr uint16_t STATE_VECS_COUNT = 4;
    static constexpr uint16_t MEAS_VECS_COUNT = 3;
    using EKF = ExtendedKalmanFilter<float, STATE_VECS_COUNT * VEC_SIZE, MEAS_VECS_COUNT * VEC_SIZE>;

    OrientationEstimator();

private:
    EKF ekf_;
};

}

}  // mart


#endif /* ORIENTATIONESTIMATOR_H */
