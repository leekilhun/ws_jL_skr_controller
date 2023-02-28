/*
 * main.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */




#include "main.h"



int main(void)
{

  bspInit();
  hwInit();
  exhwInit();
  apInit();
  apMain();

  return 0;
}

