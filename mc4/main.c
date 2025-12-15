#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <gpib/gpib_user.h>
#include <gpib/ib.h>


//extern int ibsta,iberr,ibcnt ;
#define BELL            0x07
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "decl.h"

int controller_mc4 ;

void clean_buffer (char *buffer,int size)
{
    int i=0 ;
    while (i++<size)
        *(buffer++)='\0';
}

void error (void)

{
    printf ("Device error");
    printf ("ibsta=0x%x, iberr=Ox%x ,",ibsta,iberr);
    printf ("ibcnt=O0x%x\n", ibcnt);

}

int init_device (void)
/*Get address number, set address, set EOS to CR,clear device.*/
{
    char num[4],device[7];
    int i,v,GPIBO,eoi;
    int timeout = T300ms;
//    while (1)
//    {
//        printf ("\n\nEnter GPIB target device address [6]-");
//        gets (num);
//        if (strlen (num)==0) strcpy (num,"6"); //default is address 6/
//        i=atoi (num);            /*address between 1 & 16 */
//        if ((i>=1) && (i<=16)) break;
//        else putchar (BELL);
//    }

    //sprintf (device, "DEV%s", num);
//    if ((GPIBO = ibfind ("GPIBO"))<0)
//    {
//        error ();
//        return (0);
//    }

    v=REOS |'\n'; /*finish character reading with LF */
    eoi=0;
    controller_mc4 = ibdev(0, 6, 0,    /* obtain device descriptor    */
             0, eoi, v);

    if (controller_mc4<0)
    {
        error ();
        return(0);
    }

    //v=REOS |'\n'; /*finish character reading with LF */
    //ibeos (controller_mc4,v);
    ibsic (controller_mc4);
    ibtmo (controller_mc4,0);
    printf ("Type READ or hit Enter to read status\n\n\n");
    return(1);
}

void write_device (int device,char *cmde)
/* terminator for MC4 is carriage return, set S3.4 UP */
{
    char command[70];
    strcpy (command, cmde);
    strcat (command, "\r"); /*I£ $3.4 is down then this line should read streat (command, "\r\n"); */
    ibwrt (device,command,strlen(command) );
    if (ibsta & ERR) error ();
}


void read_device (int source,char *fetch)
{

    clean_buffer (fetch,71);

    ibrd (source, fetch, 70);
    //Addr4882_t a = 6;
    //Receive(0,a,fetch,70,REOS |'\n');

    if (ibsta & ERR) error ();

    printf (" %s\n", fetch);
}

void main (void)

{
    int done=0;
    char fetch[80], command[80];
    if (!init_device ()) exit(-1);
    while (!done)
    {
        printf ("Enter command - ");

        gets (command) ;

        if (!strcasecmp(command,"Q") || !strcasecmp(command,"QUIT")) done=1;

        else if (!strcasecmp(command,"")|| !strcasecmp(command, "READ"))
            read_device (controller_mc4, fetch);

        else write_device (controller_mc4,command);
    }
}
