#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "DataBuilder.h"
#include "InsractionHandler.h"
#include "symbolTabelHandler.h"

#define ROW_MAX_LEN 81 /*Max Row len*/
#define LABEL_MAX_LEN 32 /*Max Label len*/
#define MAX_SIZE_ATTRIBUTES 9

symbol *symTabHead;
int tabelSize;

/*Initialize linked list symbol and set pointer of head symbol
 * In case of failure exit from the program and print failure message
 * */
void InitializeSymbolTable()
{
    tabelSize = 0;
    symTabHead = (symbol *)malloc(sizeof(symbol));
    /*Check if space allocation for symbol were successful*/
    if (symTabHead == NULL)
    {
        printf("failed to allocate memory for the symbol tabel");
        exit(0);
    }
    /*set value for symbol variable*/
    symTabHead->value = -1;
    symTabHead->isExternal = false;
    symTabHead->isAction = false;
    symTabHead->next = NULL;
}
/*Check if the label is Stand in the standart
*Input label and the row numeber
*output true if has problem else false*/
int labelHandler(char *label, int rowNumber)
{
    int hasError = false;
    if (isdigit(*label))
    {

        fprintf(stderr, "Error in row number:%d\n", rowNumber);
        fprintf(stderr, "Error label can't start with number\n");
        hasError = true;
    }
    else if (strlen(label) > LABEL_MAX_LEN && !hasError)
    {
        fprintf(stderr, "Error in row number:%d\n", rowNumber);
        fprintf(stderr, "Error label is to long\n");
        hasError = true;
    }
    else if (label[strlen(label) - 1] != ':')
    {
        fprintf(stderr, "Error in row number:%d\n", rowNumber);
        fprintf(stderr, "Error label doesn't ending with colon\n");
        hasError = true;
    }
    return hasError;
}
/*The function check if row contain label*/
int isContainLabel(char *line)
{
    if (strchr(line, ':'))
        return true;
    return false;
}

int addNewLabel(char *label, int counter, int isExternal, int isAction, int rowNumber)
{
    int hasErorr = false;
    char *removeTril;
    if (!label)
    {
        hasErorr = false;
    }
    /* If the label ends with \n remove it */
    else
    {
        if ((GetCommandByName(label)).type == TypeError&&getOperandType(label)!=Register)
        {
            if ((removeTril = strchr(label, '\n')) != NULL)
                *removeTril = '\0';
            if (tabelSize < 1)
            {
                strcpy(symTabHead->label, label);
                symTabHead->value = counter;
                symTabHead->isExternal = isExternal;
                symTabHead->isAction = isAction;
                if (symTabHead->isAction)
                    strcpy(symTabHead->attribute, "code");
                else if (symTabHead->isExternal)
                    strcpy(symTabHead->attribute, "external");
                else
                    strcpy(symTabHead->attribute, "data");
                symTabHead->next = NULL;
                tabelSize++;
            }
            else
            {
                symbol *tmp = symTabHead;
                symbol *lastnode;/*Pointer the help check all nodes for dupliction*/
                while (tmp!= NULL)
                {
                    if (!(strcmp(label, tmp->label)))
                    {
                        if(!tmp->isExternal)
                        {
                        hasErorr = true;
                        fprintf(stderr, "Error in row number:%d\n", rowNumber);
                        fprintf(stderr, "Error the labels:%s is duplicate\n", label);
                        }
                    }
                    lastnode=tmp;
                    tmp = tmp->next;
                }
                tmp=lastnode;
                if (!hasErorr)
                {
                    tmp->next = (symbol *)malloc(sizeof(symbol));

                    /*Check if space allocation for list node were successful*/
                    if (tmp->next == NULL)
                    {
                        free(tmp->next);
                        printf("failed to allocate memory for the symbol tabel");
                        exit(0);
                    }
                    strcpy(tmp->next->label, label);
                    tmp->next->value = counter;
                    tmp->next->isExternal = isExternal;
                    tmp->next->isAction = isAction;
                    if (isAction)
                    {
                        strcpy(tmp->next->attribute, "code");
                    }
                    else if (isExternal)
                    {
                        strcpy(tmp->next->attribute, "external");
                    }
                    else
                        strcpy(tmp->next->attribute, "data");
                    tmp->next->next = NULL;
                }
            }
        }
        else
            hasErorr=true;
    }
    return hasErorr;
}
/* function to insert a new_node in a list. Note that this 
  function expects a pointer to head_ref as this can modify the 
  head of the input linked list (similar to push())*/
void sortedInsert(symbol **head_ref,symbol *new_node)
{
    symbol *current;
    /* Special case for the head end */
    if (*head_ref == NULL || (*head_ref)->value >= new_node->value)
    {
        new_node->next = *head_ref;
        *head_ref = new_node;
    }
    else
    {
        /* Locate the symbol before the point of insertion */
        current = *head_ref;
        while (current->next != NULL &&
               current->next->value < new_node->value)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

/* function to sort a singly linked list using insertion sort */
void insertionSort(symbol **head_ref)
{
    /* Initialize sorted linked list */
    symbol *sorted = NULL;

    /* Traverse the given linked list and insert every */
    /* symbol to sorted */
    symbol *current = *head_ref;
    while (current != NULL)
    {
        symbol *next = current->next;
        sortedInsert(&sorted, current);
        current = next;
    }

    /* Update head_ref to point to sorted linked list */
    *head_ref = sorted;
}
/*The Function Update the value of the symbol and sort it*/
void updateSymbolTabel(int *IC, int *DC)
{
    if (*DC)
    {
        symbol *tmp;
        tmp = symTabHead;
        for (; tmp != NULL; tmp = tmp->next)
        {
            if (!tmp->isAction && !tmp->isExternal)
            {
                tmp->value += (*IC);
            }
        }
        insertionSort(&symTabHead);
    }
}
/*Find a symbol in the Symbol tabel*/
symbol *findSymbol(char *label)
{
    symbol *tmp;
    tmp = symTabHead;
    for (; tmp != NULL; tmp = tmp->next)
    {
        {
            if (strstr(label, tmp->label) != NULL)
            {
                return tmp;
            }
        }
    }
    return tmp;
}
/*Free all the Nodes in the instraction tabel*/
void FreeSymbolTable()
{
    symbol *tmp = symTabHead;
    symbol *current;
    while (tmp->next != NULL)
    {
        current=tmp;
        tmp = tmp->next;
        free(current);
    }
}

