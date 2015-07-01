#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "iopin_ioctl.h"

int main( int argc, char* argv[] )
{
   unsigned long ulValue;
   char  chBuf;
   int   iOption;
   int   fd;
   int   iRet;
   
   if( 2 != argc )
   {
      printf( "Wrong usage!\n" );
      printf( "Use:\n" );
      printf( "\t%s <file-name>\n", argv[0] );
      return -1;
   }
   
   fd = open( argv[1], O_RDWR );
   if( 0 > fd )
   {
      printf( "Error opening '%s': (%d) %s\n", argv[1], errno, strerror(errno) );
      return -1;
   }
   
   iOption = 99;
   while( 0 != iOption )
   {
      printf( "[ 1] - Write value\n" );
      printf( "[ 2] - Read value\n" );
      printf( "[ 3] - Set function\n" );
      printf( "[ 0] - Exit\n" );
      printf( "Option: " );
      fflush( stdout );
      
      scanf( "%d", &iOption );
      
      switch( iOption )
      {
         case 1:
         {
            printf( "Value = " );
            fflush( stdout );
            scanf( "\n%c", &chBuf );
            
            iRet = write( fd, &chBuf, 1 );
            if( 0 > iRet )
            {
               printf( "Write failed: (%d) %s\n", errno, strerror(errno) );
            }
            
            break;
         }
         
         case 2:
         {
            iRet = read( fd, &chBuf, 1 );
            if( 0 > iRet )
            {
               printf( "Read failed: (%d) %s\n", errno, strerror(errno) );
            }
            else
            {
               printf( "Value = %c\n", chBuf );
            }
            break;
         }
         
         case 3:
         {
            printf( "Value = " );
            fflush( stdout );
            scanf( "%lu", &ulValue );
            
            iRet = ioctl( fd, IOCTL_SET_FUNCTION, ulValue );
            if( 0 > iRet )
            {
               printf( "Ioctl failed: (%d) %s\n", errno, strerror(errno) );
            }
            
            break;
         }
      }
   }
   
   close( fd );
   
   return 0;
}
