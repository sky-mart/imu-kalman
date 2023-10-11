#include "OrientationEstimator.h"

namespace mart {

namespace
{

using State = typename OrientationEstimator::EKF::State;
using ProcessMatrix = typename OrientationEstimator::EKF::ProcessMatrix;
using Measurement = typename OrientationEstimator::EKF::Measurement;
using MeasurementMatrix = typename OrientationEstimator::EKF::MeasurementMatrix;

State process(const State& cur, float dt)
{
    State next;
    for (int i = 0; i < 3; ++i) {
        next[i] = cur[i] + cur[3 + i] * dt;
        next[3 + i] = cur[3 + i];
    }

    const auto rotM = rotationMatrix(cur.subvec<3>(0));
    const auto curG = cur.subvec<3>(6);
    const auto  nextG = curG + rotM * curG * dt;
    for (int i = 0; i < 3; ++i) {
        next[6 + i] = nextG[i];
    }

    const auto curM = cur.subvec<3>(9);
    const auto nextM = curM + rotM * curM * dt;
    for (int i = 0; i < 3; ++i) {
        next[9 + i] = nextM[i];
    }
    return next;
}

void getProcessJacobian(const State& state, ProcessMatrix& jacobian, float dt)
{

}

Measurement measurement(const State& state, float dt)
{
    return state;
}

void getMeasurementJacobian(const State& state, MeasurementMatrix& jacobian, float dt)
{
}

}

OrientationEstimator::OrientationEstimator()
    : ekf_(&process,
           &getProcessJacobian,
           ProcessMatrix(),
           &measurement,
           &getMeasurementJacobian,
           MeasurementMatrix())
{}




}
