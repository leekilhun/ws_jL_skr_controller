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

	constexpr int MOTOR_PULSES_PER_REV = 1600;  // Motor pulses per revolution, motor step 1.8 degree, tmc2209 microstep 1/8
	constexpr int MOTOR_MAX_PRM = 500;// Maximum motor RPM
	constexpr int MOTOR_ACCEL_TIME_MS = 200;  // Acceleration and deceleration time

	class enStepMotor
	{
	public:
		struct cfg_s{
			uint8_t 			motor_id{};
			tim_tbl_t*    ptr_timer{};

			cfg_s() = default;
			cfg_s& operator= (const cfg_s &other){
				motor_id = other.motor_id;
				ptr_timer = other.ptr_timer;
				return *this;
			};
		};


		enum :uint8_t {motor_run,motor_stop};
	public:

		// Motor control variables
    volatile uint32_t accel_index;
    volatile uint32_t vel_index;
    volatile uint32_t decel_index;
    volatile int8_t   dir;          // direction

    volatile uint32_t step_position; // 위치   스텝
 		volatile uint32_t total_steps;   // 가야할 스텝

    volatile uint32_t speed;  //
    volatile uint32_t step_count;  //
    volatile uint32_t count;
    volatile bool     move_done ;
    volatile uint8_t  state;


    /*
    volatile uint32_t steps_per_s;  // Steps per second
		volatile uint32_t steps_per_ms;  // Steps per millisecond
		volatile uint32_t curr_speed;  // Current speed in steps per second
		volatile uint32_t accel_steps;  // Steps needed to reach maximum speed during acceleration
		volatile uint32_t decel_steps;  // Steps needed to stop the motor during deceleration
		volatile uint32_t steps_to_target;  // Steps remaining to reach target revolutions

		volatile uint32_t accel_count;  // Number of steps taken during acceleration
		volatile uint32_t decel_count;  // Number of steps taken during deceleration
		volatile uint32_t time_since_last_step;  // Time since the last step in microseconds
		volatile uint32_t last_step_time;  // Time of the last step in microseconds
		*/
		prc_step_t step;
		cfg_s m_cfg;

	public:
		enStepMotor ():accel_index{},vel_index{},decel_index{},dir{},
		step_position{}, total_steps{}, speed{}, step_count{}, count{}, move_done{},
		state{}, step{} , m_cfg{} {

		};
		virtual	~enStepMotor (){};
	  /*
	  enStepMotor (const enStepMotor &other){};
		enStepMotor (enStepMotor &&other){};

		enStepMotor& operator= (enStepMotor &&other){
			return *this;
		};
		enStepMotor& operator= (const enStepMotor &other){
			return *this;
		};
		*/

	private:

		inline void delay_us(uint32_t us) {
			volatile uint32_t i;
			for (i=0; i<us; i++) { }
		}



		uint S_table_len(uint* table);
		void S_curve_gen(int steps);

	public:
		inline void Init(cfg_s &cfg) {
			// Initialize GPIO

			// Initialize timer

			// Calculate motor control variables

			// Set initial motor control variables

			// Start the timer and motor

			timStart(_DEF_TIM1);
			timAttachCB(_DEF_TIM1, this, ISR_axis);
			m_cfg = cfg;

		};


		/* test code 1cycle for and backward*/
		inline void test_stepper_constant_accel()
		{
			constexpr auto STEPS = MOTOR_PULSES_PER_REV;

		  uint32_t delays[STEPS]{};
		  float angle =1;
		  float accel = 0.01;
		  float c0 = 2000 * sqrt(2 * angle/accel)*0.67703;
		  float last_delay = 0;
		  //float max_feq = (MOTOR_MAX_PRM/60)*MOTOR_PULSES_PER_REV;
		  uint32_t high_speed = /*(1/max_feq)*1000000 */360;


		  float d = c0;

		  for (uint32_t i =0; i <STEPS; i++)
		  {
		  	if ( i > 0 )
		  		d = last_delay - (2 * last_delay)/(4*i+1);

		  	if (d < high_speed)
		  		d = high_speed;

		  	delays[i] = d;
		  	last_delay = d;
		  }

		  // use delays from the array, forward
		  for (int i=0; i<STEPS ; i++)
		  {
		  	StepPulse();

		  	delay_us(delays[i]);
		  }

		  // high speed
		  for (int i=0; i<STEPS*4; i++)
		  {
		  	StepPulse();

		  	delay_us(high_speed);
		  }

		  // use delays from the array, backward
		  for (int i=0; i<STEPS; i++)
		  {
		  	StepPulse();

		  	delay_us(delays[STEPS-i-1]);
		  }

		}


		void Run(int steps, uint32_t speed);
		void SetStep(int steps, uint32_t speed);
		void SetRel(int steps, uint32_t speed);

		void RunAndWait();

		inline void Stop(){
			timDisableISR(_DEF_TIM1);
		}

		inline void Enable(){
			gpioPinWrite(_GPIO_MOT_X_ENABLE,_DEF_LOW);
		};

		inline void Disable(){
			gpioPinWrite(_GPIO_MOT_X_ENABLE,_DEF_HIGH);
		};

		inline void StepPulse(){
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13);
		};

		inline void Direction(bool is_ccw){
			gpioPinWrite(_GPIO_MOT_X_DIR, is_ccw);
		};


		static void ISR_axis(void* obj, void* w_param, void* l_param);

		inline static void ISR_axis(void){

		};

	};
	// end of class
#if 0

	constexpr int MOTOR_PULSES_PER_REV = 1600;  // Motor pulses per revolution, motor step 1.8 degree, tmc2209 microstep 1/8
	constexpr int MOTOR_MAX_PRM = 2000;// Maximum motor RPM
	constexpr int MOTOR_ACCEL_TIME_MS = 200;  // Acceleration and deceleration time
	constexpr int MOTOR_TARGET_REVOLUTIONS = 10000;  // Target number of revolutions


	class enStepMotor
	{
	public:
		// Motor control variables
		uint32_t steps_per_s;  // Steps per second
		uint32_t steps_per_ms;  // Steps per millisecond
		uint32_t curr_speed;  // Current speed in steps per second
		uint32_t accel_steps;  // Steps needed to reach maximum speed during acceleration
		uint32_t decel_steps;  // Steps needed to stop the motor during deceleration
		uint32_t steps_to_target;  // Steps remaining to reach target revolutions
		uint32_t target_steps;  // Total steps needed to reach target revolutions
		uint32_t step_count;  // Total number of steps taken
		uint32_t accel_count;  // Number of steps taken during acceleration
		uint32_t decel_count;  // Number of steps taken during deceleration
		uint32_t time_since_last_step;  // Time since the last step in microseconds
		uint32_t last_step_time;  // Time of the last step in microseconds

		tim_tbl_t*        ptr_timer;
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

		inline void Init() {
			// Initialize GPIO

			// Initialize timer
			timStart(_DEF_TIM1);

			// Calculate motor control variables
			steps_per_s = (MOTOR_MAX_PRM * MOTOR_PULSES_PER_REV) / 60;
			steps_per_ms = steps_per_s / 1000;
			accel_steps = (steps_per_ms * MOTOR_ACCEL_TIME_MS) / 2;  // Divide by 2 because we accelerate and decelerate
			decel_steps = accel_steps;
			target_steps = MOTOR_TARGET_REVOLUTIONS * MOTOR_PULSES_PER_REV;
			steps_to_target = target_steps - accel_steps - decel_steps;

			// Set initial motor control variables
			curr_speed = 0;
			step_count = 0;
			accel_count = 0;
			decel_count = 0;
			time_since_last_step = 0;
			last_step_time = TIM3->CNT;

			ptr_timer = timGetData(_DEF_TIM1); //timer 3



			// Start the timer and motor
			//TIM1->CR1 |= TIM_CR1_CEN;
			//GPIOA->ODR |= GPIO_ODR_OD0;

			ptr_timer->func_cb = ISR_axis;

		};

		inline void Start(){
			timEnableISR(_DEF_TIM1);
		}

		inline void Stop(){
			timDisableISR(_DEF_TIM1);
		}

		inline void Enable(){
			gpioPinWrite(_GPIO_MOT_X_ENABLE,_DEF_LOW);
		};

		inline void Disable(){
			gpioPinWrite(_GPIO_MOT_X_ENABLE,_DEF_HIGH);
		};

		inline void Direction(int dir){
			gpioPinWrite(_GPIO_MOT_X_DIR,dir);
		};

		inline void StepPulse(){
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13);
		};

		// Timer interrupt handler
		inline static void ISR_axis(void* obj, void* w_param, void* l_param){
			enStepMotor* ptr_this = (enStepMotor*)obj;

			// Clear timer update interrupt flag
			TIM3->SR &= ~TIM_SR_UIF;

			// Calculate time since last step
			uint32_t curr_time = TIM3->CNT;
			uint32_t delta_t = curr_time - ptr_this->last_step_time;
			ptr_this->last_step_time = curr_time;
			ptr_this->time_since_last_step += delta_t;

			// Check if it's time to take a step
			if (ptr_this->time_since_last_step >= (1000000 / ptr_this->curr_speed))
			{
				// Reset time since last step
				ptr_this->time_since_last_step = 0;

				// Increment step count
				ptr_this->step_count++;

				// Determine if we are in acceleration, constant speed, or deceleration phase
				if (ptr_this->step_count <= ptr_this->accel_steps)
				{
					// Calculate speed for the current step during acceleration
					ptr_this->curr_speed = ((ptr_this->step_count * ptr_this->steps_per_s) / ptr_this->accel_steps);
					ptr_this->accel_count++;
				}
				else if (ptr_this->step_count > (ptr_this->target_steps - ptr_this->decel_steps))
				{
					// Calculate speed for the current step during deceleration
					ptr_this->curr_speed = ((ptr_this->steps_to_target - ptr_this->step_count) * ptr_this->steps_per_s) / ptr_this->decel_steps;
					ptr_this->decel_count++;
				}
				else
				{
					// Keep the motor at maximum speed during constant speed phase
					ptr_this->curr_speed = ptr_this->steps_per_s;
				}

				// Toggle the step pin to move the motor
				ptr_this->StepPulse();
			}

			// Check if we've reached the target number of revolutions
			if (ptr_this->step_count >= ptr_this->target_steps) {
				// Stop the timer and motor
				ptr_this->Stop();
				//TIM1->CR1 &= ~TIM_CR1_CEN;
				//GPIOA->ODR &= ~GPIO_ODR_OD0;
			}

		};

		inline static void ISR_axis(void){

#if 0
			// Clear timer update interrupt flag
			TIM3->SR &= ~TIM_SR_UIF;

			// Calculate time since last step
			uint32_t curr_time = TIM3->CNT;
			uint32_t delta_t = curr_time - last_step_time;
			last_step_time = curr_time;
			time_since_last_step += delta_t;

			// Check if it's time to take a step
			if (time_since_last_step >= (1000000 / curr_speed)) {
				// Reset time since last step
				time_since_last_step = 0;

				// Increment step count
				step_count++;

				// Determine if we are in acceleration, constant speed, or deceleration phase
				if (step_count <= accel_steps) {
					// Calculate speed for the current step during acceleration
					curr_speed = ((step_count * steps_per_s) / accel_steps);
					accel_count++;
				} else if (step_count > (target_steps - decel_steps)) {
					// Calculate speed for the current step during deceleration
					curr_speed = ((steps_to_target - step_count) * steps_per_s) / decel_steps;
					decel_count++;
				} else {
					// Keep the motor at maximum speed during constant speed phase
					curr_speed = steps_per_s;
				}

				// Toggle the step pin to move the motor
				GPIOA->ODR ^= GPIO_ODR_OD0;
			}

			// Check if we've reached the target number of revolutions
			if (step_count >= target_steps) {
				// Stop the timer and motor
				TIM3->CR1 &= ~TIM_CR1_CEN;
				GPIOA->ODR &= ~GPIO_ODR_OD0;
			}

#endif
		}


	};


#endif

} /* namespace MOTOR */

#endif /* SRC_AP__INC_ENSTEPMOTOR_HPP_ */
