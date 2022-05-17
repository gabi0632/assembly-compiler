#ifndef _FileHandler_H_
#define _FileHandler_H_
/*Get file name and file pointer and create it*/
FILE* OpenFile(FILE*,char *);
/*  Write Row into Entry File
*   Input the Symbol name and the place in the instraction Code
*    Output create file if not existe and write the  label name and place in memory*/
void outputEntryFile(char *symbolName, short ICNumbar);
/*  Write Row into Externy File
*   Input the Symbol name and the place in the instraction Code
*    Output create file if not existe and write the  label name and place in memory*/
void outputeExternalFile(char *symbolName, short ICNumbar);
/*  Write Row into Object File
 *   Input the Symbol name and the place in the instraction Code
 *   Output create file if not existe and write the name and place in memory*/
void outputeObjectFile(int IC,int DC);
/*  Write Row into Object File
 *   Input the Symbol name and the place in the instraction Code
 *   Output create file if not existe and write the name and place in memory*/
void outputeDataToObjectFile(int IC,int number);
/* Close  all open files
 * In case of success return 1, otherwise return 0 and print error
 */
int CloseAllOpenFiles();
/*In case of Error in the code Delet all open files
* In case of success return 1, otherwise return 0 and print error*/
void DeleteAllFiles();
#endif