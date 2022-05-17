#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "InsractionHandler.h"
#include "DataBuilder.h"
#define asmFileExt ".as"
#define entryFileExt ".ent"
#define externalFileExt ".ext"
#define objectFileExt ".obj"
#define MAX_FILENAME_LEN 255
#define NUMBER_OF_OUTPUT_FILES 3

char asmFileName[MAX_FILENAME_LEN];
FILE *entryFp;
FILE *externalFp;
FILE *objectFp;
FILE *asmFp;
/*get file name and file pointer and create it*/
FILE *OpenFile(FILE *file, char *fileName)
{
    strcpy(asmFileName, fileName);/*Copy the file name*/
    strcat(asmFileName, asmFileExt);/*Add the file extntion*/
    file = fopen(asmFileName, "r");
    asmFp=file;
    strcpy(asmFileName, fileName);
    return file;
}
/*Open Entry file 
* in case of sucssec return true else false*/
int openEntry()
{
    int entryFileNameLenght;        /*File lenght*/
    char *entryOutputName;          /*File full name*/
    int hasError=false;
    entryFileNameLenght = strlen(asmFileName) + strlen(entryFileExt)+1;
    entryOutputName = (char *)malloc(sizeof(char) * entryFileNameLenght);
    /*Check if entryOutputName was created and print error if not*/
    if (!entryOutputName)
    {
        fprintf(stderr, "Error Failed to allocate memory for a new string");
        exit(0);
    }
    strcpy(entryOutputName, asmFileName);   /*Copy the file name*/
    strcat(entryOutputName, entryFileExt);  /*Add the file extntion*/
    entryFp = fopen(entryOutputName, "a+");
    if (entryFp)
        hasError = true;
    else
        hasError = false;
    free(entryOutputName);
    return hasError;
}
/*Open External file 
* in case of sucssec return true else false*/
int openExternal()
{
    int externalFileNameLenght;     /*File lenght*/
    char *externalOutputName;       /*File full name*/
    int hasError = false;
        externalFileNameLenght = strlen(asmFileName) + strlen(entryFileExt) + 1;
        externalOutputName = (char *)malloc(sizeof(char) * externalFileNameLenght);
        if (!externalOutputName)
        {
            fprintf(stderr, "Error Failed to allocate memory for a new string");
            exit(0);
        }
        strcpy(externalOutputName, asmFileName);    /*Copy the file name*/
        strcat(externalOutputName, externalFileExt);/*Add the file extntion*/
        externalFp = fopen(externalOutputName, "a+");
        if (externalFp)
            hasError = true;
        else
            hasError = false;
        free(externalOutputName);
    return hasError;
}
/*Open Object file 
* in case of sucssec return true else false*/
int openObject()
{
    int objectFileNameLenght;   /*File lenght*/
    char *objectOutputName;     /*File full name*/
    int hasError=false;
    objectFileNameLenght = strlen(asmFileName) + strlen(objectFileExt) + 1;
    objectOutputName = (char *)malloc(sizeof(char) * objectFileNameLenght);
    if (!objectOutputName)
    {
        fprintf(stderr, "Error Failed to allocate memory for a new string");
        exit(0);
    }
    strcpy(objectOutputName, asmFileName);      /*Copy the file name*/
    strcat(objectOutputName, objectFileExt);    /*Add the file extntion*/
    objectFp = fopen(objectOutputName, "a+");
    if (objectFp)
        hasError = true;
    else
        hasError = false;
    free(objectOutputName);
    return hasError;
}
/*  Write Row into Externy File
*   Input the Symbol name and the place in the instraction Code
*    Output create file if not existe and write the  label name and place in memory*/
void outputEntryFile(char *symbolName, short ICNumbar)
{
    if (!openEntry())
        fprintf(stderr, "Error entry File wasn't created\n");
    else
        fprintf(entryFp, "%hd\t%s\n", ICNumbar, symbolName);/*Print to the file the place in memoru and symbol name*/
}
/*  Write Row into Object File
 *   Input the Symbol name and the place in the instraction Code
 *   Output create file if not existe and write the name and place in memory*/
void outputeExternalFile(char *symbolName, short placeInTabel)
{
    if(externalFp==NULL)
    {
        if (!openExternal())
            fprintf(stderr, "Error external File wasn't created\n");
    }
    else
        fprintf(externalFp, "%hd\t%s\n", placeInTabel, symbolName);/*Print to the file the place in memoru and symbol name*/
}
/*  Write Row into Entry File
 *   Input the Symbol name and the place in the instraction Code
 *   Output create file if not existe and write the name and place in memory*/
void outputeObjectFile(int IC, int DC)
{
    instructionCode *tmp;
    tmp = getListHead();
    if (!openObject())/*check if file was open currectle*/
        fprintf(stderr, "Error object File wasn't created\n");
    else
    {
        fprintf(objectFp, "\t%d\t%d\n", IC, DC);
        /*the loop will go unil he get the end of the instraction table*/
        for (; tmp != NULL; tmp = tmp->next)
        {
            fprintf(objectFp, "%04hd\t%03X\t", tmp->CIC, tmp->machineCode);/*print to the file the instraction*/
            /*Check the link type of the instraction*/
            if (tmp->linkType == LINK_TYPE_A)
                fprintf(objectFp, "A");
            else if (tmp->linkType == LINK_TYPE_R)
                fprintf(objectFp, "R");
            else if (tmp->linkType == LINK_TYPE_E)
                fprintf(objectFp, "E");
            fprintf(objectFp, "\n");
        }
    }
}
/* Close  all open files
 * In case of success return 1, otherwise return 0 and print error
 */
void outputeDataToObjectFile(int IC, int number)
{
    fprintf(objectFp, "%04hd\t%03X\tA", IC, number);
    fprintf(objectFp, "\n");
}
/*Close Entry file 
* in case of sucssec return true else false*/
int CloseEntryFile()
{
    int hasError;
    /* Check if the file need to be close and close it */
    if (!entryFp || (fclose(entryFp)) == 0)
    {
        entryFp = NULL;
        hasError = 0;
    }
    else
        hasError = 1;
    return !hasError;
}
/*Close External file 
* in case of sucssec return true else false*/
int CloseExternalFile()
{
    int hasError;
    /* Check if the file need to be close and close it */
    if (!externalFp || (fclose(externalFp)) == 0)
    {
        externalFp = NULL;
        hasError = 0;
    }
    else
        hasError = 1;
    return !hasError;
}
/*Close External file 
* in case of sucssec return true else false*/
int CloseObjectFile()
{
    int hasError;
    /* Check if the file need to be close and close it */
    if (!objectFp || (fclose(objectFp)) == 0)
    {
        objectFp = NULL;
        hasError = 0;
    }
    else
        hasError = 1;
    return !hasError;
}
/*Close Assemble file 
* in case of sucssec return true else false*/
int CloseAsmFile()
{
    int hasError;
    /* Check if the file need to be close and close it */
    if ((fclose(asmFp)) == 0)
        hasError = 0;
    else
        hasError = 1;
    return !hasError;
}
/*  The function call all the cile close function
    If not all files were closed return 0 else return 1 
*/
int CloseAllOpenFiles()
{

    if ((CloseEntryFile() + CloseExternalFile() + CloseObjectFile()+CloseAsmFile()) != 4)
    {
        fprintf(stderr, "Error cannot close files \n");
        return 0;
    }
    return 1;
}
/*In case of Error in the code Delet all open files
* In case of success return 1, otherwise return 0 and print error*/
void DeleteAllFiles()
{

    if (entryFp != NULL)
    {
        int entryFileNameLenght;
        char *entryOutputName;
        entryFileNameLenght = strlen(asmFileName) + strlen(entryFileExt)+1;
        entryOutputName = (char *)malloc(sizeof(char) * entryFileNameLenght);
        if (entryOutputName == NULL)
        {
            fprintf(stderr, "Error Failed to allocate memory for a new string");
            exit(0);
        }
        else
        {
            strcpy(entryOutputName, asmFileName);
            strcat(entryOutputName, entryFileExt);
            if (remove(entryOutputName))
            {
                fprintf(stderr, "Error cannot delete file %s assembly \n", entryOutputName);
            }
            free(entryOutputName);
        }
    }
    if (externalFp != NULL)
    {
        int externalFileNameLenght;
        char *externalOutputName;
        externalFileNameLenght = strlen(asmFileName) + strlen(externalFileExt)+1;
        externalOutputName = (char *)malloc(sizeof(char) * externalFileNameLenght);
        if (externalOutputName == NULL)
        {
            fprintf(stderr, "Error Failed to allocate memory for a new string");
            exit(0);
        }

        else
        {
            strcpy(externalOutputName, asmFileName);
            strcat(externalOutputName, externalFileExt);
            if (remove(externalOutputName))
            {
                fprintf(stderr, "Error cannot delete file %s assembly \n", externalOutputName);
            }
            free(externalOutputName);
        }
    }
    if (objectFp != NULL)
    {
        int objectFileNameLenght;
        char *objectOutputName;
        objectFileNameLenght = strlen(asmFileName) + strlen(objectFileExt) + 1;
        objectOutputName = (char *)malloc(sizeof(char) * objectFileNameLenght);
        if (!objectOutputName)
        {
            fprintf(stderr, "Error Failed to allocate memory for a new string");
            exit(0);
        }
        else
        {
            strcpy(objectOutputName, asmFileName);
            strcat(objectOutputName, objectFileExt);
            if (remove(objectOutputName))
            {
                fprintf(stderr, "Error cannot delete file %s assembly \n", objectOutputName);
            }
            free(objectOutputName);
        }
    }
}