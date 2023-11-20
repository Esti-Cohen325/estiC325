#include "linked_list.h"
#define MAX_ERROR_LINE 80
#define MAX_NUM_FOR_DATA 16383
#define MIN_NUM_FOR_DATA -16384
#define MAX_NUM_FOR_INSTRUCTION 4095
#define MIN_NUM_FOR_INSTRUCTION -4096
#define WHITE_CHARACTER "\t \n \v \f \r ,()"

/*checks if the macro name as defined */
void errorDoubleMacroName(char *macro_name,list *macro_list);

/*checks double defenition of labels*/
void errorDoubleLabelName(char *lable_name,list *leble_list);

/*if there is an entry to a label the function checks if the label as defined if nor=error*/
void checkEntryNotDefined(list *entry_lable_list);
/*Checks if the lebal is a legal*/
int checkLabel(char str[]);

/*Checks if the string is legal*/
int checkString(char str[]);

/*Checks if the numbers in data are legal*/
int checkData(char str[]);

/*Checks if the commas in data sentences are correct */
int checkDataCommas(char s[]);

/* checks if there are incorrect commas */
int checkCommas(char s[]);

/*Checks whether the commas are correct */
int checkLegalCommas(char s[]);

/*Checks if the number after label # is a legal */
int checkNumber(char str[]);

/*Checks if the register is a legal*/
int checkRegister(char s[]);

/*Checks whether the teaching sentences are legal*/
int checkTeachingSentences(char str[]);

/*Checks if the macro is a legal*/
void checMacroName(char *str);
