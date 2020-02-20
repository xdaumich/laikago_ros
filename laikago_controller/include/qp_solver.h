#ifndef CATKIN_WS_QP_SOLVER_H
#define CATKIN_WS_QP_SOLVER_H

#include <casadi/casadi.hpp>
#include <Eigen/Dense>

using namespace casadi;

//todo: This is specified to stance torque
class QpSolver {
public:
    QpSolver();

    Eigen::Matrix<float, 12, 1> solve(const Eigen::MatrixXf &A, const Eigen::MatrixXf &b);

private:
    Opti opti_;
    MX x_;
    MX A_;
    MX b_;
    Function opti_f_;
};

class QpLow {
public:
    QpLow();
    Eigen::Matrix<float, 12, 1> solve(const Eigen::MatrixXf &A_in, const Eigen::MatrixXf &b_in);

private:
    DM A_;
    Function S_;
};

#endif //CATKIN_WS_QP_SOLVER_H
