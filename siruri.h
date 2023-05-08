#include <cstring>
#include <time.h>

char* subStr(char* str, int left, int right)
{///de la left, pana la right inlcusiv
 ///se foloseste diferit fata de mai jos
    if(left<0)
        left = 0;
    if(right > strlen(str))
        right = strlen(str);
 char* newstr = (char*)malloc(1000);
 strncpy(newstr,str+left, right-left+1);
 newstr[right-left+1]=0;
 return newstr;
}

void inserare (char *str, char *strIns, int left, int right)
{
 char *temp;
 if(left<0)
        left = 0;
 if(right > strlen(str))
        right = strlen(str);
 temp=subStr(str,right,strlen(str)-1);
 str[left]=0;
 strcat(str,strIns);
 strcat(str,temp);
}

void stergere (char *str, int left, int right)
{
 strcpy(str+left,str+right);
}

char* getCurrentDate()
{
    time_t currDate = time(0);
    tm tstruct = *localtime(&currDate);
    return asctime(&tstruct);
}
