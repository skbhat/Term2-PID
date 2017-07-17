#include "PID.h"
#include <math.h>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {

	PID::Kp = Kp;
	PID::Ki = Ki;
	PID::Kd = Kd;
	p_error = d_error = i_error = 0.0;

	iter_count = 0;
	prev_cte = total_cte = 0;
	prev_steer = 0;
	prev_angle = 0;
	max_cte = 0;
}

void PID::UpdateError(double cte, double steer, double angle) {
	prev_steer = steer;
	prev_angle = angle;
	if (fabs(cte)>max_cte)
		max_cte = fabs(cte);


	prev_cte = cte;
	if(cte==0){
		total_cte = 0;
	}
	else{
		total_cte += cte;
	}
	iter_count++;
}

double PID::TotalError() {
	return 0.0;
}

