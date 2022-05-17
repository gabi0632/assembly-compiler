#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "globals.h"
#include "InsractionHandler.h"
#include "fileHandler.h"
#include "symbolTabelHandler.h"
/* this function make the first scan file 
 * the function get the IC first address and a assembly file
 * Output - 0 in case of a failure
 * 			1 in case success   */
int second(FILE *fp, int *IC)
{
    char rowTemp[ROW_MAX_LEN];/*Variable Holad the row from the assembly file */
    char *currentRow;/*pointer to row we are parsing*/
    int rowNumber;/*row number*/

    symbol *entrySymbol;/*Variabel to hold Entery symbol*/
    int hasError = false;/*Flag Error*/
    char *token; /*pointer to splited row*/

    rowNumber=1;
    while (fgets(rowTemp, ROW_MAX_LEN, fp) != NULL)
    {
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
            token = strtok(NULL, space);
        }
        /*row is data*/
        if (token[0] == '.')
        {
            if (!strcmp(token, ".entry"))
            {
                /* Get the symbol from the symbol table */
                token = strtok(NULL, space);
                token = trimmFunction(token, rowNumber);
                entrySymbol = findSymbol(token);
                if (entrySymbol != NULL)
                {
                    if (!entrySymbol->isExternal)
                    {
                        strcat(entrySymbol->attribute, ",entry");
                        outputEntryFile(entrySymbol->label, entrySymbol->value);
                    }
                    else
                    {
                        fprintf(stderr, "Error in row number:%d\n", rowNumber);
                        fprintf(stderr, "Error the label: %s can't be external and entry at the same time\n", token);
                        hasError = true;
                    }
                }
                else
                {
                    fprintf(stderr, "Error in row number:%d\n", rowNumber);
                    fprintf(stderr, "Error the label: %s wasn't declared\n", token);
                    hasError = true;
                }
                free(token);
            }
        }
        rowNumber++;
    }
    /*Update the instraction tabel*/
    hasError=updateInstractionTabel();
    return hasError;
}
