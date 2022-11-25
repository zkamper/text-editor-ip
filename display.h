#include <graphics.h>
#include <cmath>

struct Point{
    int x;
    int y;
};

struct Button{
    Point b;
    int buttonWidth;
    int buttonHeight;
    char *icon;
    int iconWidth;
    int iconHeight;
    char *text;
    int bkcolor;
};

void drawButton(Button button){
    setfillstyle(1,button.bkcolor);
    bar(button.b.x,button.b.y,button.b.x+button.buttonWidth,button.b.y+button.buttonHeight);
    int offset = abs(button.buttonHeight-button.iconHeight)/2;
    readimagefile(button.icon,button.b.x+offset,button.b.y+offset,button.b.x+offset+button.iconWidth,button.b.y+offset+button.iconHeight);
    int bkColor = getbkcolor();
    int color = getcolor();
    setbkcolor(button.bkcolor);
    setcolor(COLOR(42,47,47));
    settextstyle(DEFAULT_FONT,0,0);
    outtextxy(button.b.x+button.buttonHeight,button.b.y+(button.buttonHeight-textheight(button.text))/2,button.text);
    setbkcolor(bkColor);
    setcolor(color);
}