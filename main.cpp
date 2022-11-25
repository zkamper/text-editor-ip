#include <iostream>
#include <graphics.h>
using namespace std;

int winlenght, winheight;

int main()
{
 winlenght=1280;
 winheight=720;
 initwindow(winlenght, winheight, "Notepad^2");


 int x = 400, y = 300, r;
 line(x,y,x+100,y+100);


 getch();
 closegraph();
 return 0;
}
