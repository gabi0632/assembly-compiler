#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "fileHandler.h"
#include "firstRead.h"
#include "symbolTabelHandler.h"
#include "InsractionHandler.h"
#include "secondRead.h"
#include "DataBuilder.h"
#define CODE_ADDRESS_BASE 100
#define MAX_LINE_LEN 81

/* The instructions counter */
int IC;
/* The data counter */
int DC;
/* The function get all assembler files and compile them*/
void AssemblyCompailer(char **fileList, int numberOfFiles)
{
	FILE *file = NULL;
	int hasError = false; /*Error flag*/
	while (*fileList != NULL)
	{
		/*Initialize Symbol Table*/
		InitializeSymbolTable();
		IC = CODE_ADDRESS_BASE;
		DC = 0;
		file = OpenFile(file, *fileList);/*open the current file*/
		if (file != NULL)
		{
			/* First Scan */
			if (!first(file, &IC, &DC))
			{
				/*tSet the file pointer position to the beginnig*/
				rewind(file);
				/* Update the symbol table data addresses */
				updateSymbolTabel(&IC, &DC);
				/* Second scan */
				if (!second(file, &IC))
				{	
					outputeObjectFile(IC-CODE_ADDRESS_BASE,DC);
					writeDataToObjectFile(IC,DC);
				}
				else
				{
					fprintf(stderr, "Error found on the Second scan \n");
					hasError = true;
				}
			}
			else
				hasError = true;

			/*if there was en error we delet all the files*/
			if (hasError)
			{
				fprintf(stderr, "Error unable to creat output files\n");
				DeleteAllFiles();
			}
		CloseAllOpenFiles();
		FreeSymbolTable();
		FreeInstractionTabel();
		}
		else
			fprintf(stderr, "Error unable to open file: %s\n", *fileList);
		fileList++;
	}
}
