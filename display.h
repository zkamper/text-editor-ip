#include <graphics.h>
#include <cmath>
#include <iostream>

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

struct Toggle{
    Point b;
    int radius;
    Point center;
    char *text;
    int font = DEFAULT_FONT;
    int textColor = COLOR(12,17,17);
    int textBk = COLOR(221, 234, 235);
    int toggleWidth = 0;
    int toggleHeight = 0;
    int bkcolor;
    int oncolor;
    bool isSet = false;
};

struct Menu{
    Point b;
    int width;
    int height;
    int color;
    Button *toggledBy;
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

void drawToggle(Toggle toggle){
    int bkColor = getbkcolor();
    int color = getcolor();
    std::cout<<toggle.text<<": "<<toggle.bkcolor<<"\n";
    setcolor(toggle.bkcolor);
    setfillstyle(1,toggle.bkcolor);
    fillellipse(toggle.b.x+toggle.radius+5,toggle.b.y+toggle.toggleHeight/2,toggle.radius,toggle.radius);
    if(toggle.isSet == true)
    {
        setcolor(toggle.oncolor);
        setfillstyle(1,toggle.oncolor);
        fillellipse(toggle.b.x+toggle.radius+5,toggle.b.y+toggle.toggleHeight/2,toggle.radius*0.75,toggle.radius*0.75);
    }
    setcolor(toggle.textColor);
    settextstyle(toggle.font,0,0);
    if(toggle.text != NULL)
        outtextxy(toggle.b.x+10+2*toggle.radius,toggle.b.y+(toggle.toggleHeight-textheight(toggle.text))/2,toggle.text);
    setbkcolor(bkColor);
    setcolor(color);
}

