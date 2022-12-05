#include <cstring>

char* subStr(char* str, int left, int right) ///
{
 char* newstr = (char*)malloc(1000);
 strncpy(newstr,str+left, right-left+1);
 newstr[right-left+1]=0;
 return newstr;
}
