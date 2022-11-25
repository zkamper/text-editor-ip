#include <iostream>
#include <graphics.h>
using namespace std;

int winlenght, winheight;

int main()
{
  winlenght = 1280;
  winheight = 720;
  initwindow(winlenght, winheight, "Notepad^2");

  setbkcolor(COLOR(221,234,235));
  cleardevice();

  getch();
  closegraph();
  return 0;
}
