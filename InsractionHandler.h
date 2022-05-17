#ifndef InstractionHandler_H_
#define InstractionHandler_H_
#include "globals.h"
/*  The function will handel all data type
*   Input:
*   instructionParameter    - the type of the data
*   IC          - Instraction counter
*   sourceOp       - the source operand
*   destinationOp   -the destinatio operand*/
void addInstractionToTabel(commandType *instructionParameter, long IC, OperandType sourceOp, OperandType destinationOp);
/*return the head of the list*/
instructionCode *getListHead();
/*The function go over the instraction tabel in update all the missing parts
    in case of error return true else false*/
int updateInstractionTabel();
/*Free all the Nodes in the instraction tabel*/
void FreeInstractionTabel();
/*The function convert number to binary code*/
int numberToHex(short number);
/*  The function check if row is Empty
*   input row from File
*   Output return true is empty else false*/
int IsRowEmpty(char *row);
/*  Function to create one big string without white spaces and check number of commas
*   Input a string with space and the row number 
*   Outpot a proper string or Null if not by the rolles*/
char *trimmFunction(char *strCommand, int rowNumber);
/* Get the instruction by the command name  */
commandType GetCommandByName(char *command);
/* Get the Operand by the scanf of the parmeter  */
OperandType getOperandType(char *parameter);
/* This function get the command,2 strings,row number and insert it to the instraction tabel
 * command      - the command
 * source 		- the first operand on the command
 * destination 	- the second operand on the command
 * rowNum 		- the row number where this command were taking from
 * Output 		-  check if the was en error and return true else return false*/
int computeInstrctionRows(commandType,int *,char*,char*,int);
#endif