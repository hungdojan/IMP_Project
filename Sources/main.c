/**
 * @brief Distance measurement using ultrasonic sensor SRF05.
 *
 * This program measures distance between object and ultrasonic sensor
 * and displays it onto 7-segment display. Both sensor and display are
 * connected to programmable microcontroller MK60.
 *
 * This source code serves as submission for a project
 * of class IMP at FIT, BUT 2022/23
 *
 * @author Hung Do
 * @file main.c
 * @date 12/08/2022
 */
#include "MK60DZ10.h"
#include <stdint.h>

#define C1 0x8C0u
#define C2 0xA80u
#define C3 0x2C0u
#define C4 0xA40u

#define C_ALL 0xAC0u
#define SIGNAL_MASK 0x4000000u
#define DEC_POINT_D 0x4000

#define SIGNAL_TRIG_DELAY 0x1F3
#define SIGNAL_ECHO_DELAY 0x16E35
// value must be greater than 50ms for optimal measuring
#define MEASUREMENT_DELAY 0x4C4B3F  // 100ms between measurements

#define DIGIT_1(c_pos) \
    do { \
        PTD->PDOR = GPIO_PDOR_PDO(0x100); \
        PTA->PDOR = GPIO_PDOR_PDO(0x100 | c_pos); \
    } while (0)

#define DIGIT_2(c_pos) \
    do { \
        PTD->PDOR = GPIO_PDOR_PDO(0xB100); \
        PTA->PDOR = GPIO_PDOR_PDO(0x400 | c_pos); \
    } while (0)

#define DIGIT_3(c_pos) \
    do { \
        PTD->PDOR = GPIO_PDOR_PDO(0x9100); \
        PTA->PDOR = GPIO_PDOR_PDO(0x500 | c_pos); \
    } while (0)

#define DIGIT_4(c_pos) \
    do { \
        PTD->PDOR = GPIO_PDOR_PDO(0x300); \
        PTA->PDOR = GPIO_PDOR_PDO(0x500 | c_pos); \
    } while (0)

#define DIGIT_5(c_pos) \
    do { \
        PTD->PDOR = GPIO_PDOR_PDO(0x9200); \
        PTA->PDOR = GPIO_PDOR_PDO(0x500 | c_pos); \
    } while (0)

#define DIGIT_6(c_pos) \
    do { \
        PTD->PDOR = GPIO_PDOR_PDO(0xB200); \
        PTA->PDOR = GPIO_PDOR_PDO(c_pos | 0x500); \
    } while (0)

#define DIGIT_7(c_pos) \
    do { \
        PTD->PDOR = GPIO_PDOR_PDO(0x1100); \
        PTA->PDOR = GPIO_PDOR_PDO(c_pos | 0x100); \
    } while (0)

#define DIGIT_8(c_pos) \
    do { \
        PTD->PDOR = GPIO_PDOR_PDO(0xB300); \
        PTA->PDOR = GPIO_PDOR_PDO(c_pos | 0x500); \
    } while (0)

#define DIGIT_9(c_pos) \
    do { \
        PTD->PDOR = GPIO_PDOR_PDO(0x9300); \
        PTA->PDOR = GPIO_PDOR_PDO(c_pos | 0x500); \
    } while (0)

#define DIGIT_0(c_pos) \
    do { \
    	PTD->PDOR = GPIO_PDOR_PDO(0xB300); \
        PTA->PDOR = GPIO_PDOR_PDO(c_pos | 0x100); \
    } while (0)

#define DIGIT_OFF(c_pos) \
	do { \
		PTD->PDOR = GPIO_PDOR_PDO(0); \
        PTA->PDOR = GPIO_PDOR_PDO(c_pos | 0); \
    } while (0)

// distance between object/obstacle and sensor
float distance = 0.0f;

/**
 * @brief Custom delay.
 *
 * @param bound Number of ticks in bounds.
 */
void delay(long long bound) {
	for (long long i = 0; i < bound; i++);
}

/**
 * @brief Ports initialization.
 *
 * Function initializes all ports needed for project.
 * Project uses GPIO pins of ports A and D. Also pin PTA24 is
 * used for reading ultrasonic sound waves. So interrupt flags
 * for this pin are set.
 */
void PORT_init() {
	SIM->SCGC5 = (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK);  // Turn on clocks for PORTA and PORTD

	// set GPIO pins of ports A and D for 7-segment display
	PORTD->PCR[8] = (0 | PORT_PCR_MUX(0x1));   // 17  7B
	PORTD->PCR[9] = (0 | PORT_PCR_MUX(0x1));   // 18  10F
	PORTD->PCR[12] = (0 | PORT_PCR_MUX(0x1));  // 19  11A
	PORTD->PCR[13] = (0 | PORT_PCR_MUX(0x1));  // 20  1E
	PORTD->PCR[14] = (0 | PORT_PCR_MUX(0x1));  // 21  2DP
	PORTD->PCR[15] = (0 | PORT_PCR_MUX(0x1));  // 22  3D

	PORTA->PCR[8] = (0 | PORT_PCR_MUX(0x1));   // 23 4C
	PORTA->PCR[10] = (0 | PORT_PCR_MUX(0x1));  // 24 5G
	PORTA->PCR[6] = (0 | PORT_PCR_MUX(0x1));   // 25 9C2
	PORTA->PCR[11] = (0 | PORT_PCR_MUX(0x1));  // 26 8C3
	PORTA->PCR[7] = (0 | PORT_PCR_MUX(0x1));   // 27 6C4
	PORTA->PCR[9] = (0 | PORT_PCR_MUX(0x1));   // 28 12C1

	// setup GPIO pins for ultrasonic sensor
    PORTA->PCR[26] = (0 | PORT_PCR_MUX(0x01));  // TRIG 37
    PORTA->PCR[24] = (0 | PORT_PCR_ISF_MASK  |
    					  PORT_PCR_MUX(0x01) |
    					  PORT_PCR_IRQC(0xB) |
						  PORT_PCR_DSE_MASK);   // ECHO 39

    // setup port directions and default values
	PTA->PDDR = GPIO_PDDR_PDD(0x4000FC0);
	PTA->PDOR = GPIO_PDOR_PDO(0xAC0);
	PTD->PDDR = GPIO_PDDR_PDD(0xF300);
	PTD->PDOR = GPIO_PDOR_PDO(0x0);

	NVIC_ClearPendingIRQ(PORTA_IRQn);  // echo interrupt
	NVIC_EnableIRQ(PORTA_IRQn);
}

/**
 * @brief Initialize PIT module.
 *
 * PIT module is used by ultrasonic sensor. This project uses 3 channels.
 * TRIG signal is generated by at least 10ms so first PIT channel is set up for that.
 * Second PIT channel defines delay between measurements. And third channel allows
 * program to calculate distance of obstacle via ECHO port.
 */
void PIT_init() {
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;  // PIT clock enable

	PIT->MCR = 0;
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(SIGNAL_TRIG_DELAY); // 500 cycles for 10us signal
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;              // enable interrupt

	PIT->CHANNEL[1].LDVAL = PIT_LDVAL_TSV(SIGNAL_ECHO_DELAY); // max echo timer value

	PIT->CHANNEL[2].LDVAL = PIT_LDVAL_TSV(MEASUREMENT_DELAY); // delay between measurements
	PIT->CHANNEL[2].TCTRL |= PIT_TCTRL_TIE_MASK;              // enable interrupt

	// setup interrupts
	NVIC_ClearPendingIRQ(PIT0_IRQn);   // timer to generate ultrasonic signal
	NVIC_ClearPendingIRQ(PIT2_IRQn);   // timer of pause between measurements
	NVIC_EnableIRQ(PIT0_IRQn);
	NVIC_EnableIRQ(PIT2_IRQn);
}

/**
 * @brief Display one digit on the 7-segment display.
 *
 * @param digit 	 Select value of the digit.
 * @param digit_pos  Position of digit on the display.
 */
void set_digit(char digit, unsigned char digit_pos) {
	uint32_t pos;
	// get position of digit base on given parameter
	switch(digit_pos) {
		case 1:  { pos = C1; break; }
		case 2:  { pos = C2; break; }
		case 3:  { pos = C3; break; }
		case 4:  { pos = C4; break; }
		default: { pos = C1; break; }
	}
	PTA->PSOR = GPIO_PSOR_PTSO(C_ALL);
	// display selected value on display
	switch(digit) {
		case 1:  { DIGIT_1(pos);   break; }
		case 2:  { DIGIT_2(pos);   break; }
		case 3:  { DIGIT_3(pos);   break; }
		case 4:  { DIGIT_4(pos);   break; }
		case 5:  { DIGIT_5(pos);   break; }
		case 6:  { DIGIT_6(pos);   break; }
		case 7:  { DIGIT_7(pos);   break; }
		case 8:  { DIGIT_8(pos);   break; }
		case 9:  { DIGIT_9(pos);   break; }
		case 0:  { DIGIT_0(pos);   break; }
		default: { DIGIT_OFF(pos); break; }
	}
}

/**
 * @brief Display distance on 7-segment display.
 *
 * Display will show only one digit after decimal point.
 * Max value that this display can show is 999.9.
 */
void update_display() {
	char c1=-1, c2=-1, c3=-1, c4=-1;
	if (distance >= 1000.0f) {
		// display distance greater than 1000cm
		c1 = 9;
		c2 = 9;
		c3 = 9;
		c4 = 9;
	} else if (distance >= 100.0f) {
		// display distance in range 100cm to 1000cm
		c1 = ((int)(distance / 100)) % 10;
		c2 = ((int)(distance / 10)) % 10;
		c3 = (int)distance % 10;
		c4 = ((int)(distance * 10)) % 10;
	} else if (distance >= 10.0f) {
		// display distance in range 10cm to 100cm
		c2 = ((int)(distance / 10)) % 10;
		c3 = (int)distance % 10;
		c4 = ((int)(distance * 10)) % 10;
	} else {
		// display distance in range 0cm to 10cm
		c3 = (int)distance % 10;
		c4 = ((int)(distance * 10)) % 10;
	}

	// turn on segments
	set_digit(c4, 4);
	delay(100);
	set_digit(c3, 3);
	PTD->PSOR = GPIO_PSOR_PTSO(DEC_POINT_D);
	delay(100);
	set_digit(c2, 2);
	delay(100);
	set_digit(c1, 1);
	delay(100);
}

/**
 * @brief Start generating ultrasonic signal from TRIG pin.
 */
void start_ultrasonic() {
	PTA->PSOR = GPIO_PDOR_PDO(SIGNAL_MASK);         // start generating signal
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;    // enable timer
}

/**
 * @brief First PIT channel interrupt handler (TRIG signal).
 */
void PIT0_IRQHandler(void) {
	// turn off signal
	PTA->PCOR = GPIO_PCOR_PTCO(SIGNAL_MASK);

	// clear interrupt flags and reset timer
	PIT->CHANNEL[0].TFLG = 0x01;
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_TIE_MASK;
}
/**
 * @brief Third PIT channel interrupt handler (delay between measurements).
 */
void PIT2_IRQHandler(void) {
	// clear interrupt flags and reset timer
	PIT->CHANNEL[2].TFLG = 0x01;
	PIT->CHANNEL[2].TCTRL &= PIT_TCTRL_TIE_MASK;
	start_ultrasonic();
	PIT->CHANNEL[2].TCTRL |= PIT_TCTRL_TEN_MASK;
}

/**
 * @brief PORTA interrupts handler.
 */
void PORTA_IRQHandler(void) {
	// ignore interrupts except pin PTA24
	// where ultrasonic's ECHO pin is connected
	if (PORTA->PCR[24] & PORT_PCR_ISF_MASK) {
		if (PTA->PDIR & 0x1000000) {
			// rising edge; echo start
			PIT->CHANNEL[1].TCTRL = 0;
			PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
		} else {
			// falling edge; echo end
			// get number of ticks value
			uint32_t diff = SIGNAL_ECHO_DELAY - PIT->CHANNEL[1].CVAL;
			PIT->CHANNEL[1].TCTRL = 0;
			distance = diff * 0.02 / 58;
			distance_delay = 0;
		}
	}
	// clear interrupt flags
	PORTA->ISFR = PORTA->ISFR;
}

int main(void)
{
    PORT_init();
    PIT_init();

    // MEASUREMENTS_DELAY ticks wait for trigger
    // before triggering ultrasonic sensor to generate a signal
    PIT->CHANNEL[2].TCTRL |= PIT_TCTRL_TEN_MASK;
	for (;;) {
		update_display();
	}

	return 0;
}
/* main.c */
