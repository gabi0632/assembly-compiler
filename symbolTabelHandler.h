#ifndef SymbolTabelHandler_H_
#define SymbolTabelHandler_H_

/*Initialize linked list symbol and set pointer of head symbol
 * In case of failure exit from the program and print failure message
 * */
void InitializeSymbolTable();
/*Check if the label is Stand in the standart
*Input label and the row numeber
*output true if has problem else false*/
int labelHandler(char *label, int rowNumber);
/*The function check if row contain label*/
int isContainLabel(char *line);
/*The function Add new label to SymbolTabel
*   Input:
*   label       - label name
*   isExternal  - External Flag
*   isAction    - Action Flag
*   rowNumber   - the row number in the assemble file
*   Output:
*               -0 in case of a failure
* 			    -1 in case success   */    
int addNewLabel(char *label, int counter, int isExternal, int isAction, int rowNumber);
/*The Function Update the value of the symbol and sort it*/
void updateSymbolTabel(int *IC, int *DC);
/*Find a symbol in the Symbol tabel
*   Input:
*   label       - label name
*   Output:
*               -NULL if not found
* 			    -the Symbol in case success   */ 
symbol *findSymbol(char *label);
/*Free all the Nodes in the instraction tabel*/
void FreeSymbolTable();
#endif