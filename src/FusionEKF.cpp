#include "FusionEKF.h"
#include "tools.h"
#include "Eigen/Dense"
#include <iostream>
#define EPS 0.00001 // should be a very small number

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

/*
 * Constructor.
 */
FusionEKF::FusionEKF() {
	is_initialized_ = false;

	previous_timestamp_ = 0;

	// initializing matrices
	// It isn't really necessary to have persistent
	// storage for these in FusionEKF.
	// They can just as easily be constructed at local scope
	// the first time ProcessMeasurement() is called,
	// then used to initialize persistent containers in
	// KalmanFilter.  I do use them to initialize containers 
	// in KalmanFilter, but I'm keeping them here as well because
	// they are small and if it ain't broke, don't fix it.
	R_laser_ = MatrixXd(2, 2);
	R_radar_ = MatrixXd(3, 3);
	H_laser_ = MatrixXd(2, 4);
	Hj_ = MatrixXd(3, 4);

	// measurement covariance matrix - laser
	R_laser_ << 0.0225, 0,
		0, 0.0225;

	// measurement covariance matrix - radar
	R_radar_ << 0.09, 0, 0,
		0, 0.0009, 0,
		0, 0, 0.09;

	// Finish initializing the FusionEKF.

	// Set the process and measurement noises
	noise_ax = 8.;
	noise_ay = 8.;
}

/**
* Destructor.
*/
FusionEKF::~FusionEKF() {}

void FusionEKF::ProcessMeasurement(const MeasurementPackage &measurement_pack) {


  /*****************************************************************************
   *  Initialization
   ****************************************************************************/
   /**
   TODO:
   * Initialize the state ekf_.x_ with the first measurement.
   * Create the covariance matrix.
   * Remember: you'll need to convert radar from polar to cartesian coordinates.
   */
  if (!is_initialized_) 
  {
	  // Initialize the state ekf_.x_ with the first measurement.
	  // Create the covariance matrix.

	  cout << "FusionEKF inizialization" << endl;

	  VectorXd x(4);
	  if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR)
	  {
		  // Convert radar from polar to cartesian coordinates
		  float rho = measurement_pack.raw_measurements_[0];
		  float phi = measurement_pack.raw_measurements_[1];
		  x << rho * cos(phi), rho*sin(phi), 0.f, 0.f;
	  }
	  else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER)
	  {
		  // Initialize state.
		  x << measurement_pack.raw_measurements_[0], measurement_pack.raw_measurements_[1], 0.f, 0.f;
	  }
   
	  MatrixXd P(4, 4);
	  P << 1, 0, 0, 0,
		  0, 1, 0, 0,
		  0, 0, 1000, 0,
		  0, 0, 0, 1000;

	  // Initialize transition matrix
	  MatrixXd F(4, 4);
	  F << 1, 0, 0, 0,
		  0, 1, 0, 0,
		  0, 0, 1, 0,
		  0, 0, 0, 1;

	  // Initialize measurement matrix for laser measurements
	  H_laser_ << 1, 0, 0, 0,
		  0, 1, 0, 0;

	  // Initialize ekf_ with the first state vector, 
	  // estimated initial state covariance matrix,
	  // and an empty matrix for Q
	  MatrixXd Q(4, 4);
	  ekf_.Init(x, /*x_in*/
		  P, /*P_in*/
		  F, /*F_in*/
		  H_laser_, /*H_in*/
		  Hj_, /*Hj_in*/
		  R_laser_, /*R_in*/
		  R_radar_, /*R_ekf_in*/
		  Q); /*Q_in*/

	
	
	// first measurement

	  if (fabs(ekf_.x_(0)) < EPS and fabs(ekf_.x_(1)) < EPS) {
		  ekf_.x_(0) = EPS;
		  ekf_.x_(1) = EPS;
	  }

	  // Initial covariance matrix
	  ekf_.P_ = MatrixXd(4, 4);
	  ekf_.P_ << 1, 0, 0, 0,
		  0, 1, 0, 0,
		  0, 0, 1000, 0,
		  0, 0, 0, 1000;

	  // Print the initialization results
	  cout << "EKF init: " << ekf_.x_ << endl;

	  // Save the initiall timestamp for dt calculation
	  previous_timestamp_ = measurement_pack.timestamp_;

	  // Inizialization completed
	  is_initialized_ = true;
	  return;
  }
  /*****************************************************************************
   *  Prediction
   ****************************************************************************/

  /**
   TODO:
     * Update the state transition matrix F according to the new elapsed time.
      - Time is measured in seconds.
     * Update the process noise covariance matrix.
     * Use noise_ax = 9 and noise_ay = 9 for your Q matrix.
   */
  
   // Compute the time from the previous measurement in seconds.
  float dt = (measurement_pack.timestamp_ - previous_timestamp_) / 1000000.0;
  previous_timestamp_ = measurement_pack.timestamp_;

  if (dt > 0.)
  {
	  // Update the motion model matrix for a timestep dt.
	  // We use a motion model with a constant velocity.
	  ekf_.F_(0, 2) = dt;
	  ekf_.F_(1, 3) = dt;

	  // Update the process noise covariance matrix for a timestep dt.
	  // Our motion model uses Gaussian random accelerations in the x and y directions.
	  float dt2 = dt * dt;
	  float dt3 = dt2 * dt;
	  float dt4 = dt3 * dt;
	  float dt4over4 = dt4 / 4.;
	  float dt3over2 = dt3 / 2.;
	  ekf_.Q_ << dt4over4 * noise_ax, 0, dt3over2*noise_ax, 0,
		  0, dt4over4*noise_ay, 0, dt3over2*noise_ay,
		  dt3over2*noise_ax, 0, dt2*noise_ax, 0,
		  0, dt3over2*noise_ay, 0, dt2*noise_ay;
	  ekf_.Predict();
  }
  
  /*****************************************************************************
   *  Update
   ****************************************************************************/

  /**
   TODO:
     * Use the sensor type to perform the update step.
     * Update the state and covariance matrices.
   */

  if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR)
  {
	  // Radar updates
	  cout << "RADAR: " << endl;
	  ekf_.UpdateEKF(measurement_pack.raw_measurements_);
  }
  else
  {
	  // Laser updates
	  cout << "LASER: " << endl;
	  ekf_.Update(measurement_pack.raw_measurements_);
  }
  
  // print the output
  cout << "x_ = " << ekf_.x_ << endl;
  cout << "P_ = " << ekf_.P_ << endl;
}