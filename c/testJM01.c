
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "zowetypes.h"
#include "alloc.h"
#include "dynalloc.h"
#include "idcams.h"
#include "vsam.h"

#define JCL_RECLEN 80
#define RPL_RET_OFFSET 60

int main(int argc, char **argv)
{
    
    FILE *infile, *intrdr;
    char record[81], buffer[81];
    char ddPath1[16];

    if (argc != 3) {
        printf("Usage: testJM01  message\n");
        return 8;
    }

    DynallocDatasetName daDsn;
    DynallocMemberName daMember;

    memset(daDsn.name, ' ', sizeof(daDsn.name));
    memcpy(daDsn.name, argv[1],  strlen(argv[1]));
    memset(daMember.name, ' ', sizeof(daMember.name));
    memcpy(daMember.name, argv[2], strlen(argv[2]));
    DynallocDDName daDDname = {.name = "????????"};

    int daRC = RC_DYNALLOC_OK, daSysRC = 0, daSysRSN = 0;

    printf("dsn (%-44.44s), member(%-8.8s)\n", daDsn.name, daMember.name);

    daRC = dynallocAllocDataset(
        &daDsn,
        &daMember,
        &daDDname,
        DYNALLOC_DISP_SHR,
        DYNALLOC_ALLOC_FLAG_NO_CONVERSION | DYNALLOC_ALLOC_FLAG_NO_MOUNT,
        &daSysRC, &daSysRSN
    );


    printf("daRc ... (%d)\n", daRC);
    printf("ddname (%s)\n", daDDname.name);
    snprintf(ddPath1, sizeof(ddPath1), "DD:%8.8s", daDDname.name);    

    int rc = 0;
    char ddnameResult[9] = "         ";
    char errorBuffer[256];

    int macrfParms = 0;
    int rplParms   = 0;
    unsigned int maxlrecl       = 80;
    unsigned int maxbuffer      = maxlrecl;

    char *outACB = NULL;
    RPLCommon *rpl = NULL;

    rc = AllocIntReader("A", ddnameResult, errorBuffer);

    printf("INTRDR alloc rc ... (%d)\n", rc);
    printf("INTRDR ddname (%-8.8s)\n", ddnameResult);

    macrfParms = ACB_MACRF_ADR | ACB_MACRF_SEQ | ACB_MACRF_OUT;
    rplParms = RPL_OPTCD_ADR | RPL_OPTCD_SEQ;


    outACB = openACB(ddnameResult, ACB_MODE_OUTPUT, macrfParms, 0, rplParms, maxlrecl, maxbuffer);

    if (outACB) {
        rpl = (RPLCommon *)(outACB+RPL_COMMON_OFFSET+8);
        if (rpl) {
            if ( ( infile = fopen(ddPath1, "r" ) ) == NULL )
                printf( "Unable to open (%s)\n", ddPath1 );
            else {
                while ( fgets( buffer, sizeof( buffer ), infile ) != NULL ) {
                    memset(record, ' ', JCL_RECLEN);
                    memcpy(record, buffer, strlen( buffer ) - 1);
                    putRecord(outACB, record, JCL_RECLEN);
                    }
                char *jobID;
                jobID = (char *) rpl + RPL_RET_OFFSET;
                printf("jobId (%-8.8s)\n", jobID);
                closeACB(outACB, ACB_MODE_OUTPUT);
                fclose(infile);
                }
            }
    }

    daRC = dynallocUnallocDatasetByDDName(&daDDname, DYNALLOC_UNALLOC_FLAG_NONE,
                                        &daSysRC, &daSysRSN);
    if (daRC != RC_DYNALLOC_OK) {
        printf("Unallocate error (%d) (%-8.8s)\n", daRC, &daDDname);
        }


/*    memcpy(daDDname.name, ddnameResult, sizeof(ddnameResult)); 
    daRC = dynallocUnallocDatasetByDDName(&daDDname, DYNALLOC_UNALLOC_FLAG_NONE,
                                        &daSysRC, &daSysRSN);
    if (daRC != RC_DYNALLOC_OK) {
        printf("Unallocate error (%d) (%-8.8s)\n", daRC, &daDDname);
        }
*/

}
