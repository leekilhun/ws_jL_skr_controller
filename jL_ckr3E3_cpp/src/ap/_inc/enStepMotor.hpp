/*
 * enStepMotor.h
 *
 *  Created on: Mar 14, 2023
 *      Author: gns2l
 */

#ifndef SRC_AP__INC_ENSTEPMOTOR_HPP_
#define SRC_AP__INC_ENSTEPMOTOR_HPP_

namespace MOTOR
{

	class enStepMotor
	{


	public:
		enStepMotor (){};
		virtual	~enStepMotor (){};
		enStepMotor (const enStepMotor &other){};
		enStepMotor (enStepMotor &&other){};

		enStepMotor& operator= (enStepMotor &&other){
			return *this;
		};
		enStepMotor& operator= (const enStepMotor &other){
			return *this;
		};


	public:


	};

} /* namespace MOTOR */

#endif /* SRC_AP__INC_ENSTEPMOTOR_HPP_ */
