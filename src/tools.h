#ifndef TOOLS_H_
#define TOOLS_H_
#include <vector>
#include "Eigen/Dense"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace std;

class Tools {

	// Variables for running computation of root mean standard
	// error using Kahan sum
	VectorXd mse;
	VectorXd lastmse;
	VectorXd residual;
	VectorXd kahanerror;
	VectorXd rmse;

	// Reset running root mean standard error calculation to zero
	void resetRMSE();

public:
  /**
  * Constructor.
  */
  Tools();

  /**
  * Destructor.
  */
  virtual ~Tools();

  /**
  * A helper method to calculate RMSE.
  */
  VectorXd CalculateRMSE(const vector<VectorXd> &estimations, const vector<VectorXd> &ground_truth);

  /**
  * A helper method to calculate Jacobians.
  */
  MatrixXd CalculateJacobian(const VectorXd& x_state);

};

#endif /* TOOLS_H_ */
