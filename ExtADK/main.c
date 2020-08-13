#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "f_extadk.h"

#define LIB "libfasooextadk.so"

int main(void)
{
    void *dlHandle = NULL;

    int(*T_DRMInterfaceInitialize)(void**, void**, void**, void**) = NULL;

    if ((dlHandle = dlopen(LIB, RTLD_LAZY)) == NULL)
    {
        printf("dlopen failed..\n");
        fputs(dlerror(), stderr);
        return -1;
    }
    
    if ((T_DRMInterfaceInitialize = dlsym(dlHandle, "DRMInterfaceInitialize")) == NULL )
    {
        printf("%s\n", dlerror());
    }
    
    DIM* pTI = NULL;
    SIM* pSI = NULL;
    FVI fOSVI;
    FVI fOFFICEVI;
    char szFilePath[256] = "c://test//test.docx";


    strcpy(fOSVI.szname, "TMAXOS");
    fOSVI.ulVersion = 1001;
    strcpy(fOFFICEVI.szname, "TOFFICE");
    fOSVI.ulVersion = 1001;

    //Initialize..
    (*T_DRMInterfaceInitialize)((void **)&pTI, (void **)&pSI, (void *)&fOSVI, (void *)&fOFFICEVI);
    //Function call..
    (unsigned long)(pTI->pfngetVersion());
  
    if((int)(pSI->pfnADKIsSecureContentByPath(szFilePath)) > 0)
    {
        printf("function call test\n");
    }
 
    dlclose(dlHandle);
    return 0;
}