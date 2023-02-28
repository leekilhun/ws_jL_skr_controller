/*
 * ap.h
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */

#ifndef SRC_AP_AP_H_
#define SRC_AP_AP_H_


#include "ap_def.h"

#if 0
struct prc_step_t
{
  volatile uint8_t curr_step{};
  volatile uint8_t pre_step{};
  volatile uint8_t wait_step{};
  volatile uint32_t prev_ms{};
  volatile uint32_t elap_ms{};
  volatile uint8_t retry_cnt{};
  UTL::_que<uint8_t> msgQ;
  volatile bool wait_resp{};//true - wait step complete, false - completed step

  inline void SetStep(uint8_t step){
  	elap_ms = millis() - prev_ms;
    prev_ms = millis();
  	pre_step = curr_step;
    curr_step = step;
  }

  inline uint8_t GetStep() const{
    return curr_step;
  }

  inline bool LessThan(uint32_t msec){
  	elap_ms = millis() - prev_ms;
  	if (elap_ms < msec)
  		return true;
  	else
  		return false;
  }

  inline bool MoreThan(uint32_t msec){
  		return !LessThan(msec);
  }


  inline bool Available() const {
  	return !wait_resp;
  }


  inline bool IsInStep (uint8_t step) {
  	if (msgQ.m_Head)
  	{
  		if (msgQ.m_Buff[msgQ.m_Head - 1] != step
  				|| msgQ.m_Buff[msgQ.m_Head - 1] != step + 1
					|| msgQ.m_Buff[msgQ.m_Head - 1] != step + 2)
  		{
  			return true;
  		}
  	}
  	return false;
  }

} ;


#endif


void apInit(void);
void apMain(void);



#endif /* SRC_AP_AP_H_ */
