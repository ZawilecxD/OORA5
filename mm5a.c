#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include "papi_oora.h"

int SIZE;

static double gtod_ref_time_sec = 0.0;

/* Adapted from the bl2_clock() routine in the BLIS library */

double dclock()
{
  double         the_time, norm_sec;
  struct timeval tv;
  gettimeofday( &tv, NULL );
  if ( gtod_ref_time_sec == 0.0 )
    gtod_ref_time_sec = ( double ) tv.tv_sec;
  norm_sec = ( double ) tv.tv_sec - gtod_ref_time_sec;
  the_time = norm_sec + tv.tv_usec * 1.0e-6;
  return the_time;
}

int mm(double first[][SIZE], double second[][SIZE], double multiply[][SIZE])
{
  register unsigned int i,j,k; //<- OPT 1
  register unsigned int local_size=SIZE; //<- OPT 2
  register double sum = 0;
  for (i = SIZE; i-- ; ) { //rows in multiply <- OPT 3
    for (j = SIZE; j-- ;) { //columns in multiply
      for (k = 0; k < SIZE; ) {
        if(k<SIZE-8) { //<- OPT 4
	  sum = sum + first[i][k]*second[k][j];
	  sum = sum + first[i][k+1]*second[k+1][j];
	  sum = sum + first[i][k+2]*second[k+2][j];
	  sum = sum + first[i][k+3]*second[k+3][j];
	  sum = sum + first[i][k+4]*second[k+4][j];
	  sum = sum + first[i][k+5]*second[k+5][j];
	  sum = sum + first[i][k+6]*second[k+6][j];
	  sum = sum + first[i][k+7]*second[k+7][j];
  	  k=k+8;
        }
        else {
	  sum = sum + first[i][k]*second[k][j];
	  k++;
	}
      }
      multiply[i][j] = sum;
      sum = 0;
    }
  }
  return 0;
}



int main( int argc, const char* argv[] )
{
  papi_init();
  int i,j,iret,w;
  for(w = 320;w<640;w+=64){
    SIZE = w;
  double first[SIZE][SIZE];
  double second[SIZE][SIZE];
  double multiply[SIZE][SIZE];
  double dtime;
  for (i = 0; i < SIZE; i++) { //rows in first
    for (j = 0; j < SIZE; j++) { //columns in first
      first[i][j]=i+j;
      second[i][j]=i-j;
    }
  }
  papi_start();
  iret=mm(first,second,multiply);
  papi_stop((w-320)/64);
}
  print_papi_results();

  return iret;
}