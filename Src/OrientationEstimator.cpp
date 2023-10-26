#include "OrientationEstimator.h"

namespace mart {

namespace
{

using EkfState = typename OrientationEstimator::EKF::State;
using ProcessMatrix = typename OrientationEstimator::EKF::ProcessMatrix;
using Measurement = typename OrientationEstimator::EKF::Measurement;
using MeasurementMatrix = typename OrientationEstimator::EKF::MeasurementMatrix;

class State : public EkfState
{
public:
    auto omega() const { return subvec<3>(0); }

    auto omegaDot() const { return subvec<3>(3); }

    auto g() const { return subvec<3>(6); }

    auto m() const { return subvec<3>(9); }
};

void process(EkfState& nextState, const EkfState& currentState, float dt)
{
    const auto& current = reinterpret_cast<const State&>(currentState);
    auto& next = reinterpret_cast<State&>(nextState);

    next.omega() = current.omega() + current.omegaDot() * dt;
    next.omegaDot() = current.omegaDot();

    const auto rotM = rotationMatrix(current.omega());
    next.g() = current.g() + rotM * current.g() * dt;
    next.m() = current.m() + rotM * current.m() * dt;
}

void getProcessJacobian(const EkfState& state, ProcessMatrix& jacobian, float dt)
{

}

Measurement measurement(const EkfState& state, float dt)
{
    return state;
}

void getMeasurementJacobian(const EkfState& state, MeasurementMatrix& jacobian, float dt)
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
