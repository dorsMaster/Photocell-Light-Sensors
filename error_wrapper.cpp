/*
 * error_wrapper.cpp
 *
 *  Created on: 16-Jan-2014
 *      Author: Nancy Minderman
 *      Description: These wrapper functions will help display the errors returned from the
 *      				operating system calls
 *
 */

#include "predef.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <basictypes.h>
#include <ucos.h>

#define OS_TIMEOUT_MESSAGE 				0
#define OS_MBOX_FULL_MESSAGE 			1
#define OS_Q_FULL_MESSAGE				2
#define OS_PRIO_EXIST_MESSAGE			3
#define OS_SEM_ERR_MESSAGE				4
#define OS_SEM_OVR_MESSAGE				5
#define OS_CRIT_ERROR_MESSAGE			6
#define OS_NO_MORE_TCP_MESSAGE 			7
#define OS_UNRECOGNIZED_ERROR_MESSAGE 	8


BYTE display_error(const char * info, BYTE error) {
	static const char * messages[] =
						{	"OS_TIMEOUT has occurred\n",
							"OS_MBOX_FULL has occurred\n",
							"OS_Q_FULL has occurred\n",
							"OS_PRIO_EXIST has occurred\n",
							"OS_SEM_ERR has occurred\n",
							"OS_SEM_OVF has occurred\n",
							"OS_CRIT_ERR has occurred\n",
							"OS_NO_MORE_TCB has occurred\n",
							"Unrecognized error\n"};

	switch (error) {
		case OS_NO_ERR:
			break; // do nothing here
		case OS_TIMEOUT:
			iprintf(info);
			iprintf(messages[OS_TIMEOUT_MESSAGE]);
			break;
		case OS_MBOX_FULL:
			iprintf(messages[OS_MBOX_FULL_MESSAGE]);
			break;
		case OS_Q_FULL:
			iprintf(messages[OS_Q_FULL_MESSAGE]);
			break;
		case OS_PRIO_EXIST:
			iprintf(messages[OS_PRIO_EXIST_MESSAGE]);
			break;
		case OS_SEM_ERR:
			iprintf(messages[OS_SEM_ERR_MESSAGE]);
			break;
		case OS_SEM_OVF:
			iprintf(messages[OS_SEM_OVR_MESSAGE]);
			break;
		case OS_CRIT_ERR:
			iprintf(messages[OS_CRIT_ERROR_MESSAGE]);
			break;
		case OS_NO_MORE_TCB:
			iprintf(messages[OS_NO_MORE_TCP_MESSAGE]);
			break;
		default:
			iprintf(messages[OS_UNRECOGNIZED_ERROR_MESSAGE]);
			iprintf("%i: %s",error, info);
	}
	return error;
}
