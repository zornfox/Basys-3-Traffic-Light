/**************************************************************************************************************************
 ******** Name: Keyi Liu
 ******** Student ID: s1703084
 ******** Lab Group: Wednesdays
 **************************************************************************************************************************/

/**************************************************************************************************************************
 * Traffic Light project
 **************************************************************************************************************************
 * functionalities achieved under this project :
 *
 * 1. Use VGA_REDION 0-2 to simulate lights: RED, AMBER, GREEN on road-1 of T-Junction road
 * 2. Use VGA_REDION 6-8 to simulate lights: RED, AMBER, GREEN on  road-2 of T-Junction road
 * 3. Use VGA_REGION 4 to represent pedestrian light: either RED or GREEN
 *
 * 4. Use LED L1, P1, N3 to demonstrate RED, AMBER, GREEN respectively on road 1
 * 5. Use LED P3, U3, W3 to demonstrate RED, AMBER, GREEN respectively on road 2
 * 6. use indicator LED U16 to represent if the pedestrian is pressed
 *
 * 7. 7-segment can be used as countDown timer, to display the time duration of each state
 * ***time duration of the state in basic operations is 3 seconds
 * ***time duration of the pedestrian state is 3 seconds
 *
 * 8. basic operations of each road are followed UK traffic light standard which are R-RY-G-Y-R
 * 9. the pedestrian light turns to GREEN only when the pedestrian button is triggered and
 *    the basic operations finish on road-2
 *
 * 10. the blink will occur at the last remaining 2 seconds in the pedestrian state
 *
 *****************************************************************************************************************



/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */
/**************************************************************************************************************************
 *                                                Include Files
 **************************************************************************************************************************/
#include <stdio.h>
#include "platform.h"     // contains function init_platform() and cleanup_platform
#include "gpio_init.h"    // the device I have used from BASYS3
#include "seg7_display.h" // display decimal digits on 7-segment display
#include "definition.h"   // the colour constants I used in this project

/**************************************************************************************************************************
 *                              function will be called in this project, has been defined
 **************************************************************************************************************************/
void print(char *str);      // allow to print a string using recursion
int setUpInterruptSystem(); // Set ISR, has been defined in xinterruptES3.c
void hwTimerISR(void *CallbackRef); // Interrupt handler function, has been declared in xinterruptES3.c


/**************************************************************************************************************************
 *                             define and Initialise all the variables will be used
 **************************************************************************************************************************/
volatile int interruptCount=0; //initialise the interruptCount, giving the interrupt counter to 0 at first
u16 wait=FALSE; // flag to show if the pedestrian button has been pressed or not,
u16 time = 3;   // declare time, giving it to 3s represents duration of different states in Basic operations
u16 state = 0;  // declare state is used as variable in switch-case later, give state = 0 at first;
u16 blink = 0x0000;   // declare the blink and initialise to 0x0000, aim to use bitwise OR to compare with GREEN


/**************************************************************************************************************************
 *                            ISR function as traffic light controller, parallel working with main function
 **************************************************************************************************************************/
void hwTimerISR(void *CallbackRef){
	displayDigit(); // to determine display which digit on the 7-segment display
	interruptCount++; // the hardware timer is set give interrupt every 0.004s, it means every 0.004s, interruptCount+1

	if(interruptCount == 250){
		//every second finish, to reset interruptCount to 0 and decrement time by minus 1
		time--;
		interruptCount = 0;
		if(time == 0) // after run out the time duration of each state, going to the if_statement
		{
			time = 3; // reset the time to 3 for basic operations
			if(state == 8)// when the sequence reach 8
			{
				// check if the pedestrian button is pressed or not
				if(wait == FALSE)
					state = 0;    // if not, restart the sequence cycle
				else
				{
					//if the pedestrian button has been triggered at some point during state 0-8
					wait = FALSE; // reset the flag to wait to false
					time = 6;     // assign time duration to 6s in the pedestrian state
				}
			}

			state++;  //every time time=0, the state+1 to move to the next state

		}
	}



}
/**************************************************************************************************************************
 *                                    Main function
 **************************************************************************************************************************/
int main()
{
    init_platform();

/**************************************************************************************************************************
*                                 check the initialisation of GPIOs and interrupt System
**************************************************************************************************************************/
    int status;
    // Initialise the GPIOs
        status = initGpio();
   /***Check if the general-purpose IOs in the hardware are successful initialised. If not, the program is terminated.******/
        if (status != XST_SUCCESS) {
            print("GPIOs initialisation failed!\n\r");
            cleanup_platform();
            return 0;
        }
    // Setup the Interrupt System
        status = setUpInterruptSystem();
    /***Check if the interrupt system has been set up. If not, the program is terminated.******/
        if (status != XST_SUCCESS) {
            print("Interrupt system setup failed!\n\r");
            cleanup_platform();
            return 0;
        }

/**************************************************************************************************************************
*                                      while(1) loop
**************************************************************************************************************************/

        while(1)
        {
         /***Check if the pedestrian button is pressed or not, using the device BTNR on the FPGA ******/
        	if(XGpio_DiscreteRead(&P_BTN_RIGHT, 1) )
        		wait = TRUE; // if BTNR is pressed, give the wait to TRUE


        	switch(state){
        /***case 0 represent Road-1 RED and Road-2 RED with pedestrian light RED ***********************/
        		case 0:
        			XGpio_DiscreteWrite(&REGION_0_COLOUR, 1, RED);
        			XGpio_DiscreteWrite(&REGION_6_COLOUR, 1, RED);
        			XGpio_DiscreteWrite(&REGION_4_COLOUR, 1, RED);
       /***assign other VGA regions to WHITE, aim to recover the the colour at this state before ********/
        			XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, WHITE);
        			XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, WHITE);
        			XGpio_DiscreteWrite(&REGION_7_COLOUR, 1, WHITE);
        			XGpio_DiscreteWrite(&REGION_8_COLOUR, 1, WHITE);
       /***assign the value to the LEDS to illuminate L1, P3***********************************************/
       /***If the pedestrian button is pressed at this state, illuminate U16*******************************/
        			XGpio_DiscreteWrite(&LED_OUT, 1, 0x9000 + wait);
        			break;
        		case 1:
       /***case 1 represent Road-1 RED & YELLOW and Road-2 RED with pedestrian light RED ******************/
        		    XGpio_DiscreteWrite(&REGION_0_COLOUR, 1, RED);
        		    XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, YELLOW);
        		    XGpio_DiscreteWrite(&REGION_6_COLOUR, 1, RED);
        		    XGpio_DiscreteWrite(&REGION_4_COLOUR, 1, RED);
      /***assign other VGA regions to WHITE, aim to recover the the colour at this state before ***********/
        		    XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_7_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_8_COLOUR, 1, WHITE);
	  /***assign the value to the LEDS to illuminate L1, P1 and P3*****************************************/
	  /***If the pedestrian button is pressed at this state, illuminate U16********************************/
					XGpio_DiscreteWrite(&LED_OUT, 1, 0xd000 + wait);
        		    break;
        		case 2:
     /***case 2 represent Road-1 GREEN and Road-2 RED with pedestrian light RED **************************/
        		    XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, GREEN);
        		    XGpio_DiscreteWrite(&REGION_6_COLOUR, 1, RED);
        		    XGpio_DiscreteWrite(&REGION_4_COLOUR, 1, RED);
     /***assign other VGA regions to WHITE, aim to recover the the colour at this state before ************/
        		    XGpio_DiscreteWrite(&REGION_0_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, WHITE);
				    XGpio_DiscreteWrite(&REGION_7_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_8_COLOUR, 1, WHITE);
	/***assign the value to the LEDS to illuminate N3 and P3***********************************************/
	/***If the pedestrian button is pressed at this state, illuminate U16**********************************/
					XGpio_DiscreteWrite(&LED_OUT, 1, 0x3000 + wait);
        		    break;

        		case 3:
   /***case 3 represent Road-1 YELLOW and Road-2 RED with pedestrian light RED ****************************/
        		    XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, YELLOW);
        		    XGpio_DiscreteWrite(&REGION_6_COLOUR, 1, RED);
        		    XGpio_DiscreteWrite(&REGION_4_COLOUR, 1, RED);
   /***assign other VGA regions to WHITE, aim to recover the the colour at this state before **************/
        		    XGpio_DiscreteWrite(&REGION_0_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, WHITE);
				    XGpio_DiscreteWrite(&REGION_7_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_8_COLOUR, 1, WHITE);
  /***assign the value to the LEDS to illuminate P1 and P3*************************************************/
  /***If the pedestrian button is pressed at this state, illuminate U16************************************/
					XGpio_DiscreteWrite(&LED_OUT, 1, 0x5000 + wait);
        		    break;
        		case 4:
  /***case 4 represent Road-1 RED and Road-2 RED with pedestrian light RED *******************************/
        		    XGpio_DiscreteWrite(&REGION_0_COLOUR, 1, RED);
        		    XGpio_DiscreteWrite(&REGION_6_COLOUR, 1, RED);
        		    XGpio_DiscreteWrite(&REGION_4_COLOUR, 1, RED);
  /***assign other VGA regions to WHITE, aim to recover the the colour at this state before **************/
        		    XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_7_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_8_COLOUR, 1, WHITE);
 /***assign the value to the LEDS to illuminate L1, P3****************************************************/
 /***If the pedestrian button is pressed at this state, illuminate U16***********************************/
					XGpio_DiscreteWrite(&LED_OUT, 1, 0x9000 + wait);
        		    break;
        		case 5:
 /***case 5 represent Road-1 RED and Road-2 RED & YELLOW with pedestrian light RED **********************/
        		    XGpio_DiscreteWrite(&REGION_0_COLOUR,1, RED);
        		    XGpio_DiscreteWrite(&REGION_6_COLOUR,1, RED);
        		    XGpio_DiscreteWrite(&REGION_7_COLOUR,1, YELLOW);
        		    XGpio_DiscreteWrite(&REGION_4_COLOUR,1, RED);
 /***assign other VGA regions to WHITE, aim to recover the the colour at this state before **************/
        		    XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_8_COLOUR, 1, WHITE);
 /***assign the value to the LEDS to illuminate L1, P3 and U3********************************************/
 /***If the pedestrian button is pressed at this state, illuminate U16***********************************/
					XGpio_DiscreteWrite(&LED_OUT, 1, 0x9800 + wait);
        		    break;
        		case 6:
 /***case 6 represent Road-1 RED and Road-2 GREEN with pedestrian light RED *****************************/
        		    XGpio_DiscreteWrite(&REGION_0_COLOUR,1, RED);
        		    XGpio_DiscreteWrite(&REGION_8_COLOUR,1, GREEN);
        		    XGpio_DiscreteWrite(&REGION_4_COLOUR,1, RED);
 /***assign other VGA regions to WHITE, aim to recover the the colour at this state before **************/
        		    XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_6_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_7_COLOUR, 1, WHITE);
 /***assign the value to the LEDS to illuminate L1, W3***************************************************/
 /***If the pedestrian button is pressed at this state, illuminate U16***********************************/
					XGpio_DiscreteWrite(&LED_OUT, 1, 0x8400 + wait);
        		    break;
        		case 7:
 /***case 7 represent Road-1 RED and Road-2 YELLOW with pedestrian light RED *****************************/
        		    XGpio_DiscreteWrite(&REGION_0_COLOUR,1, RED);
        		    XGpio_DiscreteWrite(&REGION_7_COLOUR,1, YELLOW);
        		    XGpio_DiscreteWrite(&REGION_4_COLOUR,1, RED);
 /***assign other VGA regions to WHITE, aim to recover the the colour at this state before **************/
        		    XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_6_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_8_COLOUR, 1, WHITE);
 /***assign the value to the LEDS to illuminate L1, U3***************************************************/
 /***If the pedestrian button is pressed at this state, illuminate U16***********************************/
					XGpio_DiscreteWrite(&LED_OUT, 1, 0x8800 + wait);
        		    break;
        		case 8:
 /***case 0 represent Road-1 RED and Road-2 RED with pedestrian light RED *******************************/
        		    XGpio_DiscreteWrite(&REGION_0_COLOUR, 1, RED);
        		    XGpio_DiscreteWrite(&REGION_6_COLOUR, 1, RED);
        		    XGpio_DiscreteWrite(&REGION_4_COLOUR, 1, RED);
 /***assign other VGA regions to WHITE, aim to recover the the colour at this state before **************/
        		    XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_7_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_8_COLOUR, 1, WHITE);
 /***assign the value to the LEDS to illuminate L1, P3***************************************************/
 /***If the pedestrian button is pressed at this state, illuminate U16***********************************/
					XGpio_DiscreteWrite(&LED_OUT, 1, 0x9000 + wait);
        		    break;
        		case 9:
 /***case 9 represent pedestrian state: Road-1 RED and Road-2 RED, pedestrians can cross the road ********/

        	       if(time <= 2)// at last remaining 2 seconds, blink 5 times
        			{
        				if(interruptCount % 50 == 0)// 2s = 500 interrupts in total
        					blink = ~blink; // every 50 interrupts, reverse the blink by using bitwise-NOT
        			}
        		    XGpio_DiscreteWrite(&REGION_0_COLOUR,1, RED);
        		    XGpio_DiscreteWrite(&REGION_6_COLOUR,1, RED);
        		    // using bitwise-OR to assign the colour: white or green
        		    XGpio_DiscreteWrite(&REGION_4_COLOUR,1, GREEN | blink);

        		    XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, WHITE);
        		    XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_7_COLOUR, 1, WHITE);
					XGpio_DiscreteWrite(&REGION_8_COLOUR, 1, WHITE);

					XGpio_DiscreteWrite(&LED_OUT, 1, 0x9000); // at this state, the LED indicator of pedestrian button should go off
        		    break;

        		default:
        			state = 0;// define the default case to state=0
        	}
        	displayNumber(time);// display the time in while(1) loop to let 7-segment keep working
        }

    cleanup_platform();
    return 0;
}



