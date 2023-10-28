#include "OrientationEstimator.h"

namespace mart {

namespace
{

using EkfState = typename OrientationEstimator::EKF::State;
using ProcessMatrix = typename OrientationEstimator::EKF::ProcessMatrix;
using Measurement = typename OrientationEstimator::EKF::Measurement;
using MeasurementMatrix = typename OrientationEstimator::EKF::MeasurementMatrix;

template<class T>
::mart::alloc::Matrix<T, 3, 3> rotationMatrix(const Vector<T, 3>& w)
{
    return ::mart::alloc::Matrix<T, 3, 3>({
        0,    -w[2], w[1],
        w[2],  0,   -w[0],
        -w[1], w[0], 0
    });
}

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

void getProcessJacobian(const EkfState& currentState, ProcessMatrix& jacobian, float dt)
{
    auto J = jacobian.partition<3, 3>();
    const auto& current = reinterpret_cast<const State&>(currentState);
    const auto I = alloc::Matrix<float, 3, 3>::eye();
    const alloc::Matrix<float, 3, 3> O;
    const auto rotG = rotationMatrix(current.g()) * (-dt);
    const auto rotM = rotationMatrix(current.m()) * (-dt);
    const auto rotW = I + rotationMatrix(current.omega()) * dt;

    J = std::initializer_list<Matrix<float, 3, 3>>{
        I,    I*dt, O,    O,
        O,    I,    O,    O,
        rotG, 0,    rotW, 0,
        rotM, 0,    0,    rotW
    };
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
           ProcessMatrix::Alloc(),
           &measurement,
           &getMeasurementJacobian,
           MeasurementMatrix::Alloc())
{}




}
