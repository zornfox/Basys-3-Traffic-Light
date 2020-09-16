#include "gpio_init.h"

XStatus initGpio(void)
{
	XStatus status;

	/* Note that all the required Xgpio objects
	 * have been declared in the gpio_init.h header file
	 * but make sure you confirm that the IDs used to initialize
	 * the GPIOs here are correct
	 */
    status = XGpio_Initialize(&LED_OUT, 2);
    if (status != XST_SUCCESS)
    {
    	return XST_FAILURE;
    }
	status = XGpio_Initialize(&SEG7_HEX_OUT, 0);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	status = XGpio_Initialize(&SEG7_SEL_OUT, 1);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	status = XGpio_Initialize(&SLIDE_SWITCHES, 19);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}


	status = XGpio_Initialize(&P_BTN_RIGHT, 5);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}


	status = XGpio_Initialize(&REGION_0_COLOUR, 7);
    if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	status = XGpio_Initialize(&REGION_1_COLOUR, 10);
    if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	status = XGpio_Initialize(&REGION_2_COLOUR, 11);
    if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	status = XGpio_Initialize(&REGION_3_COLOUR, 12);
    if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	status = XGpio_Initialize(&REGION_4_COLOUR, 13);
    if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	status = XGpio_Initialize(&REGION_5_COLOUR, 14);
    if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	status = XGpio_Initialize(&REGION_6_COLOUR, 15);
    if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	status = XGpio_Initialize(&REGION_7_COLOUR, 16);
    if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	status = XGpio_Initialize(&REGION_8_COLOUR, 17);
    if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	status = XGpio_Initialize(&REGION_9_COLOUR, 18);
    if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	status = XGpio_Initialize(&REGION_10_COLOUR, 8);
    if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	status = XGpio_Initialize(&REGION_11_COLOUR, 9);
    if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}
