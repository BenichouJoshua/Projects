/****************************************************
 *  WATCHDOG INTERNAL                               *   
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
#include <stdlib.h> /* malloc free */
#include <assert.h> /* assert */
#include <string.h> /* assert */

/*************************** HEADER INCLUDES ******************************/

#include "wd.h" /* internal watchdog API*/

/************************************ MAIN ***********************************/

int main()
{
	info_t *wd_info = NULL;

	wd_info = CreateInfoStruct(WD_SEMA, APP_SEMA);

	SetWDIndicator(WATCH_DOG);
	
	InitScheduler(wd_info);
	
	return 0;
}