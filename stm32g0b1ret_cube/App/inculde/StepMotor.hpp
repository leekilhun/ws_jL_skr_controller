/*
 * StepMotor.hpp
 *
 *  Created on: Mar 13, 2023
 *      Author: gns2l
 */

#ifndef INCULDE_STEPMOTOR_HPP_
#define INCULDE_STEPMOTOR_HPP_

namespace MOTOR
{

	class StepMotor
	{
	public:
		StepMotor ();
		virtual	~StepMotor ();
		StepMotor (const StepMotor &other);
		StepMotor (StepMotor &&other);
		StepMotor&	operator= (const StepMotor &other);
		StepMotor&	operator= (StepMotor &&other);

	};

} /* namespace MOTOR */

#endif /* INCULDE_STEPMOTOR_HPP_ */
