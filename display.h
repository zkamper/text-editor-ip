#include <graphics.h>
#include <cmath>

struct Point{
    int x;
    int y;
};

struct Button{
    Point b;
    int buttonWidth = 0;
    int buttonHeight = 0;
    char *icon;
    int iconWidth = 0;
    int iconHeight = 0;
    char *text;
    int bkcolor;
    int font = DEFAULT_FONT;
    int textColor = COLOR(12,17,17);
};

void drawButton(Button button){
    setfillstyle(1,button.bkcolor);
    bar(button.b.x,button.b.y,button.b.x+button.buttonWidth,button.b.y+button.buttonHeight);
    int offset = abs(button.buttonHeight-button.iconHeight)/2;
    if(button.icon != NULL) readimagefile(button.icon,button.b.x+offset,button.b.y+offset,button.b.x+offset+button.iconWidth,button.b.y+offset+button.iconHeight);
    int bkColor = getbkcolor();
    int color = getcolor();
    setbkcolor(button.bkcolor);
    setcolor(button.textColor);
    settextstyle(button.font,0,0);
    if(button.icon != NULL && button.text !=NULL )
        outtextxy(button.b.x+button.buttonHeight,button.b.y+(button.buttonHeight-textheight(button.text))/2,button.text);
    else if(button.text != NULL)
    {
        outtextxy(button.b.x+button.buttonWidth/2-textwidth(button.text)/2,button.b.y+button.buttonHeight/2-textheight(button.text)/2,button.text);
    }
    setbkcolor(bkColor);
    setcolor(color);
}
