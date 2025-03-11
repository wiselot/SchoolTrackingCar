#ifndef _TEST_H
#define _TEST_H

/* 主要是出问题测试各个模块的,所以有些写的乱七八糟的( */

//#define LED_TEST
//#define BUZZER_TEST
//#define ADC_TEST
//#define ADC_VAL_TEST

#ifdef ADC_VAL_TEST

void adc_val_test_config(_Bool isNet);
void adc_val_test_loop();

#endif

//#define MG996R_TEST
#ifdef MG996R_TEST

void mg996r_foward10();
void mg996r_back10();

#endif
#define MOTOR_TEST
#ifdef MOTOR_TEST

void motor_test_forward();
void motor_test_back();

#endif

#define ENCODER_TEST


//#define WIFI_TEST
#ifdef WIFI_TEST

void wifi_test();

#endif

//#define TRACE_TEST

#endif