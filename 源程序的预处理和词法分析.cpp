#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char s[100000];			//存储源程序文本内容 
char p[100000];			//存储预处理后的源程序文本
char t[100]; 			//存放暂时取得的单词符号 
int lengths = 0;			//记录源程序文本的长度
int lengthp = 0;			//存储预处理后的源程序文本的长度 
int length = 0;

void preprocessing(char s[], int lengths);//预处理子程序 
void LexicalAnalysis(char p[], int lengthp);//词法分析程序 


int main()
{
	FILE* sf;			//指向源程序文本 
	char c;

	if ((fopen_s(&sf, "C:\\Users\\王新涛\\Desktop\\编译原理实验报告\\实验1   源程序的预处理和词法分析程序设计\\source file.txt", "r")))
	{
		printf("Cann't open the source file!\n");
		exit(0);
	}

	//将源程序文本内容存储到数组s中 
	while ((c = fgetc(sf)) != EOF)
	{
		s[lengths] = c;
		lengths++;
	}
	fclose(sf);

	preprocessing(s, lengths);//预处理 
	LexicalAnalysis(p, lengthp);//词法分析 

	system("pause");
	return 0;
}

//预处理子程序 
void preprocessing(char s[], int lengths)
{
	FILE* pf;				//指向预处理后的源程序文本

	if ((fopen_s(&pf, "C:\\Users\\王新涛\\Desktop\\编译原理实验报告\\实验1   源程序的预处理和词法分析程序设计\\preprocessing file.txt", "w")))
	{
		printf("Cann't open the preprocessing file!\n");
		exit(0);
	}

	for (int i = 0; i < lengths; i++)
	{
		//去掉源程序文本中的回车符、换行符、制表符 
		if (s[i] == '\n' || s[i] == '\t')
			continue;

		//将源程序文本中的多个连续的空白符合并为一个 
		if (s[i] == ' ')
		{
			p[lengthp] = s[i];
			lengthp++;
			i++;
			while (s[i] == ' ')
				i++;
			i--;
			continue;
		}

		//去掉单行注释 
		if (s[i] == '/' && s[i + 1] == '/')
		{
			while (s[i] != '\n')
				i++;
			continue;
		}

		//去掉多行注释
		if (s[i] == '/' && s[i + 1] == '*')
		{
			i = i + 2;
			for (;;)
			{
				if (s[i] == '*' && s[i + 1] == '/')
					break;
				i++;
			}
			i = i + 1;
			continue;
		}
		/*
			Deserve-Wxt
		*/

		p[lengthp] = s[i];
		lengthp++;
	}
	for (int i = 0; i < lengthp; i++)
	{
		printf("%c", p[i]);
		fputc(p[i], pf);
	}
	printf("\n\n");
	fclose(pf);
	printf("Preprocessing Finished!\n\n");
}


//词法分析程序
void LexicalAnalysis(char p[], int lengthp)
{
	FILE* bf;
	int syn;

	if ((fopen_s(&bf, "C:\\Users\\王新涛\\Desktop\\编译原理实验报告\\实验1   源程序的预处理和词法分析程序设计\\binary file.txt", "w")))
	{
		printf("Cann't open the binary file!\n");
		exit(0);
	}

	char const* keyword[26] = { "main","if","then","while","do","static","int","double","struct","break","else","long","switch","case","typedef","char","return","const","float","short","continue","for","void","sizeof","default","do" };

	for (int i = 0; i < lengthp; i++)
	{
		if (p[i] == '#')			syn = 0;
		else if (p[i] == '+')		syn = 27;
		else if (p[i] == '-')		syn = 28;
		else if (p[i] == '/')		syn = 30;
		else if (p[i] == '[')		syn = 39;
		else if (p[i] == ']')		syn = 40;
		else if (p[i] == ';')		syn = 41;
		else if (p[i] == '(')		syn = 42;
		else if (p[i] == ')')		syn = 43;
		else if (p[i] == '*')
		{
			if (p[i + 1] == '*') { syn = 31; i++; }
			else 				syn = 29;
		}
		else if (p[i] == '=')
		{
			if (p[i + 1] == '=') { syn = 32; i++; }
			else 				syn = 38;
		}
		else if (p[i] == '>')
		{
			if (p[i + 1] == '=') { syn = 37; i++; }
			else 				syn = 36;
		}
		else if (p[i] == '<')
		{
			if (p[i + 1] == '>') { syn = 34; i++; }
			else if (p[i + 1] == '=') { syn = 35; i++; }
			else 				  syn = 33;
		}
		else if (p[i] == ' ')	continue;
		else if ((p[i] >= 'a' && p[i] <= 'z') || (p[i] >= 'A' && p[i] <= 'Z'))
		{
			length = 0;
			t[length++] = p[i++];
			while ((p[i] >= 'a' && p[i] <= 'z') || (p[i] >= 'A' && p[i] <= 'Z') || (p[i] >= '0' && p[i] <= '9'))
				t[length++] = p[i++];
			t[length] = '\0';
			int k;
			for (k = 0; k < 26; k++)
			{
				if (strcmp(keyword[k], t) == 0)
					break;
			}
			if (k < 24)
				syn = k + 1;
			else if (k == 24)
				syn = 44;
			else
				syn = 25;
			i--;
		}
		else if (p[i] >= '0' && p[i] <= '9')
		{
			length = 0;
			t[length++] = p[i++];
			while (p[i] >= '0' && p[i] <= '9')
				t[length++] = p[i++];
			t[length] = '\0';
			syn = 26;
			i--;
		}
		else if ((p[i] >= '0' && p[i] <= '9') && ((p[i + 1] >= 'a' && p[i + 1] <= 'z') || (p[i + 1] >= 'A' && p[i + 1] <= 'Z') || (p[i + 1] >= '0' && p[i + 1] <= '9')))
		{
			syn = -1;
			length = 0;
			t[length++] = p[i++];
			while ((p[i] >= 'a' && p[i] <= 'z') || (p[i] >= 'A' && p[i] <= 'Z') || (p[i] >= '0' && p[i] <= '9'))
				t[length++] = p[i++];
			t[length] = '\0';
			i--;
		}

		else
		{
			syn = -1;
			t[0] = p[i];
			t[1] = '\0';
		}

		if (syn == 0 || (syn >= 27 && syn <= 30) || syn == 33 || syn == 36 || (syn >= 38 && syn <= 43))
		{
			printf("(	%d	,	%c	)\n", syn, p[i]);
			fprintf(bf, "(	%d	,	%c	)\n", syn, p[i]);
		}
		else if (syn == 31 || syn == 32 || syn == 34 || syn == 35 || syn == 37)
		{
			printf("(	%d	,	%c%c	)\n", syn, p[i - 1], p[i]);
			fprintf(bf, "(	%d	,	%c%c	)\n", syn, p[i - 1], p[i]);
		}
		else if (syn >= 1 && syn <= 24)
		{
			printf("(	%d	,	%s	)\n", syn, keyword[syn - 1]);
			fprintf(bf, "(	%d	,	%s	)\n", syn, keyword[syn - 1]);
		}
		else if (syn == 44)
		{
			printf("(	%d	,	%s	)\n", syn, keyword[24]);
			fprintf(bf, "(	%d	,	%s	)\n", syn, keyword[24]);
		}
		else if (syn == 25 || syn == 26)
		{
			printf("(	%d	,	%s	)\n", syn, t);
			fprintf(bf, "(	%d	,	%s	)\n", syn, t);
		}
		else if (syn == -1)
		{
			printf("(	error	,	%s	)\n", t);
			fprintf(bf, "(	error	,	%s	)\n", t);
		}
	}
}