
#include "errors.h"
void decode(FILE *f,char *line,char *first_word_in_line,int *icounter,int *dcounter)
{
char *temp;
int i=0;
list *entry_lable_list;
flages line_taken=ON;/*if there is just one word in line it will be insert to first_word_in_line and line_taken=OFF*/
char* lebleName;
int leble_count=0;
flages flage=ON;/*if flage=0-false the function wont count ic or dc*/
flages leble_flage=OFF;/*to tell if there is a lable*/
char delime[3];
char* copy_line;
int ic=0,dc=0;
int length;

entry_lable_list=createList();
createLableList();
delime[0]=TAB;
delime[1]=SPACE;
delime[2]=ENTER;
lineNum=1;
temp=(char*)malloc((MAX_LINE_LENGTH)*sizeof(char));
copy_line=(char*)malloc((MAX_LINE_LENGTH)*sizeof(char));
lebleName=(char*)malloc((MAX_LINE_LENGTH)*sizeof(char));
while(!feof(f))
{ 	
fgets(line,MAX_LINE_LENGTH,f);/*get the rest of the line into "line"*/
flage=prepLine(first_word_in_line,line);
	if(line_taken!=OFF)
		jumpspace(&line);
	if(strstr(first_word_in_line,"stop")||strstr(first_word_in_line,"rts"))
	{
		flage=0;
		ic=ic+1;
	}

	if(first_word_in_line[0]==REMARK)/*it's a remark*/
	{
		flage=OFF;
	}
	length=strlen(first_word_in_line);
	copyArr(copy_line,line);
	if(length==0)
		flage=OFF;

	if(first_word_in_line[length-1]==LABEL)/*the line start with a lebel so we skip the leble and also skis the operation name*/
	{
		
		checkLabel(first_word_in_line);/*check if it a ligal label*/
		leble_flage=ON;
		if(leble_flage==ON)/*if there was a leble in this line we will kip the leble and the line in the leble tub*/
		{
			jumpspace(&line);
			
			copyArr(lebleName,first_word_in_line);
			if(line[0]==POINT)
			{
				add_to_lebles(leble_list,lebleName,dc+100,instruction_label);
			}
			else
				add_to_lebles(leble_list,lebleName,ic+100,inside_label);
			leble_count+=1;
			
		}
		if(strstr(line,"stop")||strstr(line,"rts"))
		{
			ic=ic+1;
			flage=0;
		}
		jumpspace(&line);
		if(line[0]!=POINT&&flage==ON)/*if it's not a guiding line we will skip the operation name*/
		{
		length=strlen(strtok(copy_line,delime));
		copyArr(copy_line,line+length);
		jumpspace(&copy_line);
		}
	}
	jumpspace(&first_word_in_line);
	
	if(strcmp(first_word_in_line,".extern")==0)
	{
		jumpspace(&line);
		checkLabel(line);/*check if it a ligal label*/
		copy_line=strtok(copy_line,delime);
		add_to_lebles(leble_list,copy_line,0,external_leble);
		flage=OFF;
	}
	if(strcmp(first_word_in_line,".entry")==0)
	{
		jumpspace(&line);
		copyArr(copy_line,line);
		checkLabel(line);/*check if it a ligal label*/
		jumpspace(&copy_line);
		copy_line=strtok(copy_line,delime);
		add_to_lebles(entry_lable_list,copy_line,-1,entry_label);
	}
	if(flage==ON&&(line[0]==POINT || first_word_in_line[0]==POINT))
	{
		copyArr(copy_line,line);
		jumpspace(&copy_line);
		if(first_word_in_line[1]==DATA)
		{
			
			length=strlen(copy_line);
			if(checkData(copy_line)==0)
			{
				if(checkDataCommas(copy_line)==0)
					dc+=get_data_dc(copy_line);
			}
			copyArr(copy_line,copy_line+length);
		}
		if(first_word_in_line[1]==STRING)
		{
			if(checkString(copy_line)==0)
				dc+=get_str_dc(copy_line);
		}
		copyArr(copy_line,line+1);
		if(copy_line[0]==DATA)
		{
			
			jumpspace(&copy_line);
			length=strlen(copy_line+4);
			if(checkData(copy_line+4)==0)
			{
				if(checkDataCommas(copy_line+4)==0)
					dc+=get_data_dc(copy_line);
			}
			copyArr(copy_line,copy_line+length);
		}
		if(copy_line[0]==STRING)
		{
			jumpspace(&copy_line);
			if(checkString(copy_line+6)==0)
				dc+=get_str_dc(copy_line+6);
			
		}
	flage=OFF;
	}
	length=strlen(line);
	if(flage==ON&&length==0)
	{
		ic+=1;
		flage=OFF;/*false*/
	}
	jumpspace(&line);
	if(line[0]!=POINT&&flage==ON)
		ic+=1;

	if(line[0]!=REG && line[0]!=IMMIDIAT && line[0]!=POINT && flage==ON)/*it's a leble*/
	{
		ic+=1;
		copyArr(copy_line,line);
		temp=strrchr(copy_line,'(');/*get the string from the chat'('*/
		if(temp==NULL)/* if the char '(' is not after the leble if the command is just a */
		{

			copyArr(copy_line,line);
			temp=strrchr(copy_line,COMMA);
			if(temp!=NULL)
			{
				ic+=1;
			}
			flage=OFF;/*if the command is just a leble we counted ic already*/
		}
		if(flage==ON)
		{
			copyArr(line,temp);
			line=line+1;
		}
		if(flage==ON&&line[0]!=REG && line[0]!=IMMIDIAT && line[0]!=POINT )
		{
			ic+=2;
			flage=OFF;
		}
	}
	if(flage==ON&&line[0]==REG)/*check if it's a register*/
	{
		copyArr(copy_line,line+REG_LENGTH);
		jumpspace(&copy_line);
		if(copy_line[0]!=END)
		{
			copyArr(copy_line,copy_line+1);/*skip the comma*/
			jumpspace(&copy_line);
		}
		if(copy_line[0]==REG ||copy_line[0]==END)/*check if it's another register*/
		{
			/*if ther is an operation name and than two register the command line will translat into tow code line*/
			ic+=1;
		}
		else/*after the operand reg there is an immidiat or a lebel operand-so the command line will translat into three code line*/
		{
			ic+=2;
		}
	}
	if(flage==ON&&line[0]==IMMIDIAT)/*check if the first operand is an immidiat-the the command line will translat to three code line*/
	{
		
		i=2;
		while(isdigit(copy_line[i]))
		{
			i=i+1;
		}
		copyArr(copy_line,copy_line+i);
		jumpspace(&copy_line);
		if(copy_line[0]==END)
		{
			ic+=1;
		}
		else
		{
			ic+=2;
		}
	}
	line_taken=1;
	leble_flage=0;
	flage=ON;
	fseek(f, 0, SEEK_CUR);
	if(fgetc(f)==END||feof(f))
		break;
	fseek(f, -1, SEEK_CUR);
	lineNum=lineNum+1;
}/*end of while*/
/*free(copy_line);
free(lebleName);
free(temp);*/
*icounter=ic;
*dcounter=dc;
checkEntryNotDefined(entry_lable_list);
calculat_instruction_label(leble_list,ic);
printEntery(entry_lable_list);
return;
}

void calculat_instruction_label(list *leble_list,int ic)
{
lebels *l=(*(*leble_list).head).lable_var;
	while(l!=NULL)
	{
		if((*l).era==instruction_label)
		{
			(*l).era=inside_label;
			(*l).label_address=(*l).label_address+ic;
		}
		l=(*l).next_leble;
	}
}

/*
this function prints to the print_list all the entry labls-the print list will be print in the end of the program
*/
void printEntery(list *entry_lable_list)
{
	char *temp_line;
	lebels *l;
	lebels *l_ent;
	l=(*(*leble_list).head).lable_var;
	l_ent=(*(*entry_lable_list).head).lable_var;
	while(l!=NULL)
	{
		while(l_ent!=NULL)
		{
			if(strcmp((*l).leble_name,(*l_ent).leble_name)==0)
			{
			temp_line=(char*)malloc((strlen((*l).leble_name)+10)*sizeof(char));
			sprintf(temp_line,"%s\t%d",(*l).leble_name,(*l).label_address);
			insertPrint(print_list,temp_line,entry);/*insert the line of entrny lable into print_list*/
			free(temp_line);
			}
			l_ent=(*l_ent).next_leble;
		}
	l=(*l).next_leble;
	l_ent=(*(*entry_lable_list).head).lable_var;
	}
}

/*
This function receives an instruction statement of ".data" and summarizes the dc
*/
int get_data_dc(char *line)
{
	char comma[2];
	int dc=0;
	comma[0]=COMMA;
	jumpspace(&line);
	strtok(line,",");
	while(line!=NULL)
	{
		dc++;
		line=strtok(NULL,comma);
	}
	return dc;
}
/*
This function receives an instruction statement of ".string" and summarizes the dc
*/
int get_str_dc(char *line)
{
	int dc=0;
	jumpspace(&line);
	line=line+1;
	while(line[0]!='"')
	{
		dc+=1;
		line=line+1;
	}
return dc+1;/*because in the end of a string there is '\0' */
}

/*
This function puts each label in the label list and saves the label address
*/
void add_to_lebles(list *list_to_add,char* leble_n,int ic,leble_ERA era)
{
	lebels *l=(lebels*)malloc(sizeof(lebels));
	if(leble_n==NULL)
	{
		return;
	}
	jumpspace(&leble_n);
	if(leble_n==NULL||strlen(leble_n)==0)
		return;

	(*l).leble_name=(char*)malloc(strlen(leble_n)*sizeof(char));
	if(!l)
	{
		fprintf(stderr,"No alocation\n");
		exit(0);
	}
	strtok(leble_n,":");
	(*l).leble_name=copyString(leble_n);
	(*l).label_address=ic;
	(*l).era=era;
	errorDoubleLabelName(leble_n,list_to_add);/*CHECK ERROR-IF THE LABEL HAS BEEN DEFINED ALREADY*/
	insertLable(list_to_add,l);
	
	free((*l).leble_name);
	free(l);
}
flages prepLine(char *first_word_in_line,char *line)
{
int i=0;
int j=0;
int t=-1;
flages flage;
	char error_line[MAX_ERROR_LINE];
	while(!isspace(line[i]))
		i++;
	if(line[i-1]!=LABEL)
	{
	jumpspace(&line);

	t=checkTeachingSentences(line);
	}
	for(j=0;j<i;j++)
	{
		first_word_in_line[j]=line[j];
	}
	first_word_in_line[j]=END;
	copyArr(line,line+j);
	if(t==-1)
	{
			jumpspace(&line);
		t=checkTeachingSentences(line);
	}
	if(first_word_in_line[j-1]==LABEL&&line[0]==END)
	{
			insertPrint(print_list,"Error - no command after the label",error);
			sprintf(error_line,"lable name:%s	line num%d",first_word_in_line,lineNum);
			insertPrint(print_list,error_line,error);
			insertPrint(print_list,"----------------------------------------------",error);
	}
	if(t==1){
		flage=OFF;
		isValid=1;
	}
	else
		flage=ON;
return flage;
}

