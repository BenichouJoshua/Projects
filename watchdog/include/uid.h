#ifndef __UID_H__
#define __UID_H__

#include <sys/types.h>  /* size_t, pid_t */
#include <time.h> /* time_t */

typedef struct uid ilrd_uid_t;

struct uid
{
    size_t counter;
    time_t time;
    pid_t pid;
};

/*
 * DESCRIPTION:
 *  Generates a UID.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  none.
 *
 * RETURN:
 *  A generated uid struct.
 *  If fails, returns bad UID.
 */
ilrd_uid_t UIDCreate(void);

/*
 * DESCRIPTION:
 * Checks id two UID objects are the same.
 * Bad UIDs are all the same.
 *
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  uid1-   First UID to be compared.
 *  uid2-   Second UID to be compared.
 *
 * RETURN:
 *  0 if different, else than 0 if same.
 *
 */
int IsSameUID(ilrd_uid_t uid1, ilrd_uid_t uid2);

/*
 * DESCRIPTION:
 *  Generates an invalid UID.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  None.
 *
 * RETURN:
 *  An invalid UID object.
 *
 */
ilrd_uid_t GetBadUID(void);

#endif /* __UID_H__ */