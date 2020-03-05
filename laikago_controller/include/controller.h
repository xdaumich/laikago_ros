#ifndef CATKIN_WS_CONTROLLER_H
#define CATKIN_WS_CONTROLLER_H

#include <casadi/casadi.hpp>
#include <Eigen/Dense>
#include "body.h"
#include "body_estimation.h"
#include "eigen_def.h"
#include "kinematics.h"
#include "laikago_msgs/GainParam.h"
#include "laikago_msgs/ControlState.h"
#include "qp_solver.h"

using namespace casadi;
using laikago_model::lowCmd;

class LowPassFilter {
public:
    float firstOrder(float x_in, float fil_const) {
        low_pass_x_ = fil_const * x_in + (1 - fil_const) * low_pass_x_;
        return low_pass_x_;
    }

private:
    float low_pass_x_{0};
};

class Controller {
public:
    Controller(ros::NodeHandle *n);

    void paramCallback(const laikago_msgs::GainParam &msg);

    void sendCommand();

    void sendCommandPD();

    void setTime(const double &time);

    static void setMotorZero();

    static const std::vector<Eigen::Vector4i> &getGaitSet();

    static void __attribute__ ((used)) printMat(const Eigen::Matrix<float, 3, 12> &mat) {
        std::cout << mat << std::endl;
    }

    static void __attribute__ ((used)) printMat(const Eigen::MatrixXf &mat) {
        std::cout << mat << std::endl;
    }

private:
    void setTorque(const Vector12f &motor_torque);

    void updateStance();

    Vector12f getKinForceM(const Vector12f &p_feet_desired,
                                             const Matrix3f &M);

    Vector12f getKinForce(const Vector12f &p_feet_desired);

    Vector12f getFpForce(const Vector12f &p_feet_desired);

    void getDynMat(Eigen::Matrix<float, 3, 12> &Mat_lin,
                   Eigen::Matrix<float, 3, 12> &Mat_rot);

    float getGroundWeight();

    Vector12f getFpTarget();

    void getAccState(Vector6f &acc_body);

    int getGaitIndex(const std::vector<Eigen::Vector4i> &D_vec, const std::vector<float> &cost_vec);

    static Matrix3f conjMatrix(const Vector3f &vec);

    float time_{0};
    static constexpr int sample_t_{175}; // 175*0.002 = 0.35s(~3Hz) half period
    Kinematics kin_;
    BodyPoseEstimator est_;
    Vector12f p_feet_default_;
    MultiQp multiQp_;
    unsigned int grf_index_{0};
    ros::NodeHandle &n_;
    ros::Subscriber param_sub_;
    ros::Publisher controlState_pub_;
    laikago_msgs::ControlState controlState_;
    float kp_kin_{1200};
    float kp_[3]{0, 0, 0};
    float kd_[3]{0, 0, 0};
    float kt_[6]{};
    bool is_stance_{false};
    float yaw_offset_{0};
    float control_switch_weight_{0};
    LowPassFilter lpFilterGrf_[12];
    LowPassFilter lpFilterVel_[3];
    float vel_x_{0};
    float vel_y_{0};
};


#endif //CATKIN_WS_CONTROLLER_H
