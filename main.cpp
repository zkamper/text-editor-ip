#include <iostream>
#include <winbgim.h>
#include <graphics.h>
#include "display.h"
#define NMAX 10000
using namespace std;

double lengthError;
Button saveButton,copyButton,pasteButton,fontButton;

struct {int x, y, length; char c;} text[NMAX];

int lgtext;

void getButtonClick(int x,int y);
void setPosChar(char curr);

void drawIcons()
{
    saveButton.bkcolor = COLOR(177,187,188);
    copyButton.bkcolor = COLOR(177,187,188);
    pasteButton.bkcolor = COLOR(177,187,188);
    fontButton.bkcolor =  COLOR(177,187,188);

    saveButton.icon = "icons\\save_icon.gif";
    copyButton.icon = "icons\\copy_icon.gif";
    pasteButton.icon = "icons\\paste_icon.gif";

    saveButton.text = "Save";
    copyButton.text = "Copy";
    pasteButton.text = "Paste";
    fontButton.text = "Font";

    saveButton.iconWidth = pasteButton.iconWidth = copyButton.iconWidth = 32;
    saveButton.iconHeight = pasteButton.iconHeight = copyButton.iconHeight =  32;

    int offset = 5;
    saveButton.buttonWidth = pasteButton.buttonWidth = copyButton.buttonWidth = fontButton.buttonWidth = 125;
    saveButton.buttonHeight = pasteButton.buttonHeight = copyButton.buttonHeight = fontButton.buttonHeight = 40;

    saveButton.b.x = offset;
    copyButton.b.x = saveButton.b.x+saveButton.buttonWidth+offset;
    pasteButton.b.x = copyButton.b.x+copyButton.buttonWidth+offset;
    fontButton.b.x = pasteButton.b.x + pasteButton.buttonWidth+offset;

    saveButton.b.y = copyButton.b.y = pasteButton.b.y = fontButton.b.y = offset;

    drawButton(saveButton);
    drawButton(copyButton);
    drawButton(pasteButton);
    drawButton(fontButton);

    registermousehandler(WM_LBUTTONDOWN,getButtonClick);
}

int font=8; /// 8 - font recomandat, fara niciun offset
void setTextFont()
{lengthError=1;
 switch (font)
 {
  case 1:  settextstyle(TRIPLEX_FONT,0,0);     lengthError=1.3; break;
  case 2:  settextstyle(SMALL_FONT,0,10);      lengthError=1;   break;
  case 3:  settextstyle(SANS_SERIF_FONT,0,10); lengthError=1;   break;
  case 4:  settextstyle(GOTHIC_FONT,0,0);      lengthError=1.2; break;
  case 5:  settextstyle(SCRIPT_FONT,0,0);      lengthError=1;   break;
  case 6:  settextstyle(SIMPLEX_FONT,0,0);     lengthError=1.3; break;
  case 7:  settextstyle(TRIPLEX_SCR_FONT,0,0); lengthError=1;   break;
  case 8:  settextstyle(COMPLEX_FONT,0,20);     lengthError=1;   break;
  case 9:  settextstyle(EUROPEAN_FONT,0,0);    lengthError=1.3; break;
  case 10: settextstyle(BOLD_FONT,0,0);        lengthError=1.1; break;
  default: settextstyle(DEFAULT_FONT,0,0);     lengthError=1.2;   break;
 }
 setcolor(BLACK);
}

void getButtonClick(int x, int y)
{
    Button b[] = {copyButton,saveButton,pasteButton,fontButton};
    int buttCount = 4;
    for(int i = 0 ; i < buttCount;i++)
    {
        if(b[i].b.x <= x && x<=b[i].b.x+b[i].buttonWidth && b[i].b.y <= y && y<=b[i].b.y+b[i].buttonHeight)
            cout<<b[i].text<<" ";
        if(strcmp(b[i].text,"Font")==0)
        {
            font=(font+1)%11;
            setTextFont();
        }
    }
}

int winLength, winHeight;
void windowsInit()
{
 ///initializare fereastra
    winLength = 1280;
    winHeight = 720;
    initwindow(winLength, winHeight, "Notepad^2");
    setbkcolor(COLOR(221,234,235));
    cleardevice();
    drawIcons();
}



double offsetHeight, offsetLength;
double currWordLength=0;
int currWordStart=0;
int x=8, y;

void write(int left, int right)
{setTextFont();
 for (int i=left; i<=right; i++)
      if (text[i].c!=32 && text[i].c!=13)
         {
          bgiout<<text[i].c;
          outstreamxy(text[i].x,text[i].y);
         }
}

void setPosChar(char curr)
{setTextFont();
 text[lgtext].length=lengthError*textwidth(&curr);
 offsetHeight=textheight(&curr);
 if (curr==13)
    {
     y+=offsetHeight; x=8;
     currWordStart=currWordLength=0;
     return;
    }
 if (curr==32)
    {
     x+=text[lgtext].length;
     currWordLength=0;
     currWordStart=lgtext+1;
     return;
    }
 if (x+text[lgtext].length+8<=winLength)
    {
     bgiout<<curr;
     outstreamxy(x,y);

     text[lgtext].x=x; text[lgtext].y=y;
     x+=text[lgtext].length;
     currWordLength+=text[lgtext].length;
     return;
    }
 if (currWordLength+text[lgtext].length+16>winLength)
    {
     y=y+offsetHeight; x=8;

     bgiout<<curr;
     outstreamxy(x,y);

     text[lgtext].x=x; text[lgtext].y=y;
     x+=text[lgtext].length;
     currWordLength=text[lgtext].length;
     currWordStart=lgtext;
     return;
    }
 else
     {
      setfillstyle(0,COLOR(221,234,235));
      bar(0,saveButton.buttonHeight+10,winLength,winHeight);
      write(0,currWordStart-1);
      y=y+offsetHeight; x=8;
      for (int i=currWordStart; i<=lgtext; i++)
          {
           text[i].y=y; text[i].x=x; x+=text[i].length; ///bug
          }
      write(currWordStart,lgtext);
      currWordLength+=text[lgtext].length;
      return;
     }
}

void readText()
{
    char curr;
    curr=getch();
    while (curr!=27) ///escape
    {   text[lgtext].c=curr;
        setPosChar(curr);
        lgtext++;
        curr=getch();
    }

}

int main()
{
    windowsInit();
    y=saveButton.buttonHeight+10;
    readText();
    closegraph();
    return 0;
}
