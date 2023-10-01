#include "OrientationEstimator.h"

namespace mart {

namespace
{

using State = typename OrientationEstimator::EKF::State;
using ProcessMatrix = typename OrientationEstimator::EKF::ProcessMatrix;
using Measurement = typename OrientationEstimator::EKF::Measurement;
using MeasurementMatrix = typename OrientationEstimator::EKF::MeasurementMatrix;

State process(const State& state)
{
    return state;
}

void getProcessJacobian(const State& state, ProcessMatrix& jacobian)
{

}

Measurement measurement(const State& state)
{
    return state;
}

void getMeasurementJacobian(const State& state, MeasurementMatrix& jacobian)
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
