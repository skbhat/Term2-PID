#ifndef PID_H
#define PID_H

#include<vector>

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

  /*
   * My variables
   */
  int iter_count;
  double prev_cte, total_cte;
  double prev_steer, prev_angle;
  double max_cte;


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
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte, double steer, double angle);

  /*
  * Calculate the total PID error.
  */
  double TotalError();
};

#endif /* PID_H */
