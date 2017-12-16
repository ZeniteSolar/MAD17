/*
 * PeO.c
 *
 * Created: 15/12/2017 19:18:18
 *  Author: jack
 */ 
#include "PeO.h"
/**
 * @brief P&O algorithm
 */
void pertub_and_observe(void)
{	
	static uint8_t	pi_med_old = 0;
	//computes voltage and current
	control.V = ma_adc0();
	control.I = ma_adc1();
	
	// Computes power input
	control.pi_med = (control.V)*(control.I);
	// Respects limits for duty Cycle
	if( control.pi_med < pi_med_old )	control.updown ^=1; // configurar updown como variavel global (talvez)

	// Apply a perturbation
	if(!control.updown) control.D -= D_STEP;		// D_STEP nao tem valor em #define 
	else control.D += D_STEP;

	// recycles
	pi_med_old = control.pi_med;
}