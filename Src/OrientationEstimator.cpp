#include "OrientationEstimator.h"

namespace mart
{
namespace orient
{

namespace
{

using State             = typename OrientationEstimator::EKF::State;
using ProcessMatrix     = typename OrientationEstimator::EKF::ProcessMatrix;
using Measurement       = typename OrientationEstimator::EKF::Measurement;
using MeasurementMatrix = typename OrientationEstimator::EKF::MeasurementMatrix;

const auto I = alloc::Matrix<float, VEC_SIZE, VEC_SIZE>::eye();
const alloc::Matrix<float, VEC_SIZE, VEC_SIZE> O;

enum { Omega, OmegaDot, G, M };
enum { X, Y, Z };

template <class T>
::mart::alloc::Matrix<T, VEC_SIZE, VEC_SIZE> rotationMatrix(
    const Vector<T, VEC_SIZE>& w)
{
    return ::mart::alloc::Matrix<T, VEC_SIZE, VEC_SIZE>(
        {0, -w[Z], w[Y], w[Z], 0, -w[X], -w[Y], w[X], 0});
}

void process(State& nextState, const State& currentState, float dt)
{
    auto current = currentState.partition<VEC_SIZE>();
    auto next    = nextState.partition<VEC_SIZE>();

    next[Omega]    = current[Omega] + current[OmegaDot] * dt;
    next[OmegaDot] = current[OmegaDot];

    const auto rotM = rotationMatrix(current[Omega]);
    next[G]         = current[G] + rotM * current[G] * dt;
    next[M]         = current[M] + rotM * current[M] * dt;
}

void getProcessJacobian(const State& currentState,
                        ProcessMatrix& jacobian,
                        float dt)
{
    auto J       = jacobian.partition<VEC_SIZE, VEC_SIZE>();
    auto current = currentState.partition<VEC_SIZE>();

    const auto rotG = rotationMatrix(current[G]) * (-dt);
    const auto rotM = rotationMatrix(current[M]) * (-dt);
    const auto rotW = I + rotationMatrix(current[Omega]) * dt;

    J = std::initializer_list<Matrix<float, VEC_SIZE, VEC_SIZE>>{
        I, I * dt, O, O, O, I, O, O, rotG, 0, rotW, 0, rotM, 0, 0, rotW};
}

Measurement measurement(const State& state, float dt)
{
    return Measurement{};
}

void getMeasurementJacobian(const State& state,
                            MeasurementMatrix& jacobian,
                            float dt)
{
}

}  // namespace

OrientationEstimator::OrientationEstimator()
    : ekf_(&process,
           &getProcessJacobian,
           ProcessMatrix::Alloc(),
           &measurement,
           &getMeasurementJacobian,
           MeasurementMatrix::Alloc())
{
}

}  // namespace orient
}  // namespace mart
