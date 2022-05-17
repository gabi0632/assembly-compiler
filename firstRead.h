#ifndef FirstRead_H_
#define FirstRead_H_
/* this function make the first scan file 
 * the function get the IC first address and a assembly file
 * Output - 0 in case of a failure
 * 			1 in case success   */
int first(FILE *fp, int *IC, int *DC);
#endif
