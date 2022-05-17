#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "DataBuilder.h"
#include "InsractionHandler.h"
#include "fileHandler.h"
#include "globals.h"

#define MAX_NUM 2047  /*Maximum number we can insert*/
#define MIN_NUM -2048 /*Munimum number we can insert*/
#define quoteMark '\"'

int data[ARR_DATA_SIZE]; /*Data array to store the data*/
/*
Check if string number_str starts from number
  If yes check if after  number are only space or tab or eol
  If all conditions described above are correct, enter number to the data array
*/
int addDataToDataArray(char *number_str, int *DC, int rowNumber)
{
    int hasError = false;
    if (data != NULL)
    {
        int number;
        char *endPtr;
        number = (int)strtol(number_str, &endPtr, 10); /*assign numerical value to str_num */
        /*Check number valid*/
        if (number <= MAX_NUM && number >= MIN_NUM)
        {
            /* Check if at the end of number_str is s[acespace/tab/new line symbol */
            if ((endPtr != NULL && endPtr != number_str && IsRowEmpty(endPtr)) || endPtr == NULL)
            {
                /*make the array size bigger*/
                data[*DC] = number;
                (*DC)++;
            }
        }
        /*if  end_ptr is not space/tab/new print error and return false*/
        else
        {
            fprintf(stderr, "Error in row number:%d\n", rowNumber);
            fprintf(stderr, "Error unresolved variable: %s . expecting integer\n", number_str);
            hasError = true;
        }
    }
    /*if number isn't in the range*/
    else
    {
        fprintf(stderr, "Error in row number:%d\n", rowNumber);
        fprintf(stderr, "Error the integer: %s is not between %d - %d\n", number_str, MIN_NUM, MAX_NUM);
        hasError = true;
    }
    return !hasError;
}
/*
Enter each char of string into "data encoding" data array*
If string doesn't start and finish with quotation marks 
 */
int addStringToDataArray(char *str, int *DC, int rowNumber)
{

    int hasError = false;
    if (data != NULL)
    {
        int endOfString = strlen(str) - 1;
        /* Remove trail \n */
        if (str[endOfString] == '\n')
            str[endOfString] = '\0';
        /* Check the beginning and the end is quotation marks */
        if (str[0] == quoteMark && str[strlen(str) - 1] == quoteMark)
        {
            /* Loop goes until didn't encounter on quotation marks or no more free space */
            for (str++; *str != '"'; str++, (*DC)++)
            {
                data[*DC] = *str;
            }
            (*DC)++;
            data[*DC] = str[strlen(str)];
            str = strtok(NULL, space);
            if (str != NULL)
            {
                fprintf(stderr, "Error in row number:%d\n", rowNumber);
                fprintf(stderr, "invalid characters: %s \n", str);
            }
        }
        /*Print error and return 0 if a  string doesn't start and finish with quotation marks*/
        else
        {

            fprintf(stderr, "Error in row number:%d\n", rowNumber);
            fprintf(stderr, "Error unresolved variable: %s . expecting string\n", str);
            hasError = true;
        }
    }

    return hasError;
}
/*Write the data array into the object file*/
void writeDataToObjectFile(int fromAddress, int DC)
{
    int i;
    short number;
    for (i = fromAddress; i < fromAddress + DC; i++)
    {
        number = numberToHex((short)data[i - fromAddress]);
        outputeDataToObjectFile(i, number); /*print the data array to the Object file*/
    }
}
