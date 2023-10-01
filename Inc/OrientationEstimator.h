#ifndef ORIENTATIONESTIMATOR_H
#define ORIENTATIONESTIMATOR_H

#include "extkalman.h"

namespace mart {

class OrientationEstimator
{
public:
    OrientationEstimator();

private:
    using EKF = ExtendedKalmanFilter<float, 9, 9>;
    using State = typename EKF::State;
    using Measurement = typename EKF::Measurement;

    static State process(const State& prevState);
    static Measurement measurement(const State& state);

    EKF ekf_;
};

}  // mart


#endif /* ORIENTATIONESTIMATOR_H */
