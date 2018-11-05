#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_init, double Ki_init, double Kd_init, bool use_p, bool use_i, bool use_d) {
    PID::Kp = Kp_init;
    PID::Ki = Ki_init;
    PID::Kd = Kd_init;
    PID::use_p = use_p;
    PID::use_i = use_i;
    PID::use_d = use_d;
    p_error = 0.0;
    i_error = 0.0;
    d_error = 0.0;
    Kx_ptr[1] = &Kp;
    Kx_ptr[2] = &Ki;
    Kx_ptr[3] = &Kd;
    dp[1] = 1.0;
    dp[2] = 1.0;
    dp[3] = 1.0;
    reduced_dp[0] = false;
    reduced_dp[1] = false;
    reduced_dp[2] = false;
    first_iter = true;

    double int_cte = 0.0;
    double prev_cte = 0.0;
}

void PID::UpdateError(double cte) {
    double diff_cte;
    if(first_iter){
        best_cte = cte;
        first_iter = false;
    }
    if(prev_cte!=0.0){
        diff_cte = cte - prev_cte;
    }
    else{
        diff_cte = 0.0;
    }
    prev_cte = cte;
    int_cte += cte;

    if(std::fabs(best_cte) >0.0){
        for (int i = 0; i < 3;i++){
            *Kx_ptr[i] += dp[i];
            if (fabs(cte) < fabs(best_cte)){
                best_cte = cte;
                dp[i]*=1.1;
                if(reduced_dp[i]){
                    reduced_dp[i] = false;
                }
            }
            else if (fabs(cte) >= fabs(best_cte)) {

                if (reduced_dp[i]) {
                    *Kx_ptr[i] += dp[i];
                    dp[i] *= 0.9;
                }
                else if (!reduced_dp[i]){
                    *Kx_ptr[i] -= 2 * dp[i];
                    reduced_dp[i] = true;
                }
            }
        }
    }
    p_error = Kp*cte;
    i_error = Ki*int_cte;
    d_error = Kd*diff_cte;
}

double PID::TotalError() {
    double steer_value = 0.0;
    if (use_p)
        steer_value -= p_error;
    if (use_i)
        steer_value -= i_error;
    if (use_d)
        steer_value -= d_error;
    return steer_value;
}

