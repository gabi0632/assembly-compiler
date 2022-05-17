#ifndef DataBuilder_H
#define DataBuilder_H

#define ARR_DATA_SIZE 4096

/*
Check if string number_str starts from number
  If yes check if after  number are only space or tab or eol
  If all conditions described above are correct, enter number to the data array
*/
int addDataToDataArray(char *number_str, int *DC, int rowNumber);
/*
enter each char of string into "data encoding" data array*
If string doesn't start and finish with quotation marks 
 */
int addStringToDataArray(char *,int *,int );
/*Write the data array into the object file*/
void writeDataToObjectFile(int fromAddress,int DC);
#endif