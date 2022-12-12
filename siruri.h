#include <cstring>

char* subStr(char* str, int left, int right) ///
{
    if(left<0)
        left = 0;
    if(right > strlen(str))
        right = strlen(str);
 char* newstr = (char*)malloc(1000);
 strncpy(newstr,str+left, right-left+1);
 newstr[right-left+1]=0;
 return newstr;
}
