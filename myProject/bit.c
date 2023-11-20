
#include "errors.h"
#define NOT_CAMMAND_LAW -1
#define DECOD_LINE 99
void turn_to_bit(FILE *f,char line[],char first_word_in_line[])
{
int count_decod_lines=DECOD_LINE;/*counts the number of line we decode*/
int second_sort=0;/*a varieble that tells if the sorting method is jump type*/
char *copy_str;
char delime[3];
int length=0;
int command_op=0;
int i=0;
list *instruction_list;
commands command_tub[COMMAND_NUM];/*there are 16 types of commands*/
flages skip_this_line=OFF;
sort sorting;
lineNum=1;
instruction_list=createList();
copy_str=(char*)malloc((MAX_LINE_LENGTH)*sizeof(char));
rewind(f);
/*initialize the command_table*/
start_command_struct(command_tub,0,"mov",0,ON,ON);
start_command_struct(command_tub,1,"cmp",1,ON,ON);
start_command_struct(command_tub,2,"add",2,ON,ON);
start_command_struct(command_tub,3,"sub",3,ON,ON);
start_command_struct(command_tub,4,"not",4,OFF,ON);
start_command_struct(command_tub,5,"clr",5,OFF,ON);
start_command_struct(command_tub,6,"lea",6,ON,ON);
start_command_struct(command_tub,7,"inc",7,OFF,ON);
start_command_struct(command_tub,8,"dec",8,OFF,ON);
start_command_struct(command_tub,9,"jmp",9,OFF,ON);
start_command_struct(command_tub,10,"bne",10,OFF,ON);
start_command_struct(command_tub,11,"red",11,OFF,ON);
start_command_struct(command_tub,12,"prn",12,OFF,ON);
start_command_struct(command_tub,13,"jsr",13,OFF,ON);
start_command_struct(command_tub,14,"rts",14,OFF,OFF);
start_command_struct(command_tub,15,"stop",15,OFF,OFF);
delime[0]=TAB;
delime[1]=SPACE;
delime[2]=ENTER;
	while(!feof(f) )
	{
		
		fscanf(f,"%s",first_word_in_line);
		jumpspace(&first_word_in_line);
		if(first_word_in_line[0]==REMARK)
			skip_this_line=ON;
		if(!fgets(line,MAX_LINE_LENGTH,f))
			strcpy(line," ");

		jumpspace(&line);
		length=strlen(first_word_in_line);
		jumpspace(&first_word_in_line);
		if(feof(f))
			break;

		/*check if there is a command in this line and if so get the op command*/
		if(skip_this_line==OFF)
			command_op=comper(command_tub,first_word_in_line);
		if(strlen(line)==0)
		{
			skip_this_line=ON;
			if(command_op!=NOT_CAMMAND_LAW)
			{
				put_op(command_tub[command_op].op,0,0,0);
				count_decod_lines=count_decod_lines+1;
			}
				
		}
		else
		{
			copyArr(copy_str,line);
			copy_str=strtok(copy_str,delime);
		}
		
		if(command_op==NOT_CAMMAND_LAW&&skip_this_line==OFF)/*check if the command is not the first_word_on_line*/
			command_op=comper(command_tub,copy_str);
		if(command_op!=NOT_CAMMAND_LAW && skip_this_line==OFF)
		{
			if(command_tub[command_op].operand_source==OFF && command_tub[command_op].operand_des==OFF)
			{
				skip_this_line=ON;
				sorting.sort_source_type=0;
				sorting.sort_des_type=0;
			}
			count_decod_lines++;
			if(first_word_in_line[length-1]==LABEL && skip_this_line==OFF)/*the line start with a lebel so we skip the leble and also skip the operation name*/
			{
				jumpspace(&line);
				copyArr(copy_str,line);/*COMMAND_LENGTH=3*/
				copyArr(copy_str,copy_str+COMMAND_LENGTH);
				jumpspace(&copy_str);
				set_sort_typ(copy_str,&sorting.sort_source,&count_decod_lines,&(sorting.sort_source_type),sorting);/*get sort type of the operand*/
			}
	
			else if(skip_this_line==OFF)/*the first word in line is the command (and not a leble)*/
			{
				copyArr(copy_str,line);
				set_sort_typ(copy_str,&sorting.sort_source,&count_decod_lines,&(sorting.sort_source_type),sorting);/*get sort type of the operand*/

			}
		
			if(command_tub[command_op].operand_source==OFF && skip_this_line==OFF)/*there is no operand_source in this command there is just a des operand*/	
			{
				sorting.sort_des_type=sorting.sort_source_type;
				sorting.sort_source_type=0;
					if(sorting.sort_des_type==jump_sort)
					{
						sorting.jump_address=sorting.sort_source;
					}
					else
						sorting.sort_des=sorting.sort_source;
					sorting.sort_source=0;
					skip_this_line=ON;/*we dont have to check operand des*/
			}
			i=0;
			if(copy_str[i]==IMMIDIAT&&copy_str[i+1]==NEGETIVE)
				i++;
			i++;/* reg and middiat length (for examle r1/#3 */
			while(isdigit(copy_str[i]))
				i++;
			while(isalpha(copy_str[i]))
				i++;

			if(sorting.sort_source_type!=jump_sort && skip_this_line==OFF&&sorting.sort_source_type!=initialize)
			{
				copyArr(copy_str,copy_str+i);
				jumpspace(&copy_str);
				copyArr(copy_str,copy_str+1);/*because of the comma*/
				set_sort_typ(copy_str,&sorting.sort_des,&count_decod_lines,&(sorting.sort_des_type),sorting);/*get sort type of the operand*/
			}
			if(sorting.sort_des_type==jump_sort)
			{
				second_sort=jump_sort;
				copyArr(copy_str,copy_str);
				copy_str=strrchr(copy_str,'(');
				copyArr(copy_str,copy_str+1);
				set_sort_typ(copy_str,&sorting.sort_source,&count_decod_lines,&(sorting.sort_source_type),sorting);/*get sort type of the operand*/
				copy_str=strrchr(copy_str,COMMA);
				copyArr(copy_str,copy_str+1);
				set_sort_typ(copy_str,&sorting.sort_des,&count_decod_lines,&(sorting.sort_des_type),sorting);/*get sort type of the operand*/
			}
			put_op(command_tub[command_op].op,sorting.sort_source_type,sorting.sort_des_type,second_sort);/*decod the op code line-preper and insert to print_list*/
			if(second_sort==jump_sort)
			{
				print_binary(sorting.jump_address);
			}
			if(!(command_tub[command_op].operand_source==OFF && command_tub[command_op].operand_des==OFF))
				next_binary_line(sorting,&count_decod_lines);
		}
		if(command_op==NOT_CAMMAND_LAW && skip_this_line==OFF)/*it is not a command line*/
		{
			jumpspace(&first_word_in_line);
			jumpspace(&line);
			if(first_word_in_line[1]=='e'||line[1]=='e')/*it .externy or entery*/
				skip_this_line=ON;
			if(first_word_in_line[length-1]==LABEL&& skip_this_line==OFF)
			{
				copyArr(copy_str,line);
				jumpspace(&copy_str);
				addToInstruction(instruction_list,copy_str[1],copy_str+1);
			}
			else
				addToInstruction(instruction_list,first_word_in_line[1],line);
		
		}
	lineNum=lineNum+1;
	skip_this_line=OFF;
	second_sort=0;
	sorting.sort_des=-1;
	sorting.sort_source=-1;
	sorting.sort_source_type=initialize;
	sorting.sort_des_type=initialize;
	}/*end while*/
	printInstruction(instruction_list);
return;
}


void addToInstruction(list *instruction_list,char type,char* line)
{
int intiger=0;
int i=0;
flages flage=ON;
jumpspace(&line);
if(type==line[0])
 flage=OFF;/*becase there was a lable in this line the line that we get in this function starts whith ".string" and not with the data string*/
	if(type==STRING)
	{
		
		if(flage==OFF)/*we need to skip the word "string"*/
			copyArr(line,line+6);
		jumpspace(&line);
		i=1;/*beacase of the ""*/
		while(line[i]!='"'&&line[i]!=END)
		{
		insertInstruction(instruction_list,(int)line[i]);
		i=i+1;
		}
	insertInstruction(instruction_list,0);
	}
	if(type==DATA)
	{
		if(flage==OFF)/*we need to skip the word "data"*/
			line=line+4;
	jumpspace(&line);
	line=strtok(line,",");
		while(line!=NULL)
		{
			 intiger=atoi(line);
			 insertInstruction(instruction_list,intiger);
			line=strtok(NULL,",");
		}
	}

}

lebels* get_leble_adress(list *list_to_search,char line[])
{
char delime[6];
lebels *l=(lebels*)malloc(sizeof(lebels));
	if(!l)
	{
		fprintf(stderr,"No alocation\n");
		exit(0);
	}
	
	delime[0]=TAB;
	delime[1]=SPACE;
	delime[2]=ENTER;
	delime[3]=COMMA;
	delime[4]=')';
	delime[5]='(';
	l=(*(*list_to_search).head).lable_var;
	line=strtok(line,delime);
	while(l!=NULL)
	{
		if(strcmp(line,(*l).leble_name)==0)
		{
			return l;
		}
		l=(*l).next_leble;
	}

return NULL;/*erorre*/
}

void printInstruction(list *instruction_list)
{
	instructions *in;
	in=(*(*instruction_list).head).instruction_var;
	while(in!=NULL)
	{
		print_binary((*in).data);
		in=(*in).next_instruction;
	}
}


