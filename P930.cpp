/* PRUSS program to drive a for RTC ERROR MEASUREMENT
*  in Linux userspace by sending an interrupt.
*  written by A. R. Anasri
*/
#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <pthread.h>
#include <unistd.h>
#define PATH_COUNTS "/root/160718/countsP930"
#define PATH_BIN "/root/KWhError/P9_30/P930.bin"	///root/160718/512/P9_30/P930.bin"
#define PRU_NUM 0
#include <fstream>  
#include <iostream>
using std::cout;
using std::endl;
static void *pru0DataMemory;
static unsigned int *pru0DataMemory_int;

int  main (void)
{
   if(getuid()!=0){
      printf("You must run this program as root. Exiting.\n");
      exit(EXIT_FAILURE);
   }
   ////pthread_t thread;
   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

   // Allocate and initialize memory
   prussdrv_init ();
   prussdrv_open (PRU_EVTOUT_0);
   prussdrv_open (PRU_EVTOUT_1);

   // Map PRU's INTC
   prussdrv_pruintc_init(&pruss_intc_initdata);
///// *(pru0DataMemory_int+2)=0;///pru0DataMemory=0;/////
   // Copy data to PRU memory - different way
   prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pru0DataMemory);
   pru0DataMemory_int = (unsigned int *) pru0DataMemory;
   // Use the first 4 bytes for the number of samples
   *pru0DataMemory_int =512;//32;// 500;
   // Use the second 4 bytes for the sample delay in ms
   *(pru0DataMemory_int+1) =0;///100;   // 2 milli seconds between samples

   // Load and execute binary on PRU
   prussdrv_exec_program (PRU_NUM,PATH_BIN);// 
/*   if(pthread_create(&thread, NULL, &threadFunction, NULL)){
       printf("Failed to create thread!");
   }*/
///sleep(1);
   int n = prussdrv_pru_wait_event (PRU_EVTOUT_0);
///   printf("PRU program completed, event number %d.\n", n);
   ////printf("The data that is in memory is:\n");
   ///printf("- the number of samples used is %d.\n", *pru0DataMemory_int);
   ///printf("- the time delay used is %d.\n", *(pru0DataMemory_int+1));
   unsigned int raw_counts = *(pru0DataMemory_int+2);
   ///printf("- the rtc error measurement=s %d.\n", raw_distance);

   // raw_distance is in 10ns samples
   // distance in inches = time (ms) / 148 according to datasheet
////   float distin = ((float)raw_distance / (100 * 148));
   /////float distcm = ((float)raw_distance / (100 * 58));
   /////printf("-- A distance of %f inches (%f cm).\n", distin, distcm);
///std::ofstream ofs;
////

cout<<raw_counts/10<<endl;
/*
std::ofstream fout;
	fout.open(PATH_COUNTS);
		fout<<raw_counts<<endl;
	fout.close();

   /* Disable PRU and close memory mappings */
   prussdrv_pru_disable(PRU_NUM);
   prussdrv_exit ();
   return EXIT_SUCCESS;
}
