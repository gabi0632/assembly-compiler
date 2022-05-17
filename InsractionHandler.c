#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "globals.h"
#include "InsractionHandler.h"
#include "symbolTabelHandler.h"
#include "fileHandler.h"

#define REGISTER_OPERAND 'r'
#define REGISTER_MAX_SIZE '7'
#define OPERAND_ARRAY_MAX 4
#define MAX_NUM 2047 /*Maximum number we can insert*/
#define MIN_NUM -2048/*Mnimum number we can insert*/
/*The Commands tabels*/
commandType cmd[TABLE_SIZE] = {
    {"mov", mov, 2, 0, 0},
    {"cmp", cmp, 2, 1, 0},
    {"add", add, 2, 2, 10},
    {"sub", sub, 2, 2, 11},
    {"lea", lea, 2, 4, 0},
    {"clr", clr, 1, 5, 10},
    {"not", not, 1, 5, 11},
    {"inc", inc, 1, 5, 12},
    {"dec", dec, 1, 5, 13},
    {"jmp", jmp, 1, 9, 10},
    {"bne", bne, 1, 9, 11},
    {"jsr", jsr, 1, 9, 12},
    {"red", red, 1, 12, 0},
    {"prn", prn, 1, 13, 0},
    {"rts", rts, 0, 14, 0},
    {"stop", stop, 0, 15, 0}};

instructionCode *instructionlist;
int tabelSize;

void InitializeSInstractionTable()
{
    tabelSize = 0;
    instructionlist = (instructionCode *)malloc(sizeof(instructionCode));
    /*Check if space allocation for symbol were successful*/
    if (instructionlist == NULL)
    {
        printf("failed to allocate memory for the symbol tabel");
        exit(0);
    }
    /*set value for symbol variable*/
    instructionlist->machineCode = 0;
    instructionlist->CIC = 0;
    instructionlist->next = NULL;
}
/*return the head of the list*/
instructionCode *getListHead()
{
    return instructionlist;
}
/*  The function will handel all data type
*   Input:
*   instructionParameter    - the type of the data
*   IC          - Instraction counter
*   sourceOp       - the source operand
*   destinationOp   -the destinatio operand*/
void addInstractionToTabel(commandType *instructionParameter, long IC, OperandType sourceOp, OperandType destinationOp)
{
    /*the first argument at the list will be instraction
    no need to check for first scan or Not*/
    if (tabelSize < 1)
    {
        InitializeSInstractionTable();/*create New Instraction tabel*/
        strcpy(instructionlist->parameter, instructionParameter->instractionName);
        instructionlist->CIC = IC;
        instructionlist->linkType = LINK_TYPE_A;
        instructionlist->machineCode = instructionParameter->opCode;
        instructionlist->machineCode = (instructionlist->machineCode << 4) + instructionParameter->funct;
        instructionlist->machineCode = (instructionlist->machineCode << 2) + sourceOp;
        instructionlist->machineCode = (instructionlist->machineCode << 2) + destinationOp;
        instructionlist->next = NULL;
        tabelSize++;
    }
    else
    {
        instructionCode *tmp = instructionlist;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = (instructionCode *)malloc(sizeof(instructionCode));
        /*Check if space allocation for tree nodes were successful*/
        if (tmp->next == NULL)
        {
            printf("failed to allocate memory for the symbol tabel");
            exit(0);
        }
        strcpy(tmp->next->parameter, instructionParameter->instractionName);
        tmp->next->CIC = IC;
        tmp->next->linkType = LINK_TYPE_A;
        tmp->next->machineCode = instructionParameter->opCode;
        tmp->next->machineCode = (tmp->next->machineCode << 4) + instructionParameter->funct;
        tmp->next->machineCode = (tmp->next->machineCode << 2) + sourceOp;
        tmp->next->machineCode = (tmp->next->machineCode << 2) + destinationOp;
        tmp->next->next = NULL;
        tabelSize++;
    }
}
/*The function convert number to binary code*/
int numberToHex(short number)
{
    int i, offset;
    int count = 0;
    for (i = 11; i >= 0; i--)
    {
        offset = number >> i;
        if (offset & 1)
        {
            count = (count << 1) + 1;
        }
        else
        {
            count = count << 1;
        }
    }
    return count;
}

int addOperandToInstractionTable(OperandType operand, int IC, int rowNumber, char *parameter)
{
    instructionCode *tmp = instructionlist;
    int hasError = false;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = (instructionCode *)malloc(sizeof(instructionCode));
    /*Check if space allocation for tree nodes were successful*/
    if (tmp->next == NULL)
    {
        printf("failed to allocate memory for the symbol tabel");
        exit(0);
    }
    strcpy(tmp->next->parameter, parameter);
    tmp->next->CIC = IC;
    tmp->next->machineCode = 0;
    if (operand == Immediate)
    {
        short number;
        tmp->next->linkType = LINK_TYPE_A;
        sscanf(parameter, "#%hd", &number);
        if (number <= MAX_NUM && number >= MIN_NUM)
            tmp->next->machineCode = numberToHex(number);
        else
        {
            fprintf(stderr, "Error in row number:%d\n", rowNumber);
            fprintf(stderr, "Error the integer: %d is not between %d - %d\n", number, MIN_NUM, MAX_NUM);
            hasError = true;
        }
    }
    else
        tmp->operand = operand; /*Indicate for the update instraction the opreand type*/
    tmp->next->next = NULL;
    tabelSize++;
    return hasError;
}
/*The function go over the instraction tabel in update all the missing parts
    in case of error return true else false*/
int updateInstractionTabel()
{
    symbol *label;
    OperandType operand;
    instructionCode *tmp = instructionlist;
    int i, value;
    int hasError = false;
    for (; tmp != NULL; tmp = tmp->next)
    {
        if (!tmp->machineCode)
        {
            operand = getOperandType(tmp->parameter);
            /*Check for the operand Type*/
            if (operand == Direct)
            {
                label = findSymbol(tmp->parameter);
                if (label != NULL)
                {
                    /*If the label External the link tpye is E*/
                    if (label->isExternal)
                    {
                        outputeExternalFile(tmp->parameter, tmp->CIC);
                        tmp->linkType = LINK_TYPE_E;
                        tmp->machineCode = 0;
                    }
                    /*If the label isn't External the link tpye is R*/
                    else
                    {
                        tmp->linkType = LINK_TYPE_R;
                        tmp->machineCode = label->value;
                    }
                }
                else
                {
                    fprintf(stderr, "Error the label: %s wasn't declared\n", tmp->parameter);
                    hasError = true;
                }
            }
            else if (operand == Relative)
            {
                label = findSymbol(tmp->parameter);
                if (label != NULL)
                {
                    tmp->linkType = LINK_TYPE_A;
                    value = label->value - tmp->CIC;
                    if (value < 0)
                        tmp->machineCode = numberToHex(value);

                    else
                        tmp->machineCode = (label->value - tmp->CIC);
                }
                else
                {
                    fprintf(stderr, "Error the label: %s wasn't declared\n", tmp->parameter);
                    hasError = true;
                }
            }
            else if (operand == Register)
            {
                tmp->linkType = LINK_TYPE_A;
                for (i = 1; i <= tmp->parameter[1] - '0'; i++)
                {
                    if (!tmp->machineCode)
                        tmp->machineCode = 1;
                    tmp->machineCode = tmp->machineCode << 1;
                }
            }
        }
    }
    return hasError;
}
/*Free all the Nodes in the instraction tabel*/
void FreeInstractionTabel()
{
    instructionCode *tmp = instructionlist;
    instructionCode *current;
    while (tmp->next != NULL)
    {
        current=tmp;
        tmp = tmp->next;
        free(current);
    }
}
/*  The function check if row is Empty
*   input row from File
    Output return true is empty else false*/
int IsRowEmpty(char *row)
{
    /* Char Index */
    int i;
    int isEmpty;
    i = 0;
    isEmpty = true;
    /* strcmp returns a nonzero value if the strings are different and 0 if they're the same */
    if (strcmp(row, ""))
    {
        while (isEmpty && row && (row[i] != '\0') && (row[i] != '\n'))
            if (isspace(row[i]))
                i++;
            else
                isEmpty = false;
    }
    return isEmpty;
}
/*  Function to create one big string without white spaces and check number of commas
*   Input a string with space and the row number 
*   Outpot a proper string or Null if not by the rolles*/
char *trimmFunction(char *strCommand, int rowNumber)
{
    int hasError;
    char *trimedCommand;
    int commStartFlag;
    int commEndFlag;
    int countElemnts;
    hasError = false;
    trimedCommand = (char *)malloc(ROW_MAX_LEN);

    if (trimedCommand == NULL)
    {
        fprintf(stderr, "Error unable to allocate memory .\n");
        exit(1);
    }
    /* Insert \0 for all the string */
    memset(trimedCommand, '\0', strlen(trimedCommand));
    commStartFlag = 0;
    commEndFlag = 0;
    countElemnts = 0;
    while (strCommand != NULL && countElemnts <= 3)
    {
        if (strchr(strCommand, comma[0]) != NULL)
        {
            int commaPlace;
            int parameterLenght;
            commaPlace = (strchr(strCommand, comma[0]) - strCommand);
            parameterLenght = strlen(strCommand) - 1;
            if (commaPlace == 0)
            {
                commStartFlag = 1;
            }
            if (commaPlace == parameterLenght)
            {
                commEndFlag = 1;
            }
        }
        countElemnts++;
        strcat(trimedCommand, strCommand);
        strCommand = strtok(NULL, space);
    }
    if ((countElemnts > 1 && !commStartFlag && !commEndFlag) ||
        (countElemnts > 3) ||
        ((commEndFlag ^ commStartFlag) && countElemnts != 2))
    {
        hasError = true;
    }
    if (hasError)
    {
        free(trimedCommand);
        return NULL;
    }
    else
    {
        int commandLength = strlen(trimedCommand);
        if (trimedCommand[commandLength - 1] == '\n')
        {
            trimedCommand[commandLength - 1] = '\0';
        }
    }
    return trimedCommand;
}

/* Get the instruction by the command name  */
commandType GetCommandByName(char *command)
{
    commandType resultCommand = {"error", TypeError, -1};
    /* Go over the commands and find the command that equal to the command name */
    int i;
    if (command)
    {
        for (i = 0; i != TABLE_SIZE; i++)
        {
            if (strcmp(cmd[i].instractionName, command) == 0)
            {
                resultCommand = cmd[i];
                break;
            }
        }
    }

    return resultCommand;
}
/* Get the Operand by the scanf of the parmeter */
OperandType getOperandType(char *parameter)
{
    OperandType operandResult = Error;
    int number;
    char label[LABEL_MAX_LEN];
    /*Check for the Operand Type*/
    /*Register*/
    if (strlen(parameter) == 2 && *parameter == REGISTER_OPERAND &&
        isdigit(parameter[1]) && REGISTER_MAX_SIZE - (int)parameter[1] >= 0)
    {
        operandResult = Register;
    }
    /*Immediate*/
    else if (sscanf(parameter, "#%d", (int *)&number) == 1)
    {
        operandResult = Immediate;
    }
    /*Direct*/
    else if (sscanf(parameter, "%30[a-zA-Z0-9]", label) == 1)
    {
        operandResult = Direct;
    }
    /*Relative*/
    else if (strchr(parameter, '%'))
    {
        operandResult = Relative;
    }
    /*not recognized*/
    else
    {
        operandResult = Error;
    }
    return operandResult;
}
/*check if the operand stand by the rules*/
int checkOperand(OperandType command, char *parameter, int isSource, int rowNumber)
{
    OperandType operandResult;
    int type = command;
    int hasError = false;
    operandResult = getOperandType(parameter);
    switch (type)
    {
    case mov:
        if (isSource)
        {
            if (operandResult != Immediate && operandResult != Direct && (operandResult != Register))
                hasError = true;
        }
        else
        {
            if (operandResult != Direct && (operandResult != Register))
                hasError = true;
        }
        break;
    case cmp:
        if (isSource)
        {
            if (operandResult != Immediate && operandResult != Direct && (operandResult != Register))
                hasError = true;
        }
        else
        {
            if (operandResult != Immediate && operandResult != Direct && (operandResult != Register))
                hasError = true;
        }
        break;
    case add:
        if (isSource)
        {
            if (operandResult != Immediate && operandResult != Direct && (operandResult != Register))
                hasError = true;
        }
        else
        {
            if (operandResult != Direct && (operandResult != Register))
                hasError = true;
        }
        break;
    case sub:
        if (isSource)
        {
            if (operandResult != Immediate && operandResult != Direct && (operandResult != Register))
                hasError = true;
        }
        else
        {
            if (operandResult != Direct && (operandResult != Register))
                hasError = true;
        }
        break;
    case lea:
        if (isSource)
        {
            if (operandResult != Direct)
            {
                hasError = true;
            }
        }
        else
        {
            if (operandResult != Direct && (operandResult != Register))
            {
                hasError = true;
            }
        }
        break;
    case clr:
        if (operandResult != Direct && (operandResult != Register))
            hasError = true;
        break;
    case not:
        if (operandResult != Direct && (operandResult != Register))
            hasError = true;
        break;
    case inc:
        if (operandResult != Direct && (operandResult != Register))
            hasError = true;
        break;
    case dec:
        if (operandResult != Direct && (operandResult != Register))
            hasError = true;
        break;
    case jmp:
        if (operandResult != Direct && (operandResult != Relative))
            hasError = true;
        break;
    case bne:
        if (operandResult != Direct && (operandResult != Relative))
            hasError = true;
        break;
    case jsr:
        if (operandResult != Direct && (operandResult != Relative))
            hasError = true;
        break;
    case red:
        if (operandResult != Direct && (operandResult != Register))
            hasError = true;
        break;
    case prn:
        if (operandResult != Immediate && operandResult != Direct && (operandResult != Relative))
            hasError = true;
        break;
    }
    if (hasError)
    {
        return Error;
    }

    return operandResult;
}
/* This function get the command,2 strings,row number and insert it to the instraction tabel
 * command      - the command
 * source 		- the first operand on the command
 * destination 	- the second operand on the command
 * rowNum 		- the row number where this command were taking from
 * Output 		-  check if the was en error and return true else return false*/
int computeInstrctionRows(commandType command, int *IC, char *source, char *destination, int rowNumber)
{
    int hasError;
    OperandType sourceOp, destinationOp;
    hasError = false;
    /*check if the source is legil*/
    if (source != NULL && !IsRowEmpty(source))
    {
        sourceOp = checkOperand(command.type, source, true, rowNumber);
        if (sourceOp == -1)
        {
            fprintf(stderr, "Error in row number:%d\n", rowNumber);
            fprintf(stderr, "Error Invalid source operand:%s\n", destination);
            hasError = true;
        }
    }
    else
    {
        sourceOp = 0;
    }
    /*check if the destination operand is legil*/
    if (destination != NULL && !IsRowEmpty(destination))
    {
        destinationOp = checkOperand(command.type, destination, false, rowNumber);
        if (destinationOp == -1)
        {
            fprintf(stderr, "Error in row number:%d\n", rowNumber);
            fprintf(stderr, "Error Invalid destination operand:%s\n", destination);
            hasError = true;
        }
    }
    else
        destinationOp = 0;
    if (*IC == 100)
        tabelSize = 0;
    /*adding the Row to the instraction tabel*/
    addInstractionToTabel(&command, *IC, sourceOp, destinationOp);
    (*IC)++;
    if (source != NULL && !IsRowEmpty(source))
    {
        hasError = addOperandToInstractionTable(sourceOp, *IC, rowNumber, source);
        (*IC)++;
    }
    if (destination != NULL && !IsRowEmpty(destination) && destinationOp != Error)
    {
        hasError = addOperandToInstractionTable(destinationOp, *IC, rowNumber, destination);
        (*IC)++;
    }

    return hasError;
}