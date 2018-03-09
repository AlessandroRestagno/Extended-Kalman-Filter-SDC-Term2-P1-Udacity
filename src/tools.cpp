#include <iostream>
#include "tools.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() : mse(4),
lastmse(4),
residual(4),
kahanerror(4),
rmse(4)
{
	resetRMSE();
}

Tools::~Tools() {}

void Tools::resetRMSE()
{
	mse << 0, 0, 0, 0;
	lastmse << 0, 0, 0, 0;
	residual << 0, 0, 0, 0;
	kahanerror << 0, 0, 0, 0;
}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
	const vector<VectorXd> &ground_truth) {
	VectorXd rmse(4);
	rmse << 0, 0, 0, 0;
	// Check the validity of the following inputs:
	// The estimation vector size should not be zero
	float t = estimations.size(); // Current timestep index

	// check inputs
	if (t == 0)
		cout << "Error in CalculateRMSE:  estimations.size() = 0" << endl;
	if (t != ground_truth.size())
		cout << "Error in CalculateRMSE: sizes of estimation and ground truth do not match" << endl;
	
	
	// Accumulate squared residuals
	for (unsigned int i = 0; i < estimations.size(); ++i) {
		VectorXd residual = estimations[i] - ground_truth[i];
		// Coefficient-wise multiplication
		residual = residual.array()*residual.array();
		rmse += residual;
	}

	// Calculate the mean
	rmse = rmse / estimations.size();
	rmse = rmse.array().sqrt();

	// cout << estimations.size() << endl;
	if (rmse(0) > .11 ||
		rmse(1) > .11 ||
		rmse(2) > .52 ||
		rmse(3) > .52)
		cout << "Warning at timestep " << t << " rmse: " << rmse << endl;

	if (rmse(0) > .11)
		cout << "rmse(0) exceeds tolerance of .11" << endl;
	if (rmse(1) > .11)
		cout << "rmse(1) exceeds tolerance of .11" << endl;
	if (rmse(2) > .52)
		cout << "rmse(2) exceeds tolerance of .52" << endl;
	if (rmse(3) > .52)
		cout << "rmse(3) exceeds tolerance of .52" << endl;
	/**
	TODO:
	* Calculate the RMSE here.
	*/

	return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
	MatrixXd Hj(3, 4);

	float px = x_state(0);
	float py = x_state(1);
	float vx = x_state(2);
	float vy = x_state(3);

	//check division by zero
	if (px == 0 && py == 0)
	{
		cout << "Error:  division by zero in CalculateJacobian" << endl;
		return Hj;
	}

	//compute the Jacobian 
	float rho = sqrt(px*px + py * py);
	float rho2 = rho * rho;
	float rho3 = rho2 * rho;
	Hj << px / rho, py / rho, 0, 0,
		-py / rho2, px / rho2, 0, 0,
		py*(vx*py - vy * px) / rho3, px*(vy*px - vx * py) / rho3, px / rho, py / rho;

	return Hj;
	/**
  TODO:
    * Calculate a Jacobian here.
  */
}