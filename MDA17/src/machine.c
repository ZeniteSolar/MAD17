#include "machine.h"

/*
 * to-do:
 *      - modularize state sinalization
 *      - 
 *
 */

/**
 * @brief 
 */
void machine_init(void)
{
    TCCR2A  =   (1 << WGM21) | (0 << WGM20)         // Timer 2 in Mode 2 = CTC (clear on compar  e)
            | (0 << COM2A1) | (0 << COM2A0)         // do nothing with OC2A
            | (0 << COM2B1) | (0 << COM2B0);        // do nothing with OC2B
    TCCR2B  =   (0 << WGM22)                        // Timer 0 in Mode 2 = CTC (clear on compar  e)
            | (0 << FOC0A) | (0 << FOC0B)           // dont force outputs
            | (1 << CS22)                           // clock enabled, prescaller = 1024
            | (1 << CS21)
            | (1 << CS20);
    OCR2A   =   80;                                // Valor para igualdade de comparacao A par  a frequencia de 150 Hz
    TIMSK2 |=   (1 << OCIE2A);                      // Ativa a interrupcao na igualdade de comp  aração do TC2 com OCR2A

	
} 

/**
 * @brief checks the quantity of the faults.
 */
inline void check_pwm_fault(void)
{
	#ifdef PWM_ON
    if(pwm_fault_count >= FAULT_COUNT_LIMIT){
        error_flags.fault = 1;
        check_pwm_fault_times = 0;
        set_state_error();
    }else if(check_pwm_fault_times++ > CHECKS_BEFORE_RESET_FAULT_COUNTER){
        pwm_fault_count = 0;    
    }
	#endif
}

/**
 * @brief checks the buffer and waits it fills up
 */
inline void check_buffers(void)
{
    //VERBOSE_MSG_MACHINE(usart_send_string("Checking buffers..."));
    //while(!CBUF_IsFull(cbuf_adc0));
    //VERBOSE_MSG_ERROR(usart_send_string("<No buffers to check>"));
    //VERBOSE_MSG_MACHINE(usart_send_string(" \t\tdone.\n")); 
}

/**
 * @brief checks if the current level is ok for IDLE state
 */
inline void check_idle_current(void)
{
    //VERBOSE_MSG_MACHINE(usart_send_string("Checking motor current levels..."));
    //if(motor_current <= MAXIMUM_OFF_CURRENT);
        //VERBOSE_MSG_MACHINE(usart_send_string(" \t\tdone.\n"));
    //else error_flag: motor_current at off_state bigger than it should be. 
}

/**
 * @brief checks if the voltage level is ok for IDLE state
 */
inline void check_idle_voltage(void)
{
    //VERBOSE_MSG_MACHINE(usart_send_string("Checking motor voltage levels..."));
    //if(output_voltage <= MAXIMUM_OFF_VOLTAGE);
        //VERBOSE_MSG_MACHINE(usart_send_string(" \t\tdone.\n"));
    //else error_flag: motor_voltage at off_state bigger than it should be.
}

/**
 * @brief checks if the temperature level is ok for IDLE state
 */
inline void check_idle_temperature(void)
{
    //VERBOSE_MSG_MACHINE(usart_send_string("Checking motor temperature levels..."));
    //if(temperature <= MAXIMUM_OFF_TEMPERATURE);
        //VERBOSE_MSG_MACHINE(usart_send_string(" \t\tdone.\n"));
    //else error_flag: motor_temperature at off_state bigger than it should be.
}
 

/**
 * @brief checks if the current level is ok for running state
 */
inline void check_running_OutCurrent(void)
{
   control.OutCurrent = ma_adc5(); 
}

/**
 * @brief checks if the current level is ok for running state
 */
inline void check_running_InCurrent(void){
	control.InCurrent = ma_adc4();
}

/**
 * @brief checks if the voltage level is ok for running state
 */
inline void check_running_BatteryTemperature(void)
{
	control.BatteryTemperature = ma_adc3();
}

/**
 * @brief checks if the temperature level is ok for running state
 */
inline void check_running_MotorTemperature(void)
{
    control.MotorTemperature = ma_adc1();
}
 
/**
* @brief checks if the temperature level is ok for running state
*/
inline void check_running_BatteryVoltage(void)
{
    control.BatteryVoltage = ma_adc0();
}
/**
 * @brief checks if the temperature level is ok for running state
 */
inline void check_running_MotorVoltage(void)
{
    control.MotorVoltage = ma_adc2();
}
 
/**
 * @brief set error state
 */
inline void set_state_error(void)
{
    state_machine = STATE_ERROR;
}

/**
* @brief set initializing state
*/ 
inline void set_state_initializing(void)
{
    state_machine = STATE_INITIALIZING;
}

/**
* @brief set idle state
*/ 
inline void set_state_idle(void)
{
    state_machine = STATE_IDLE;
}

/**
* @brief set running state
*/ 
inline void set_state_running(void)
{
    
    state_machine = STATE_RUNNING;
}

/**
* @brief prints the system flags
*/
inline void print_system_flags(void)
{
    /*VERBOSE_MSG_MACHINE(usart_send_string("Motor Sw: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.mppt_on));
    
    VERBOSE_MSG_MACHINE(usart_send_string(" DMS Sw: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.enable));
	
    VERBOSE_MSG_MACHINE(usart_send_string(" Pot_zero: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.all__)); */
}

/**
* @brief prints the error flags
*/
inline void print_error_flags(void)
{/*
    VERBOSE_MSG_MACHINE(usart_send_string("\nOvrI: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.overcurrent));
    
    VERBOSE_MSG_MACHINE(usart_send_string(" OvrV: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.overvoltage));

    VERBOSE_MSG_MACHINE(usart_send_string(" OvrT: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.overheat));

    VERBOSE_MSG_MACHINE(usart_send_string(" NOCAN: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.no_canbus));*/
}
 
/**
* @brief prints the error flags
*/
inline void print_control(void)
{/*
    VERBOSE_MSG_MACHINE(usart_send_string("D: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.D_raw));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.D_raw_target));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.D));
    
    VERBOSE_MSG_MACHINE(usart_send_string(" I: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.I_raw));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.I_raw_target));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.I));

    VERBOSE_MSG_MACHINE(usart_send_string(" V: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.V));

    VERBOSE_MSG_MACHINE(usart_send_string(" R: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.R));

    VERBOSE_MSG_MACHINE(usart_send_string(" T: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.T));*/
} 

/**
 * @brief Checks if the system is OK to run:
 *  - all ring_buffers needed to be full
 *  - checks the current
 *  - checks the voltage
 *  - checks the temperature
 *
 */
inline void task_initializing(void)
{
    set_led();
	timestamp = 0;
    //set_pwm_off();
    //pwm_fault_count = 0;

    //check_buffers();
    //check_idle_current();
    //check_idle_voltage();
    //check_idle_temperature();
       
    if(!error_flags.all){
        //VERBOSE_MSG_INIT(usart_send_string("System initialized without errors.\n"));
        set_state_idle();
    }
    else{
        //VERBOSE_MSG_ERROR(usart_send_string("Sorry. I have have found errors in the initialilation process. \n\nI will begin to process it...\n"));
        set_state_error();
    }
}

/**
 * @brief waits for commands while checking the system:
 *  - checks the deadman's switch state
 *  - checks the on_off_switch state
 *  - checks the potentiometer state
 *  then if its ok, enable the system to operate
 */
inline void task_idle(void)
{
    if(led_clk_div++ >= 50){
        cpl_led();
        led_clk_div = 0;
    }

    //VERBOSE_MSG_MACHINE(usart_send_string("Enjoy, the system is at its RUNNING STATE!!\n"));
    set_state_running();
}


/**
 * @brief running task checks the system and apply the control action to pwm.
 */
inline void task_running(void)
{
    if(led_clk_div++ >= 10){
        cpl_led();
        led_clk_div = 0;
    }

    //!<LÊ os ADCs's
    check_running_OutCurrent();
	check_running_InCurrent();
    check_running_BatteryTemperature();
    check_running_MotorVoltage();
	check_running_MotorTemperature();
	check_running_BatteryVoltage();
	
	//!<Envia os dados para o bluetooth	
	bt_send_package(); //!< Envia dados para o bluetooth


    /*if(system_flags.mppt_on && system_flags.enable){
        //pwm_compute();
    }else{
        //pwm_reset();
        set_state_idle();
    }*/

}

/**
 * @brief error task checks the system and tries to medicine it.
 */
inline void task_error(void)
{
    if(led_clk_div++ >= 5){
        cpl_led();
        led_clk_div = 0;
    }



    total_errors++;         // incrementa a contagem de erros
    /*VERBOSE_MSG_ERROR(usart_send_string("The error code is: "));
    VERBOSE_MSG_ERROR(usart_send_uint16(error_flags.all));
    VERBOSE_MSG_ERROR(usart_send_char('\n'));

    if(error_flags.overcurrent)
        VERBOSE_MSG_ERROR(usart_send_string("\t - Motor over-current!\n"));
    if(error_flags.overvoltage)
        VERBOSE_MSG_ERROR(usart_send_string("\t - Motor over-voltage!\n"));
    if(error_flags.overheat)
        VERBOSE_MSG_ERROR(usart_send_string("\t - Motor over-heat!\n"));
    if(error_flags.fault)
        VERBOSE_MSG_ERROR(usart_send_string("\t - IR2127 FAULT!\n"));
    if(error_flags.no_canbus)
        VERBOSE_MSG_ERROR(usart_send_string("\t - No canbus communication with MIC17!\n"));
    if(!error_flags.all)
        VERBOSE_MSG_ERROR(usart_send_string("\t - Oh no, it was some unknown error.\n"));
 
    VERBOSE_MSG_ERROR(usart_send_string("The error level is: "));
    VERBOSE_MSG_ERROR(usart_send_uint16(total_errors));
    VERBOSE_MSG_ERROR(usart_send_char('\n'));
    
    if(total_errors < 2){
        VERBOSE_MSG_ERROR(usart_send_string("I will reset the machine state.\n"));
    }*/
    if(total_errors >= 20){
        //VERBOSE_MSG_ERROR(usart_send_string("The watchdog will reset the whole system.\n"));
        for(;;);    // waits the watchdog to reset.
    }
    
    cpl_led();
    set_state_initializing();
}


/**
 * @brief this is the machine state itself.
 */
inline void machine_run(void)
{
	#ifdef CAN_ON
    can_app_task();
	#endif
   // print_system_flags();
   // print_error_flags();
   // print_control();

    if(machine_clk){
        machine_clk = 0;
        switch(state_machine){
            case STATE_INITIALIZING:
                task_initializing();

                break;
            case STATE_IDLE:
                task_idle();

                break;
            case STATE_RUNNING:
                task_running();

                break;
            case STATE_ERROR:
                task_error();

            default:
                break;
        }
    }
}

/**
 * @brief Interrupcao das chaves: se alguma chave desligar, o motor desliga.
 */


/**
* @brief ISR para ações de controle
*/
ISR(TIMER2_COMPA_vect)
{
    //VERBOSE_MSG_ERROR(if(machine_clk) usart_send_string("\nERROR: CLOCK CONFLICT!!!\n"));
	machine_clk = 1;
}

