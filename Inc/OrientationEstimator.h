#ifndef ORIENTATIONESTIMATOR_H
#define ORIENTATIONESTIMATOR_H

#include "extkalman.h"

namespace mart {

class OrientationEstimator
{
public:
    using EKF = ExtendedKalmanFilter<float, 12, 12>;

    OrientationEstimator();

private:
    EKF ekf_;
};

}  // mart


#endif /* ORIENTATIONESTIMATOR_H */
