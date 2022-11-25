#include <iostream>
#include <graphics.h>
#include <winbgim.h>

using namespace std;

int main()
{
//  int gd = DETECT, gm;
//  initgraph(&gd, &gm, "");
  initwindow(800,600); int x = 400, y = 300, r;
  for (r = 25; r <= 125; r += 20)
      circle(x, y, r);

  cout<<"Hello Patrut!";

  getch();
  closegraph();
  return 0;
}
