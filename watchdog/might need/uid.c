/*
	Coder : Josh Benichou
	Date : 02/07/2023
	Reviewer : Shai
*/	

#include <unistd.h> /* getpid() */

#include "uid.h" /* our UID functions */ 

static size_t counter = 1;

const ilrd_uid_t g_bad_uid = {0, 0, 0};

ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t new_uid;

	new_uid.counter = counter++;

	new_uid.time = time(NULL);
	if (-1 == new_uid.time)
	{
		return GetBadUID();
	}

	new_uid.pid = getpid();

	return new_uid;

}

int IsSameUID(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	return 1 == (uid1.counter == uid2.counter && uid1.time == uid2.time  
											&& uid1.pid == uid2.pid) ? 1 : 0;
}

ilrd_uid_t GetBadUID(void)
{
	return g_bad_uid;
}
