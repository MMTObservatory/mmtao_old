
#include "pcrerror.h"


int ErrorTableLen = sizeof(ErrorTable)/sizeof(ErrorList);

static char *retcode = "undefined ?";

char *whaterror(int errcode) 
{
    char *pt = retcode;
    int list;

    errcode=(errcode>0)?PCR_OK:errcode; // if it is positive, then assume PCR_OK
    for(list=0;list<ErrorTableLen;list++) {
	if(ErrorTable[list].pcr_code == errcode) {
	    pt=ErrorTable[list].errmsg;
	    break;
	}
    }
    return retcode;
}