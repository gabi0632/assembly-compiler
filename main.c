/*
 ==============================================================================================
 Name        : count_bits.ch_fr_input
 Author      : gabriel abramovich
 Version     :
 Description : The program get an assembler code and convert it to bits
               The program will output 3 files.
               The object file will presenet the instraction in the memory by there order
                and present each word in a bits
                The program will make a file that show a external and enternal verbiel at the code
 ==============================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AssemblerManager.h"
/* argv - file names list
 * argc - number of parametrs at the cmd
*/
int main(int argc, char *argv[]){
    char **fileInputs = argv;
    /*the first argument is the compiler name*/
    fileInputs++;
    if(argc <2)
	{
		printf("No File where typed.\n");
		exit(0);
	}
    AssemblyCompailer(fileInputs,--argc);
    return 0;
}