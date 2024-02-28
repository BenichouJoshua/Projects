/****************************************************
 *  WATCHDOG                                        *   
 *                                                  *           
 *  Written by: Josh BENICHOU                       *    
 *                                                  *
 *  Reviewed by: MICHAL                             *
 *                                                  *
 *  Completed on - 31/08/23                         *
 *                                                  *
 ****************************************************/

/*************************** LIBRARY INCLUDES ******************************/

#include <stdio.h> /* printf */
#include <unistd.h> /* sleep */

/*************************** HEADER INCLUDES ******************************/

#include "watchdog.h" /*  */

/************************************ MAIN ***********************************/

int main(int argc, char *argv[])
{
	size_t index  = 0;

	WatchdogStart(argc, argv, 1, 2);
	printf("---------- PROTECTED SECTION 1 ----------\n\n");

	while(20 > index)
	{
		sleep(2);

		if (0 == index % 5)
		{

			printf("\tapp : protect me ... \n");
		}
		++index;
	}
	WatchdogStop();
	printf("\n------ END OF PROTECTED SECTION 1 ------\n\n");


	index = 0;
	while(index < 4)
	{
		printf("      app :keep your protection ! %ld\n", index);
		sleep(1);
		++index;
	}


	WatchdogStart(argc, argv, 1, 4);
	printf("---------- PROTECTED SECTION 2 ----------\n\n");

	index = 0;
	while(5 > index)
	{
		sleep(2);

		if (0 == index % 5)
		{

			printf("\tapp : protect me ... \n");
		}
		++index;
	}
	WatchdogStop();
	printf("\n------ END OF PROTECTED SECTION 2 ------\n\n");

	return 0;
}