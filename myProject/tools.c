#include "errors.h"
/*
this funtion return the macro name
@param s-the all macro command in the macro table
*/
char* getmacroname(char *s)
{
    jumpspace(&s);
    strtok(s,"\n");
    return (char*)s;
}
/* Function to copy the string*/
char* copyString(char *s)
{
    char *s2;
    s2 = (char*)malloc((strlen(s)+100)*sizeof(char));

    strcpy(s2, s);
    return s2;
}
/* Function to copy an array*/
void copyArr(char *arr,char *s)
{
	int i=0;
	if(strlen(s)==0||s==NULL)
	{
		arr[0]=' ';
		return;
	}
	while(s[i]!=END)
	{
		arr[i]=s[i];
		i++;
		
		
	}
	arr[i]='\0';

}
/*function to jump with the pointer to the command above all the white tags(tabs and apac..)*/
void jumpspace(char **s)
{
int i=0;
int j=0;
	while(isspace((*s)[i])){
		i++;
	}
	while(j<strlen(*s))
	{
		(*s)[j]=(*s)[j+i];
		j++;
	}
}

/*
this function chackes if threr is a macro name and if so the function prints to the file the macro command and if not the function return 0=fals*
*/
int addMacro(FILE *newf,list *macro_list,char *first_word_in_line)
{
	int flage=0;
	macros *m=(*(*macro_list).head).macro_var;
	if((*macro_list).count==0)
	{
		return 0;
	}
	first_word_in_line[strlen(first_word_in_line)]=END;
	while(m!=NULL)
	{
		if(strcmp((*m).macro_name,first_word_in_line)==0)/*the first word in line is a macro name*/
		{
			flage=1;
			if((*m).index>1)
				fprintf(newf,"\n");
			fprintf(newf,"%s",(*m).macro_line);
		}
		m=(*m).next_macro;
	}/*end while*/
return flage;
}

/*
this function 
*/
void put_op(char op,types sort_source_type,types sort_des_type,int sort)
{
unsigned short temp=1;
unsigned short x=0;
	if(sort==jump_sort)
	{
	temp=(short)sort_source_type<<12;
	x=temp;
	temp=(short)sort_des_type<<10;
	x=temp|x;
	sort_source_type=DEFOALT;
	sort_des_type=jump_sort;
	}

	temp=(short)op<<6;
	x=temp|x;
	temp=(short)sort_source_type<<4;
	x=x|temp;
	temp=(short)sort_des_type<<2;
	x=x|temp;
	print_binary(x);
}

void print_binary(unsigned short a)
{
char decoding[14];
int temp=0;
int i=0;
unsigned short x=a; /* A variable to make the changes of a on it*/
int length=13; /* amount of bits in this compiler*/
temp= (~(~0<<(length+1)));
a=( (a) & (temp));
temp=temp>>1;
	while(length>=0 )
	{
	x=a>>(length);
	if(x==0)
		decoding[i]='.';
	else
		decoding[i]='/';
	length=length-1;
	a=( (a) & (temp));
	temp=temp>>1;	
	i++;
	}
insertPrint(print_list,decoding,bynary);
}




/*
This method encodes to a temporarily file the rest of the lines of code
*/
void next_binary_line(sort sorting,int *count_decod_lines)
{
unsigned short temp=1;
unsigned short x=0;
/* if the two operand are reg sort  and if thre is no source operand threre is just one binary line for the operand*/
		if((sorting.sort_source_type==reg_sort&&sorting.sort_des_type==reg_sort) || sorting.sort_source==0)
		{
		x=sorting.sort_source<<8;
		temp=sorting.sort_des<<2;
			if(sorting.sort_des_type==stright_sort)
			{	
			temp=sorting.sort_des;
			}
	
		x=temp|x;
		print_binary(x);
	
		}
		else
		{
			if(sorting.sort_source_type==midiat_sort)
				x=sorting.sort_source<<2;
			if(sorting.sort_source_type==reg_sort)
				x=sorting.sort_source<<8;
			if(sorting.sort_source_type==stright_sort)
				x=(sorting.sort_source);
			print_binary(x);
			
			if(sorting.sort_des_type==stright_sort)
				x=(sorting.sort_des);
			else
				x=sorting.sort_des<<2;
			print_binary(x);
			
		}
}

/*
this function initialize the command_tub with commandName,op,if threr is a operand_source and aoperand_des
*/
void start_command_struct(commands *command_tub, int index,char command_name[],char op,flages operand_source,flages operand_des)
{
/*command_tub[index].command_name=copyString(command_name);*/
copyArr(command_tub[index].command_name,command_name);
command_tub[index].op=op;
command_tub[index].operand_source=operand_source;
command_tub[index].operand_des=operand_des;
}

/*
this funcrion sets the sorting type and the value of the sorting operand and return a label in case there is
*/
void set_sort_typ(char line[],int *sorting,int *count_decod_lines,types *sorting_type,sort sort_data)
{ 
	int num=0;
	char temp_line[MAX_ERROR_LINE];
	lebels *l;
	char *copy_line;
	jumpspace(&line);
	if(strcmp(line,"stop")==0 || strcmp(line,"rts")==0)
	{
		*sorting=0;
		*sorting_type=0;
		return ;
	}
	*count_decod_lines=*count_decod_lines+1;
	
	if(line[0]==IMMIDIAT)
	{
		if(line[1]==NEGETIVE)
		{
			copyArr(line,line+2);
			while(isdigit(line[0]))
			{
				num=num*10+((int)line[0]-ASCI_ZERO);
				line=line+1;
			}
		*sorting=(num)*(-1);
		}
			
		else
		{
			if(line[1]=='+')
				copyArr(line,line+2);
			else
				copyArr(line,line+1);
			while(isdigit(line[0]))
			{
				num=num*10+((int)line[0]-ASCI_ZERO);
				line=line+1;
			}
			*sorting=num;
		}
		*sorting_type=midiat_sort;
		return ;
	}
	if(line[0]=='r')
	{
		if(sort_data.sort_source_type==reg_sort)
				*count_decod_lines=*count_decod_lines-1;
		*sorting=(int)line[1]-ASCI_ZERO;
		*sorting_type=reg_sort;
		return ;
	}
	copy_line=copyString(line);
	jumpspace(&copy_line);
	l=get_leble_adress(leble_list,copy_line);
	if(l==NULL)/*this lable is not found-error*/
	{
		
		insertPrint(print_list,"Erroe-this lable has not defined",error);
		sprintf(temp_line,"The wrong lable:%s\tIn line:%d\n",copy_line,lineNum);
		insertPrint(print_list,temp_line,error);
		insertPrint(print_list,"----------------------------------------------",error);
		free(copy_line);	
		return ;
	}
	*sorting= ((*l).label_address<<2)|((*l).era);
	free(copy_line);
		if(strrchr(line,'(')==NULL)/* if the char '(' is not after the leble if the command is just a */
		{
			if(l!=NULL&&(*l).era==external_leble)
			{
				sprintf(temp_line,"%s\t%d",(*l).leble_name,*count_decod_lines);
				insertPrint(print_list,temp_line,externy);/*insert the line of externy lable into print_list*/
			}
			*sorting_type=stright_sort;
			return ;
		}
	
	if(l!=NULL&&(*l).era==external_leble)
	{
		sprintf(temp_line,"%s\t%d",(*l).leble_name,*count_decod_lines);
		insertPrint(print_list,temp_line,externy);
	}
	*sorting_type=jump_sort;
	return ;
}
/*check if there is a command in this line and if so get the op command*/
int comper(commands *command_tub,char command_name[])
{
int i;

	for(i=0;i<COMMAND_NUM;i++)
	{
		if(strcmp(command_tub[i].command_name,command_name)==0)
		{
			return i;
		}
	}
return -1;
}

void printings(char *fileName)
{
	if(printTypeLine(fileName,error)==ON)
	{
		return;
	}
	strcat(fileName,".ext");
	printTypeLine(fileName,externy);
	strtok(fileName,".");
	
	strcat(fileName,".ent");
	printTypeLine(fileName,entry);
	strtok(fileName,".");
	strcat(fileName,".ob");
	printTypeLine(fileName,bynary);
}

flages printTypeLine(char *fileName,printType type)
{
	flages print_flage=OFF;
	FILE *f;
	if(type==error)
		return printLoop(NULL,type,fileName);
	if(!isTypeExist(type))
		return print_flage;
	if(!(f = fopen(fileName,"w")))
	{
		fprintf(stderr,"Eror unable to open the file\n");
		free(fileName);
		exit(0);
	}
	fseek(f, 0, SEEK_END);
	if(type==bynary)
		print_flage=printLoop(f,ic_dc,fileName);
	print_flage=printLoop(f,type,fileName);
		
	fclose(f);
	return print_flage;
}

flages printLoop(FILE *f,printType type,char *fileName)
{
	char line_number[4]="0099";

	flages print_flage=OFF;
	printLine *p;
	p=(*(*print_list).head).print_var;
	while(p!=NULL)
	{
			
		if((*p).type==type)
		{
			if(type==bynary&&strcmp((*p).line,"")!=0)
			{
				setLineNumber(line_number);
				fprintf(f,"%s\t",line_number);
			}
			print_flage=ON;
			if(f==NULL)
				fprintf(stdout,"%s:%s\n",fileName,(*p).line);
			else
				fprintf(f,"%s\n",(*p).line);
		}
                p=(*p).next_print;
	}
return print_flage;
}
void setLineNumber(char *line_number)
{
	int length=strlen(line_number)-1;
	while(length>=0)
	{
		if(line_number[length]<(9+ASCI_ZERO)){
			line_number[length]=line_number[length]+1;
			return;
		}
		if(line_number[length]==(9+ASCI_ZERO)){
			line_number[length]=ASCI_ZERO;
		}
	length--;
	}
}
flages isTypeExist(printType type)
{
	printLine *p;
	p=(*(*print_list).head).print_var;
	while(p!=NULL)
	{
			
		if((*p).type==type)
		{
			return ON;
		}
                p=(*p).next_print;
	}
return OFF;
}
