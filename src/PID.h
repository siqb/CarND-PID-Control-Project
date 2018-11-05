#ifndef PID_H
#define PID_H
#include <cmath>
class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  // component switches
  bool use_p;
  bool use_i;
  bool use_d;

  // prev_cte
  double prev_cte;
  double int_cte;
  double * Kx_ptr[3];
  double dp[3];
  double best_cte;
  bool reduced_dp[3];
  bool first_iter;
  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd, bool use_p, bool use_i, bool use_d);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();
};

#endif /* PID_H */
