#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define max(a,b) (((a)>(b))?(a):(b))
void fput4(FILE *fp, unsigned int a)
{
	fputc((a >> 24) & 0xFF, fp);
	fputc((a >> 16) & 0xFF, fp);
	fputc((a >> 8) & 0xFF, fp);
	fputc(a & 0xFF, fp);
}
typedef enum PPC_register
{
    PPC_r0 = 0x00,
    PPC_r1 = 0x01,
    PPC_r2 = 0x02,
    PPC_r3 = 0x03,
    PPC_r4 = 0x04,
    PPC_r5 = 0x05,
    PPC_r6 = 0x06,
    PPC_r7 = 0x07,
    PPC_r8 = 0x08,
    PPC_r9 = 0x09,
    PPC_r10 = 0x0a,
    PPC_r11 = 0x0b,
    PPC_r12 = 0x0c,
    PPC_r13 = 0x0d,
    PPC_r14 = 0x0e,
    PPC_r15 = 0x0f,
    PPC_r16 = 0x10,
    PPC_r17 = 0x11,
    PPC_r18 = 0x12,
    PPC_r19 = 0x13,
    PPC_r20 = 0x14,
    PPC_r21 = 0x15,
    PPC_r22 = 0x16,
    PPC_r23 = 0x17,
    PPC_r24 = 0x18,
    PPC_r25 = 0x19,
    PPC_r26 = 0x1a,
    PPC_r27 = 0x1b,
    PPC_r28 = 0x1c,
    PPC_r29 = 0x1d,
    PPC_r30 = 0x1e,
    PPC_r31 = 0x1f
} PPC_register;
typedef enum PPC_conditions
{
    PPC_equal = 0b0110000010,
    PPC_notEqual = 0b0010000010,
	PPC_greaterThan = 0b0110000001,
	PPC_lessThan = 0b0110000000,
	PPC_greaterEqualThan = 0b0010000000,
	PPC_lessEqualThan = 0b0010000001
} PPC_conditions;
unsigned int swap32(unsigned int num)
{
   unsigned int b0 = (num & 0x000000ff) << 24u;
   unsigned int b1 = (num & 0x0000ff00) << 8u;
   unsigned int b2 = (num & 0x00ff0000) >> 8u;
   unsigned int b3 = (num & 0xff000000) >> 24u;
    
    unsigned int res = b0 | b1 | b2 | b3;
    return res;
}
unsigned short swap16(short num)
{
    unsigned short b1 = (unsigned short)(num & 0x00ff) << 8u;
    unsigned short b2 = (unsigned short)(num & 0xff00) >> 8u;
    
    unsigned short res = b1 + b2;
    return res;
}
void PPC__branch(FILE *fp, unsigned int currentLoc, unsigned int jumpLoc)
{
    int newloc = (jumpLoc - currentLoc)/4;
    if(newloc == 0) //no location no need to jump
        return ;
    unsigned int loc = 0x48000000;
    //shift and filter out the upper 6 bits and lower 2.
    loc += ((newloc<<2)&0x03FFFFFC);
    fput4(fp,loc);
}
void PPC__Lbranch(FILE *fp, unsigned int currentLoc, unsigned int jumpLoc)
{
    int newloc = (jumpLoc - currentLoc)/4;
    if(newloc == 0) //no location no need to jump
        return ;
    unsigned int loc = 0x48000000;
    //shift and filter out the upper 6 bits and lower 2.
    loc += ((newloc<<2)&0x03FFFFFC);
	loc += 0b01;
    fput4(fp,loc);
}
void PPC__LRbranch(FILE *fp)
{
    fput4(fp,0x4E800020);
}
void PPC__Cbranch(FILE *fp,PPC_conditions condition, unsigned short shortjumpLoc)
{
    if(shortjumpLoc == 0) //no location no need to jump
        return ;
    unsigned int loc = 
    (0x40000000) + 
    (condition<<16) +
    ((shortjumpLoc<<2)&0xfffc);
    fput4(fp,loc);
}
void PPC__WStore(FILE *fp, PPC_register a, PPC_register b, unsigned short word)
{
    unsigned int out = 
    (0x90000000) +
    (a<<21) +
    (b<<16) +
    word;
    fput4(fp,out);
}
void PPC__WLoad(FILE *fp, PPC_register a, PPC_register b, unsigned short word)
{
    unsigned int out = 
    (0x80000000) +
    (a<<21) +
    (b<<16) +
    word;
    fput4(fp,out);
}
void PPC__HStore(FILE *fp, PPC_register a, PPC_register b, unsigned short word)
{
    unsigned int out = 
    (0xB0000000) +
    (a<<21) +
    (b<<16) +
    word;
    fput4(fp,out);
}
void PPC__HLoad(FILE *fp, PPC_register a, PPC_register b, unsigned short word)
{
    unsigned int out = 
    (0xA0000000) +
    (a<<21) +
    (b<<16) +
    word;
    fput4(fp,out);
}
void PPC__BStore(FILE *fp, PPC_register a, PPC_register b, unsigned short word)
{
    unsigned int out = 
    (0x98000000) +
    (a<<21) +
    (b<<16) +
    word;
    fput4(fp,out);
}
void PPC__BLoad(FILE *fp, PPC_register a, PPC_register b, unsigned short word)
{
    unsigned int out = 
    (0x88000000) +
    (a<<21) +
    (b<<16) +
    word;
    fput4(fp,out);
}
void PPC__FStore(FILE *fp, PPC_register a, PPC_register b, unsigned short word)
{
    unsigned int out = 
    (0xD0000000) +
    (a<<21) +
    (b<<16) +
    word;
    fput4(fp,out);
}
void PPC__FLoad(FILE *fp, PPC_register a, PPC_register b, unsigned short word)
{
    unsigned int out = 
    (0xC0000000) +
    (a<<21) +
    (b<<16) +
    word;
    fput4(fp,out);
}
void PPC__Cmp(FILE *fp, PPC_register a, PPC_register b)
{
    unsigned int out = 
    (0x7C000000) + 
    (a<<16) +
    (b<<11);
    fput4(fp,out);
}
void PPC__IWCmp(FILE *fp, PPC_register a, unsigned short word)
{
    unsigned int out = 
    (0x2C000000) + 
    (a<<16) +
    word;
    fput4(fp,out);
}
void PPC__IAdd(FILE *fp, PPC_register a, PPC_register b, unsigned short word)
{
    unsigned int out = 
    (0x38000000) +
    (a<<21) +
    (b<<16) +
    word;
    fput4(fp,out);
}
void PPC__ISAdd(FILE *fp, PPC_register a, PPC_register b, unsigned short word)
{
    unsigned int out = 
    (0x3C000000) +
    (a<<21) +
    (b<<16) +
    word;
    fput4(fp,out);
}
void PPC__ISub(FILE *fp, PPC_register a, PPC_register b, unsigned short word)
{
    unsigned int out = 
    (0x20000000) +
    (a<<21) +
    (b<<16) +
    word;
    fput4(fp,out);
}
void PPC__IMull(FILE *fp, PPC_register a, PPC_register b, unsigned short word)
{
    unsigned int out = 
    (0x1C000000) +
    (a<<21) +
    (b<<16) +
    word;
    fput4(fp,out);
}
void PPC__RAnd(FILE *fp, PPC_register a, PPC_register b, PPC_register c)
{
	unsigned int out =
	(0x7C000000) +
	(a<<21) +
	(c<<16) +
	(b<<11) +
	(0x114);
	fput4(fp,out);
}
void PPC__WDiv(FILE *fp, PPC_register a, PPC_register b, PPC_register c)
{
	unsigned int out =
	(0x7C000000) +
	(c<<21) +
	(a<<16) +
	(b<<11) +
	0x3D6;
	fput4(fp,out);
}
void PPC__ILoad(FILE *fp, PPC_register a, unsigned short word)
{
	unsigned int out =
	(0x38000000) +
	(a<<21) +
	word;
    fput4(fp, out);
}
void PPC__Fadd(FILE *fp, PPC_register a, PPC_register b, PPC_register c)
{
	unsigned int out =
	(0xFC000000) +
	(a<<21) +
	(c<<16) +
	(b<<11) +
	(0x2A);
	fput4(fp,out);
}
void PPC__Fsub(FILE *fp, PPC_register a, PPC_register b, PPC_register c)
{
	unsigned int out =
	(0xFC000000) +
	(a<<21) +
	(c<<16) +
	(b<<11) +
	(0x28);
	fput4(fp,out);
}
void PPC__Fmul(FILE *fp, PPC_register a, PPC_register b, PPC_register c)
{
	unsigned int out =
	(0xFC000000) +
	(a<<21) +
	(c<<16) +
	(b<<6) +
	(0x32);
	fput4(fp,out);
}
void PPC__Fdiv(FILE *fp, PPC_register a, PPC_register b, PPC_register c)
{
	unsigned int out =
	(0xFC000000) +
	(a<<21) +
	(c<<16) +
	(b<<11) +
	(0x24);
	fput4(fp,out);
}
int isalnumstr(const char *str) //check if string has numbers or letters
{
    int i;

    for (i = 0; str[i]; ++i)
        if (isalnum((unsigned char) str[i]))
            return 1;
    return 0;
}
int checkforif(FILE *fp, char *currentline) //check for an if statement
{
	char line[256];
	int ifstatment = 0, endo = 0, count = 0;
	if(strstr(currentline, "if")) // we are in an if statement
	{
		ifstatment = ftell(fp);
		++endo;
	}
	while(fgets(line, sizeof(line), fp))
	{
		if(strstr(line, "{"))
		{
			++count;
			++endo;
		}
		if(strstr(line, "}"))
			--endo;
		if(!endo)
			break;
		if(isalnumstr(line)) //has code
			++count;
	}
	fseek(fp, ifstatment, SEEK_SET);
	return count;
}
//get word locations seperated by \0, \n, or spaces
int *getoperations(char *line, unsigned int size, unsigned int *countOut) 
{
	int count = 0;
	for(int i = 0; i < size; ++i)
	{
		if(line[i] == ' ')
			count++;
		if(line[i] == '\n' || line[i] == '\0')
		{
			count++;
			break;
		}
	}
	int *locations = malloc(sizeof(int) * count + 1);
	locations[0] = 0;
	count = 1;
	for(int i = 1; i < size; ++i)
	{
		if(line[i] == ' ')
			locations[count++] = i + 1;
		if(line[i] == '\n' || line[i] == '\0')
		{
			locations[count++] = i;
			break;
		}
	}
	*countOut = count;
	return locations;
} 
//get section of text out of a block of text
char *getsub(char *buff, unsigned int offset, unsigned int size)
{
	char *subbuff = malloc(size * sizeof(char));
	memcpy( subbuff, &buff[offset], size );
	subbuff[size] = '\0';
	return subbuff;
}
unsigned char PPC_register_list[32] =
{
    0x00,
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09,
    0x0a,
    0x0b,
    0x0c,
    0x0d,
    0x0e,
    0x0f,
    0x10,
    0x11,
    0x12,
    0x13,
    0x14,
    0x15,
    0x16,
    0x17,
    0x18,
    0x19,
    0x1a,
    0x1b,
	0x1c,
    0x1d,
    0x1e,
    0x1f
};
int estrstr(char *line, char *word)
{
	char* p = strstr(line, word);
	if ((p==line) || (p!=NULL && !isalnum((unsigned char)p[-1])))
	{
		p += strlen(word);
		if (!isalnum((unsigned char)*p))
		{
			return 1;
		}
	}
	return 0;
}
char getRegister(char *line)
{
	//printf("reg test : %s\n", line);
	char str[12];
	for(int i = 0; i < 32; ++i)
	{
		//check for general purpose registers
		sprintf(str, "r%d", i);
		//printf("%s\n", str);
        char* p = strstr(line, str); //make sure word is 100% that word
        if ((p==line) || (p!=NULL && !isalnum((unsigned char)p[-1])))
        {
            p += strlen(str);
            if (!isalnum((unsigned char)*p))
                return PPC_register_list[i];
        }
		
		//check for float registers.
		sprintf(str, "f%d", i);
		//printf("%s\n", str);
        char* p = strstr(line, str); //make sure word is 100% that word
        if ((p==line) || (p!=NULL && !isalnum((unsigned char)p[-1])))
        {
            p += strlen(str);
            if (!isalnum((unsigned char)*p))
                return PPC_register_list[i];
				//float registers use the same hex as regular registers.
        }
	}
	return -1;
}
int checkError(int line, int amount, int needed, char *errmessage)
{
	if(amount < needed)
	{
		printf("error line %i: %s\n", line, errmessage);
		return 1;
	}
	return 0;
}
unsigned int getFunctionCount(FILE *fp)
{
	char line[256];
	unsigned int count = 0;
	while(fgets(line, sizeof(line), fp)) //keep getting lines of assembly
		if(strstr(line, "$"))
			++count;
	rewind(fp);
	return count;
}
void assemble(char *filename)
{
	FILE *wp = fopen("output.bin", "w");
	FILE *rp = fopen(filename, "r");
	if(rp == NULL)
	{
		printf("unable to open file\n");
		return ;
	}
	
	int t = 0;
	char line[256];
	char functionName[256];
	unsigned int linenumber = 0, offset = 0, hasOffset = 0, functionCount = 0;
	unsigned int debuglinenumber = 0;
	char check = 0, error = 0;
	
	unsigned int functionTotal = getFunctionCount(rp);
	printf("count : %i\n", functionTotal);
	char **functionNames = malloc(functionTotal * sizeof(char *));
	for(int i = 0; i < functionTotal; ++i)
		functionNames[i] = malloc(256);
	unsigned int *functionOffsets = malloc(functionTotal * sizeof(unsigned int));
	while(fgets(line, sizeof(line), rp)) //keep getting lines of assembly
	{
		++linenumber; //keep track of line number
		++debuglinenumber;
		unsigned int subcount = 0, regcount = 0, numbercount = 0;
		int *subs = getoperations(line, 256, &subcount);
		char *code = getsub(line, subs[0], subs[1] - subs[0]);
		char *second = NULL;
		if(subcount > 2)		
			second = getsub(line, subs[1], subs[2] - subs[1]);
		//printf("code : %s\n", code);
		unsigned char registers[5] = {0,0,0,0,0};
		unsigned int numbers[5] = {0,0,0,0,0};
		for(int i = 0; i < subcount-1; ++i) //get each part of the assembly code including adresses, registers and opcode
		{
			char *sub = getsub(line, subs[i], subs[i+1] - subs[i]);
			//get current register, if negative one a register wasn't given.
			if((check = getRegister(sub)) != -1) 
				registers[regcount++] = check;
			if(strstr(sub, "0x")) //hex number
				numbers[numbercount++] = strtoul(sub, NULL, 16);
			if(strstr(sub, "#")) //memory offset area
			{
				--linenumber; //keep track of line number
				hasOffset = 1;
				offset = (int)strtoul(sub + 3, NULL, 16);
			}
			if(strstr(sub, "$")) //function area
			{
				--linenumber; //keep track of line number
				for(int j = 0; j < functionCount; ++j)
					if(estrstr(functionNames[j], sub + 1))
						error = 1;
				if(!error)
				{
					//set function name and memory offset pairs.
					printf("function in!\n");
					strcpy(functionNames[functionCount], sub + 1);
					functionOffsets[functionCount] = linenumber * 4;
					printf("function: %s offset: %u\n", functionNames[functionCount], functionOffsets[functionCount]);
					functionCount++;
				}
				else
					printf("error line %i: duplicate function name!\n", debuglinenumber);
			}
			if(strstr(sub, ";")) //comment
			{
				break;
				if(!i)
					--linenumber;
			}
			free(sub);
		}
		//printf("line %i code %s\n", linenumber, code);
		//check for assembly codes and errors
		if(estrstr(code,"bne"))
		{
			PPC__Cbranch(wp, PPC_notEqual, numbers[0]);
			if(checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"beq"))
		{
			PPC__Cbranch(wp, PPC_equal, numbers[0]);
			if(checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"blt"))
		{
			PPC__Cbranch(wp, PPC_lessThan, numbers[0]);
			if(checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}		
		else if(estrstr(code,"bgt"))
		{
			PPC__Cbranch(wp, PPC_greaterThan, numbers[0]);
			if(checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"ble"))
		{
			PPC__Cbranch(wp, PPC_lessEqualThan, numbers[0]);
			if(checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"bge"))
		{
			PPC__Cbranch(wp, PPC_greaterEqualThan, numbers[0]);
			if(checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"cmpwi"))
		{
			PPC__IWCmp(wp, registers[0], numbers[0]);
			if(checkError(debuglinenumber, regcount, 1, "forgot a register") 
			|| checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"cmpw"))
		{
			PPC__Cmp(wp, registers[0], registers[1]);
			if(checkError(debuglinenumber, regcount, 2, "forgot a register(s)"))
				error = 1;
		}
		else if(estrstr(code,"addi"))
		{
			PPC__IAdd(wp, registers[0], registers[1], numbers[0]);
			if(checkError(debuglinenumber, regcount, 2, "forgot a register(s)") 
			|| checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"addis"))
		{
			PPC__ISAdd(wp, registers[0], registers[1], numbers[0]);
			if(checkError(debuglinenumber, regcount, 2, "forgot a register(s)") 
			|| checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"subi"))
		{
			PPC__ISub(wp, registers[0], registers[1], numbers[0]);
			if(checkError(debuglinenumber, regcount, 2, "forgot a register(s)") 
			|| checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"mull"))
		{
			PPC__IMull(wp, registers[0], registers[1], numbers[0]);
			if(checkError(debuglinenumber, regcount, 3, "forgot a register(s)"))
				error = 1;
		}
		else if(estrstr(code,"and"))
		{
			PPC__RAnd(wp, registers[0], registers[1], registers[2]);
			if(checkError(debuglinenumber, regcount, 3, "forgot a register(s)"))
				error = 1;
		}
		else if(estrstr(code,"divw"))
		{
			PPC__WDiv(wp, registers[0], registers[1], registers[2]);
			if(checkError(debuglinenumber, regcount, 3, "forgot a register(s)"))
				error = 1;
		}
		else if(estrstr(code,"lis"))
		{
			PPC__ISAdd(wp, registers[0], 0x00, numbers[0]);
			if(checkError(debuglinenumber, regcount, 1, "forgot a register(s)") 
			|| checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"li"))
		{
			PPC__ILoad(wp, registers[0], numbers[0]);
			if(checkError(debuglinenumber, regcount, 1, "forgot a register") 
			|| checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"stw"))
		{
			PPC__WStore(wp, registers[0], registers[1], numbers[0]);
			if(checkError(debuglinenumber, regcount, 2, "forgot a register(s)") 
			|| checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"stb"))
		{
			PPC__BStore(wp, registers[0], registers[1], numbers[0]);
			if(checkError(debuglinenumber, regcount, 2, "forgot a register(s)") 
			|| checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"lwz"))
		{
			PPC__WLoad(wp, registers[0], registers[1], numbers[0]);
			if(checkError(debuglinenumber, regcount, 2, "forgot a register(s)") 
			|| checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"lbz"))
		{
			PPC__BLoad(wp, registers[0], registers[1], numbers[0]);
			if(checkError(debuglinenumber, regcount, 2, "forgot a register(s)") 
			|| checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"lfs"))
		{
			PPC__FLoad(wp, registers[0], registers[1], numbers[0]);
			if(checkError(debuglinenumber, regcount, 2, "forgot a register(s)") 
			|| checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"stfs"))
		{
			PPC__FStore(wp, registers[0], registers[1], numbers[0]);
			if(checkError(debuglinenumber, regcount, 2, "forgot a register(s)") 
			|| checkError(debuglinenumber, numbercount, 1, "forgot a number"))
				error = 1;
		}
		else if(estrstr(code,"fdiv"))
		{
			PPC__Fdiv(wp, registers[0], registers[1], registers[2]);
			if(checkError(debuglinenumber, regcount, 3, "forgot a register(s)"))
				error = 1;
		}
		else if(estrstr(code,"fmul"))
		{
			PPC__Fmul(wp, registers[0], registers[1], registers[2]);
			if(checkError(debuglinenumber, regcount, 3, "forgot a register(s)"))
				error = 1;
		}
		else if(estrstr(code,"fadd"))
		{
			PPC__Fadd(wp, registers[0], registers[1], registers[2]);
			if(checkError(debuglinenumber, regcount, 3, "forgot a register(s)"))
				error = 1;
		}
		else if(estrstr(code,"fsub"))
		{
			PPC__Fsub(wp, registers[0], registers[1], registers[2]);
			if(checkError(debuglinenumber, regcount, 3, "forgot a register(s)"))
				error = 1;
		}
		else if(estrstr(code,"b"))
		{
			if(hasOffset) //if the file has a predefined offset use it
			{
				PPC__branch(wp, offset + (linenumber * 4) - 4, numbers[0]);
				if(checkError(debuglinenumber, numbercount, 1, "forgot a number"))
					error = 1;
			}
			else if(numbercount == 2)
			{
				PPC__branch(wp, numbers[0], numbers[1]);
				if(checkError(debuglinenumber, numbercount, 2, "forgot a number(s)"))
					error = 1;
			}
			else if(second)
			{
				unsigned int ending_offset = hasOffset ? offset + (linenumber * 4) - 4: (linenumber * 4) - 4; 
				unsigned int start_offset = ending_offset;
				int success = 0;
				for(int j = 0; j < functionCount; ++j)
					if(strstr(functionNames[j], second))
					{
						if(ending_offset > functionOffsets[j])
							ending_offset = ending_offset - functionOffsets[j];
						else
							ending_offset = ending_offset + functionOffsets[j];
						success = 1;
					}
				if(success)
					PPC__branch(wp, start_offset, ending_offset);
				else
					printf("error line %i: branch is calling nothing!\n", debuglinenumber);
			}
			else
				printf("error line %i: branch is calling nothing!\n", debuglinenumber);
		}
		else if(estrstr(code,"bl"))
		{
			if(hasOffset) //if the file has a predefined offset use it
			{
				PPC__Lbranch(wp, offset + (linenumber * 4) - 4, numbers[0]);
				if(checkError(debuglinenumber, numbercount, 1, "forgot a number"))
					error = 1;
			}
			else if(numbercount == 2)
			{
				PPC__Lbranch(wp, numbers[0], numbers[1]);
				if(checkError(debuglinenumber, numbercount, 2, "forgot a number(s)"))
					error = 1;
			}
			else if(second)
			{
				unsigned int ending_offset = hasOffset ? offset + (linenumber * 4) - 4: (linenumber * 4) - 4;  
				unsigned int start_offset = ending_offset;
				int success = 0;
				for(int j = 0; j < functionCount; ++j)
					if(strstr(functionNames[j], second))
					{
						if(ending_offset > functionOffsets[j])
							ending_offset = ending_offset - functionOffsets[j];
						else
							ending_offset = ending_offset + functionOffsets[j];
						success = 1;
					}
				if(success)
					PPC__Lbranch(wp, start_offset, ending_offset);
				else
					printf("error line %i: branch is calling nothing!\n", debuglinenumber);
			}
			else
				printf("error line %i: branch is calling nothing!\n", debuglinenumber);
		}
		else if(estrstr(code, "blr"))
			PPC__LRbranch(wp);
		if(error)
			break;
	}
	if(error)
		printf("error!\n");
	for(int i = 0; i < functionTotal; ++i)
		free(functionNames[i]);
	free(functionNames);
	free(functionOffsets);
	fclose(rp);
	fclose(wp);
}
void edit(char *filename, unsigned long int location, unsigned int edit)
{
	FILE *fp = fopen(filename, "w");
	fseek(fp, location, SEEK_SET);
	fput4(fp, edit);
	fclose(fp);
}
int main(int argc, char **argv)
{
	if(argc == 1)
	{
		printf("No input file!\n");
		return 0;
	}
	if(argc == 3 && strstr(argv[1], "-c")) // compile
		assemble(argv[2]);
	if(argc == 5 && strstr(argv[1], "-e")) // edit
		edit(argv[2], strtoul(argv[3], NULL, 16), strtoul(argv[4], NULL, 16));
	printf("%s\n", argv[3]);
	return 0;
}