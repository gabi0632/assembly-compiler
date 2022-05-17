#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "DataBuilder.h"
#include "InsractionHandler.h"
#include "symbolTabelHandler.h"

#define ROW_MAX_LEN 81
#define LABEL_MAX_LEN 32
#define MAX_SIZE_ATTRIBUTES 9

int L;
/*  The function will handel all data type
*   Input:
*   dateType    - the type of the data
*   label       - label name
*   IC          - Instraction counter
*   rowNumber   -the row number in the assemble file
*   Output:
*               -0 in case of a failure
* 			    -1 in case success   */    
int dataHandler(char *dateType, char *label, int *IC, int *DC, int rowNumber, int hasLabel)
{
    int hasError = false;       /*Error flag*/
    char *token;                /*token from the string*/
    char *numberToken;          /*token for the numbers in the string*/
    char *externalToken;        /*token for external label*/
    /* Check what command is sent in the dateType */
    if (!strcmp(dateType, ".string"))
    {
        /* Check for label */
        if (hasLabel)
        {
            /* Insert the label to the symbol table  */
            if (addNewLabel(label, *DC, false, false, rowNumber))
                hasError = true;
        }
        token = strtok(NULL, space);
         /* In case of a string we need to save it in the data table */
        if (addStringToDataArray(token, DC, rowNumber))
        {
            fprintf(stderr, "Error in row number:%d\n", rowNumber);
            fprintf(stderr, "Error can not insert string:%s to memory \n", token);
            hasError = true;
        }
    }
    else if (!strcmp(dateType, ".data"))
    {

        /* Check for label */
        if (hasLabel)
        {
            /* insert the label to the symbol table  */
            if (addNewLabel(label, *DC, false, false, rowNumber))
                hasError = true;
        }
        /* Split all the numbers in the .data label */
        numberToken = strtok(NULL, comma);
         /* Go over the numbers and add them to the memory data array*/
        while (numberToken)
        {
             /* In case of a data we need to save it in the data table */
            if (addDataToDataArray(numberToken, DC, rowNumber))
            {
                numberToken = strtok(NULL, comma);
            }
            else
            {
                hasError = true;
                fprintf(stderr, "Error in row number:%d\n", rowNumber);
                fprintf(stderr, "Error can not insert data to memory - %s\n", numberToken);
                break;
            }
        }
    }
    else if (!strcmp(dateType, ".extern"))
    {
        /* Check for label */
        if (hasLabel)
        {
            fprintf(stderr, "Warning ignored label on extern statement in row number:%d \n", rowNumber);
        }
        /* insert the label to the symbol table  */
        externalToken = strtok(NULL, space);
        if (addNewLabel(externalToken, 0, true, false, rowNumber))
        {
            fprintf(stderr, "Error in row number:%d\n", rowNumber);
            fprintf(stderr, "Error the labels:%s is duplicate\n", label);
            hasError = true;
        }
        externalToken = strtok(NULL, space);
        if (externalToken)
        {
            fprintf(stderr, "Error in row number:%d\n", rowNumber);
            fprintf(stderr, "Error the entery is not currect\n");
            hasError = true;
        }
    }
    else if (!strcmp(dateType, ".entry"))
    {
        /* Check for label */
        if (hasLabel)
        {
            fprintf(stderr, "Warning ignored label on entry statement number:%d \n", rowNumber);
        }
    }
    else
    {
        fprintf(stderr, "Error in row number:%d\n", rowNumber);
        fprintf(stderr,"Invalid date reference\n");
        hasError = true;
    }
    return hasError;
}
/* this function make the first scan file 
 * the function get the IC first address and a assembly file
 * Output - 0 in case of a failure
 * 			1 in case success   */
int first(FILE *fp, int *IC, int *DC)
{
    char rowTemp[ROW_MAX_LEN + LABEL_MAX_LEN]; /*Variable Holad the row from the assembly file */
    char *currentRow;/*pointer to row we are parsing*/
    char *token;/*pointer to splited row*/
    char *trimmedString;/*poiner to trimmed string*/
    char currentLabel[LABEL_MAX_LEN];           /*Variabel Hold the current label*/
    int hasLabel;                               /*flag to check if the row have a label*/
    int rowNumber = 1;                          /*row number*/
    commandType currentCommandType;             /* holds current command type */
    int hasError = false;                       /*Flag Error*/
    char sourceP[ROW_MAX_LEN];                  /*Variabel Hold the source string */
    char destntionP[ROW_MAX_LEN];                /*Variabel Hold the destntion string */
    while (fgets(rowTemp, ROW_MAX_LEN + LABEL_MAX_LEN, fp) != NULL)
    {
        hasLabel = false;
        memset(currentLabel, '\0', sizeof(currentLabel));
        memset(sourceP, '\0', sizeof(sourceP));
        memset(destntionP, '\0', sizeof(destntionP));
        /* Check if the row is valid */
        if (strlen(rowTemp) > ROW_MAX_LEN)
        {
            hasError = true;
            fprintf(stderr, "Error in row number:%d\n", rowNumber);
            fprintf(stderr, "Error - The row is not valid\n");
        }
        currentRow = rowTemp;
        /* Remove all the prefix white spaces */
        while (currentRow[0] == space[0] || currentRow[0] == tab[0])
        {
            currentRow++;
        }
        /* Remove the row-break from the end of the line if exists */
        if (currentRow[strlen(currentRow) - 1] == '\n')
        {
            currentRow[strlen(currentRow) - 1] = '\0';
        }
        /* Check if the row is empty, with white spaces or a comment row */
        if (currentRow[0] == ';' || IsRowEmpty(currentRow))
        {
            rowNumber++;
            continue;
        }
        token = strtok(currentRow, space);
        if (isContainLabel(token))
        {
            hasLabel = true;
            if (!(labelHandler(token, rowNumber)))
            {
                /* Cut the label name and add null character to the end of it */
                strcpy(currentLabel, token);
                currentLabel[strlen(token) - 1] = '\0';
                token = strtok(NULL, space);
                /* Check if the token is null or is empty */
                if (!token || token[0] == '\n' || IsRowEmpty(token))
                {
                    hasError = true;
                    fprintf(stderr, "Error in row number:%d\n", rowNumber);
                    fprintf(stderr, "Error Can not be only label without a command\n");
                }
            }
            else
            {
                hasError = true;
            }
        }
        /*check is row is valid*/
        if (token != NULL)
        {
            /*check if the first char is dot (.data,.string,.extern,.entry) */
            if (token[0] == '.')
            {
                int dataError;
                dataError = dataHandler(token, currentLabel, IC, DC, rowNumber, hasLabel);
                if (dataError)
                    hasError = dataError;
            }
            else /* Instructions */
            {
                if (hasLabel)
                {
                    /* insert the label to the symbol table  */
                    if (addNewLabel(currentLabel, *IC, false, true, rowNumber))
                    {
                        hasError = true;
                    }
                }
                /*get the current command*/
                currentCommandType = GetCommandByName(token);
                /* check is the command is known  */
                if (currentCommandType.type == TypeError)
                {
                    fprintf(stderr, "Error in row number:%d\n", rowNumber);
                    fprintf(stderr, "Error unknown command: %s\n", token);
                    hasError = true;
                }
                /*In case of a command with no oprands*/
                if (currentCommandType.operandsINumber == 0)
                {
                    if (strtok(NULL, comma) != NULL)
                    {
                        fprintf(stderr, "Error in row number:%d\n", rowNumber);
                        fprintf(stderr, "Error too many parameters\n");
                        hasError = true;
                    }
                    /*Add instraction to the instraction tabel*/
                    addInstractionToTabel(&currentCommandType, *IC, 0, 0);
                    (*IC)++;
                }
                /*In case of a command with two oprands*/
                else if (currentCommandType.operandsINumber == 2)
                {
                    token = strtok(NULL, space);
                    trimmedString = trimmFunction(token, rowNumber);
                    if (trimmedString)
                    {
                        /*check if number of commas is valid*/
                        char *countCommaP; /*pointer to run the trimedCommand and for valid number of commas*/
                        int countComma = 0;
                        countCommaP = trimmedString;
                        while (*countCommaP != '\0')
                        {
                            if (*(countCommaP++) == comma[0])
                                ++countComma;
                        }
                        if (countComma > 1)
                        {
                            fprintf(stderr, "Error in row number:%d\n", rowNumber);
                            fprintf(stderr, "Error too many commas \n");
                            hasError = true;
                        }
                        /* Check if the string contains an ',' character */
                        if (strchr(trimmedString, comma[0]) != NULL)
                        {
                            char *parameter = strtok(trimmedString, comma);
                            strcpy(sourceP, parameter);
                            parameter = strtok(NULL, comma);
                            strcpy(destntionP, parameter);
                            if (strtok(NULL, comma) != NULL)
                            {
                                fprintf(stderr, "Error in row number:%d\n", rowNumber);
                                fprintf(stderr, "Error too many parameters\n");
                                hasError = true;
                            }
                        }
                        else
                        {
                            fprintf(stderr, "Error in row number:%d\n", rowNumber);
                            fprintf(stderr, "Missing operand\n");
                        }
                        /*insert the row to the instraction Tabel*/
                        if (computeInstrctionRows(currentCommandType, IC, sourceP, destntionP, rowNumber))
                            hasError = true;
                    }
                    free(trimmedString);
                }
                /* If the command gets only one operand */
                else if (currentCommandType.operandsINumber == 1)
                {
                    token = strtok(NULL, space);

                    if (token)
                    {
                        if (strchr(token, ',') == NULL)
                        {
                            strcpy(destntionP, token);
                            if (strtok(NULL, space) != NULL)
                            {
                                fprintf(stderr, "Error in row number:%d\n", rowNumber);
                                fprintf(stderr, "Error too many parameters\n");
                                hasError = true;
                            }
                        }
                        else
                        {
                            fprintf(stderr, "Error in row number:%d\n", rowNumber);
                            fprintf(stderr, "Too many commas\n");
                            hasError = true;
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Error in row number:%d\n", rowNumber);
                        fprintf(stderr, "Missing operand\n");
                        hasError = true;
                    }
                    /*insert the row to the instraction Tabel*/
                    if (computeInstrctionRows(currentCommandType, IC, NULL, destntionP, rowNumber))
                        hasError = true;
                }
            }
            rowNumber++;
        }
    }
    return hasError;
}