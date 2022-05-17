
#ifndef GLOBALS_H_
#define GLOBALS_H_

#define LABEL_MAX_LEN 32
#define ROW_MAX_LEN 81
#define TABLE_SIZE 16

typedef int bool;
#define true 1
#define false 0

#define space " "
#define comma ","
#define tab "\t"

typedef enum
{
    mov = 0,
    cmp=1,
    add,
    sub,
    lea,
    clr,
    not,
    inc,
    dec,
    jmp,
    bne,
    jsr,
    red,
    prn,
    rts,
    stop,
    TypeError = -1
} FUNC_TYPE;

typedef enum
{
    LINK_TYPE_A,
    LINK_TYPE_E,
    LINK_TYPE_R
} LINK_TYPE;

typedef struct instruction
{
    char *instractionName;
    FUNC_TYPE type;
    int operandsINumber;
    int opCode;
    int funct;
} commandType;

typedef enum
{
    Immediate=0,
    Direct,
    Relative,
    Register,
    Error = -1
} OperandType;

typedef struct node
{
    char label[LABEL_MAX_LEN];
    int value;
    char attribute[9];
    int isExternal;
    int isAction;
    struct node *next;
} symbol;

typedef struct instructionNode
{
    short machineCode;
    char parameter[ROW_MAX_LEN];
    short CIC; /*The current IC of this row*/
    OperandType operand;
    LINK_TYPE linkType;
    struct instructionNode *next;

} instructionCode;


#endif
