#include <iostream>
#include <winbgim.h>
#include <graphics.h>
#include <cmath>
#include "display.h"
#include "siruri.h"
#define NMAX 10000
#include <fstream>
#include <stdio.h>
//#include <thread>
#include <windows.h>

using namespace std;

double lengthError;
Button saveButton, copyButton, pasteButton, fontButton;
Toggle wordWrap;
int font = 8; /// 8 - font recomandat, fara niciun offset
double winLength, winHeight;
int bkColor = COLOR(221, 234, 235);      // Culoarea de fundal;
int accentColor1 = COLOR(177, 187, 188); // Fundal butoane
int accentColor2 = COLOR(99, 110, 109);  // Conturul de la hover la butoane
int accentColor3 = COLOR(12, 17, 17);    // Culoarea textului la butoane
int hlTextBk = COLOR(37, 150, 190);      // Culoarea de Highlight
int hlText = WHITE;                      // COLOR(opus la tema normala)
int cursColor = accentColor3;

double displayOffset = 0;
double displayOffset2 = 0;
double currDisplayOffset = 0;
double currDisplayOffset2 = 0;
double barRaport;
double barRaport2;
double currBarOffset = 0;
int page = 0;
bool typedText = false;
bool isHl = false;

double offsetHeight, offsetLength;
double currWordLength = 0;
int currWordStart = 0;

char *upArrow = "icons\\up.gif";
char *downArrow = "icons\\down.gif";
char *leftArrow = "icons\\left.gif";
char *rightArrow = "icons\\right.gif";

struct
{
    int x, y;
    double length;
    char c;
} text[NMAX];
int lgtext = 0;

struct row
{
    char *text;
    int offsetLine;
    int offsetCol;
    int length;
    int index;
};

struct editorConfig
{
    row row[10000];
    bool isWordWrap = false;
    int maxRowLength;
    int rowCount = 1;
} editor, editorWrap;

char alltext[1000000];
int indexStart = 0, intdexFinish = 0;

struct Cursor
{
    int lin = 0, col = 0, lin2 = 0, col2 = 0;
} cursor, cursorWrap;

int cursorToIndex(int lin, int col);
void getButtonClick(int x, int y);
void setPosChar(char curr);
void getMouseHover(int x, int y);
void getRClickUp(int x, int y);
void getRClickDown(int x, int y);
void debugFunct();
void drawArrowsHorizontal();
void drawHorizBar();
void wordWrapAll();
void displayRows();
void wordWrapAll();
void alltextToNonWrap();
void setTextFont();


void changeCursorColor()
{
    delay(1000);
    if (cursColor == accentColor3)
        cursColor = bkColor;
    else
        cursColor = accentColor3;
    displayRows();
}

Point cursorPosition(Cursor cursor);

bool cursorOutOfBounds(Cursor cursor)
{
    Point cursorP = cursorPosition(cursor);
    if (8 <= cursorP.x && cursorP.x <= winLength - 21 && 0 <= cursorP.y && cursorP.y <= winHeight - saveButton.buttonHeight - 31 - textheight("String"))
        return false;
    return true;
}

Point cursorPosition(Cursor cursor)
{
    Point cursorP;
    int x = 8;
    if (!editor.isWordWrap)
    {
        if (editor.row[cursor.lin].text != NULL && cursor.col > 0)
            x += textwidth(subStr(editor.row[cursor.lin].text, 0, cursor.col - 1));
    }
    else if (editorWrap.row[cursor.lin].text != NULL && cursorWrap.col > 0)
        x += textwidth(subStr(editorWrap.row[cursor.lin].text, 0, cursor.col - 1));
    int y = textheight("String") * cursor.lin;
    cursorP.x = x;
    cursorP.y = y;
    // printf("(%i, %i)",cursorP.x,cursorP.y);
    return cursorP;
}

void indexToCurs(int index, int &lin, int &col)
{
    int i = index;
    lin = 0,col=0;
    if(!editor.isWordWrap)
    {
        while(i>0)
        {
            if(i-(int)strlen(editor.row[lin].text)>0)
            {
                i-=strlen(editor.row[lin].text);
                lin=lin+1;
            }
            else
            {
                col = i;
                i-=strlen(editor.row[lin].text);
            }
        }
    }
    else
    {
        while(i>0)
        {
            if(i-(int)strlen(editorWrap.row[lin].text) > 0)
            {
                i-=strlen(editorWrap.row[lin].text);
                lin=lin+1;
            }
            else
            {
                col = i;
                i-=strlen(editorWrap.row[lin].text);
            }
        }
    }
    printf("INDEX: %d => [%d %d]\n",index,lin,col);
}

void drawCursor(Cursor cursor)
{
    Point cursorP = cursorPosition(cursor);
    int x = cursorP.x;
    int y = cursorP.y;
    int prevColor = getcolor();
    setcolor(cursColor);
    line(x - currDisplayOffset, y - currDisplayOffset2, x - currDisplayOffset, y - currDisplayOffset2 + textheight("String"));
    setcolor(prevColor);
}

void openTxt(char *location)
{
    if (location == NULL)
        return;
    else
    {
        printf("Opened %s", location);
        FILE *myFile = fopen(location, "r");
        while (!feof(myFile))
        {
            char *s = (char *)malloc(1000);
            fgets(s, 1000, myFile);
            strcat(alltext, s);
            editor.rowCount++;
            free(s);
        }
    }

    displayRows();
}

void drawIcons()
{
    wordWrap.toggleHeight = 40;
    wordWrap.radius = 8;
    wordWrap.text = "WordWrap";
    wordWrap.bkcolor = accentColor1;
    wordWrap.oncolor = accentColor2;
    wordWrap.isSet;
    wordWrap.toggleWidth = wordWrap.radius * 2 + textwidth(wordWrap.text) + 15;

    saveButton.bkcolor = COLOR(177, 187, 188);
    copyButton.bkcolor = COLOR(177, 187, 188);
    pasteButton.bkcolor = COLOR(177, 187, 188);
    fontButton.bkcolor = COLOR(177, 187, 188);

    saveButton.icon = "icons\\save_icon.gif";
    copyButton.icon = "icons\\copy_icon.gif";
    pasteButton.icon = "icons\\paste_icon.gif";

    saveButton.text = "Save";
    copyButton.text = "Copy";
    pasteButton.text = "Paste";
    fontButton.text = "Font";

    fontButton.font = font;

    saveButton.iconWidth = pasteButton.iconWidth = copyButton.iconWidth = 32;
    saveButton.iconHeight = pasteButton.iconHeight = copyButton.iconHeight = 32;

    int offset = 5;
    saveButton.buttonWidth = pasteButton.buttonWidth = copyButton.buttonWidth = fontButton.buttonWidth = 100;
    saveButton.buttonHeight = pasteButton.buttonHeight = copyButton.buttonHeight = fontButton.buttonHeight = 40;

    saveButton.b.x = offset;
    copyButton.b.x = saveButton.b.x + saveButton.buttonWidth + offset;
    pasteButton.b.x = copyButton.b.x + copyButton.buttonWidth + offset;
    fontButton.b.x = pasteButton.b.x + pasteButton.buttonWidth + offset;
    wordWrap.b.x = fontButton.b.x + fontButton.buttonWidth + offset * 3;

    saveButton.b.y = copyButton.b.y = pasteButton.b.y = fontButton.b.y = wordWrap.b.y = offset;

    wordWrap.center.x = wordWrap.b.x + wordWrap.radius + 5;
    wordWrap.center.y = wordWrap.b.y + wordWrap.toggleHeight / 2;

    drawToggle(wordWrap);

    drawButton(saveButton);
    drawButton(copyButton);
    drawButton(pasteButton);
    drawButton(fontButton);

    registermousehandler(WM_LBUTTONDOWN, getButtonClick);
    registermousehandler(WM_MOUSEMOVE, getMouseHover);

    registermousehandler(WM_RBUTTONDOWN, getRClickDown);
    registermousehandler(WM_RBUTTONUP, getRClickUp);
}

void drawHorizBar()
{
    setfillstyle(1, accentColor2);
    int horizBarLength = (double)(winLength - 28) * barRaport;
    int barDisplayOffset = (double)(currDisplayOffset) * (winLength - 46) / (winLength + displayOffset + 8);
    bar(22 + barDisplayOffset, winHeight - 18, 22 + horizBarLength + barDisplayOffset, winHeight - 2);
}

void drawVerticalBar()
{
    setfillstyle(1, accentColor2);
    int verticalBarLength = (double)(winHeight - saveButton.buttonHeight - 10 - 63) * barRaport2;
    int barDisplayOffset = (double)(currDisplayOffset2) * (winHeight - saveButton.buttonHeight - 73) / (winHeight - saveButton.buttonHeight - 31 + displayOffset2);
    bar(winLength - 18, saveButton.buttonHeight + 31 + barDisplayOffset, winLength - 2, saveButton.buttonHeight + 31 + barDisplayOffset + verticalBarLength);
}

void drawArrowsHorizontal()
{

    setfillstyle(1, accentColor1);
    bar(0, winHeight - 20, winLength, winHeight);
    readimagefile(leftArrow, 0, winHeight - 20, 20, winHeight);
    readimagefile(rightArrow, winLength - 20, winHeight - 20, winLength, winHeight);
    drawHorizBar();
}

void drawArrowsVertical()
{

    setfillstyle(1, accentColor1);
    bar(winLength - 20, saveButton.buttonHeight + 10, winLength, winHeight - 21);
    readimagefile(upArrow, winLength - 20, saveButton.buttonHeight + 10, winLength, saveButton.buttonHeight + 30);
    readimagefile(downArrow, winLength - 20, winHeight - 41, winLength, winHeight - 21);
    drawVerticalBar();
}

void drawBar()
{
    if (displayOffset > 0)
        drawArrowsHorizontal();
    if (displayOffset2 > 0)
        drawArrowsVertical();
}

void calculateBar(editorConfig editor)
{
    Point cursorP;
    if (editor.isWordWrap)
        cursorP = cursorPosition(cursorWrap);
    else
        cursorP = cursorPosition(cursor);
    // cout<<editor.rowCount<<endl;
    displayOffset = displayOffset2 = 0;
    for (int i = 0; i < editor.rowCount; i++)
    {
        if (textwidth(editor.row[i].text) - winLength > displayOffset)
        {
            displayOffset = textwidth(editor.row[i].text) - winLength + 29; // 8 de la marginea din stanga + 21 in caz ca e nevoie de Scroll vertical
            barRaport = (double)(winLength - 29) / (winLength + displayOffset - 29);
        }
    }
    // cout<< textheight(editor.row[0].text);
    if (editor.rowCount * textheight(editor.row[0].text) > winHeight - saveButton.buttonHeight - 31)
    {
        displayOffset2 = editor.rowCount * textheight(editor.row[0].text) - winHeight + saveButton.iconHeight + 31;
        barRaport2 = (double)(winHeight - saveButton.buttonHeight - 31) / (winHeight - saveButton.buttonHeight - 31 + displayOffset2);
    }
}

void initBuffer()
{
    int formerPage = getactivepage();
    setactivepage(1);
    setbkcolor(bkColor);
    cleardevice();
    drawIcons();
    setcolor(accentColor2);
    setlinestyle(0, 0, 2);
    line(0, saveButton.buttonHeight + 9, winLength, saveButton.buttonHeight + 9);
    setactivepage(formerPage);
}

void displayRows()
{
    if (editor.isWordWrap)
        wordWrapAll();
    else
        alltextToNonWrap();
    setactivepage(!page);
    page = !page;
    drawIcons();
    int x = 8, y = 0;
    setTextFont();
    setfillstyle(1, bkColor);
    if (editor.isWordWrap)
        calculateBar(editorWrap);
    else
        calculateBar(editor);
    bar(0, saveButton.buttonHeight + 10, winLength, winHeight);
    setviewport(0, saveButton.buttonHeight + 10, winLength, winHeight, 1);
    currDisplayOffset = (currDisplayOffset > displayOffset) ? displayOffset : currDisplayOffset;
    currDisplayOffset2 = (currDisplayOffset2 > displayOffset2) ? displayOffset2 : currDisplayOffset2;

    if (typedText)
    {
        typedText = false;
        Point cursorP = {0, 0};
        if (!editor.isWordWrap)
        {
            if (cursorOutOfBounds(cursor))
                cursorP = cursorPosition(cursor);
        }
        else if (cursorOutOfBounds(cursorWrap))
            cursorP = cursorPosition(cursorWrap);
        cursorP.x -= currDisplayOffset;
        cursorP.y -= currDisplayOffset2;
        if (!(8 <= cursorP.x && cursorP.x <= winLength - 21))
            currDisplayOffset += (cursorP.x - winLength + 21 + textwidth("String"));
        if (!(0 <= cursorP.y && cursorP.y <= winHeight - saveButton.buttonHeight - 31 - textheight("String")))
            currDisplayOffset2 += (cursorP.y - winHeight + saveButton.buttonHeight + 31 + textheight("String"));

        currDisplayOffset = (currDisplayOffset > displayOffset) ? displayOffset : currDisplayOffset;
        currDisplayOffset2 = (currDisplayOffset2 > displayOffset2) ? displayOffset2 : currDisplayOffset2;
        currDisplayOffset = (currDisplayOffset < 0) ? 0 : currDisplayOffset;
        currDisplayOffset2 = (currDisplayOffset2 < 0) ? 0 : currDisplayOffset2;
    }

    int bkPrev;
    int colPrev;

    if (!editor.isWordWrap)
        for (int i = 0; i < editor.rowCount; i++)
        {
            if (cursor.lin == i && cursor.lin == cursor.lin2 && isHl)
            {

                bkPrev = getbkcolor();
                colPrev = getcolor();
                char *p = "";
                if (cursor.col2 > 0)
                    p = subStr(editor.row[i].text, 0, cursor.col2 - 1);
                outtextxy(x - currDisplayOffset, y - currDisplayOffset2, p);
                setcolor(hlText);
                setbkcolor(hlTextBk);
                char *p2 = "";
                if (cursor.col > 0)
                    p2 = subStr(editor.row[i].text, cursor.col2, cursor.col - 1);
                outtextxy(x - currDisplayOffset + textwidth(p), y - currDisplayOffset2, p2);
                cout << cursor.col << " " << cursor.col2;
                char *p3 = subStr(editor.row[i].text, cursor.col, strlen(editor.row[i].text));
                setbkcolor(bkPrev);
                setcolor(colPrev);
                outtextxy(x - currDisplayOffset + textwidth(p) + textwidth(p2), y - currDisplayOffset2, p3);
            }
            // else if(cursor.lin2 == i && cursor.lin == i+1 && isHl)
            // {
            //     bkPrev = getbkcolor();
            //     colPrev = getcolor();
            //     char *p = subStr(editor.row[i].text,0,cursor.col2-1);
            //     outtextxy(x-currDisplayOffset,y-currDisplayOffset2,p);
            //     setcolor(hlText);
            //     setbkcolor(hlTextBk);
            //     char *p2 = subStr(editor.row[i].text,cursor.col2,strlen(editor.row[i].text));
            //     outtextxy(x-currDisplayOffset+textwidth(p),y-currDisplayOffset2,p2);
            //     char *p3 = subStr(editor.row[i+1].text,0,cursor.col-1);
            //     y+=textheight(editor.row[i].text);
            //     outtextxy(x-currDisplayOffset,y-currDisplayOffset2,p3);
            //     char *p4 = subStr(editor.row[i+1].text,cursor.col,strlen(editor.row[i+1].text));
            //     setbkcolor(bkPrev);
            //     setcolor(colPrev);
            //     outtextxy(x-currDisplayOffset+textwidth(p3),y-currDisplayOffset2,p4);
            //     isHl = false;
            //     cout<<"Afisat";
            //     i++;
            // }
            else if (cursor.lin2 == i && i + 1 <= cursor.lin && isHl)
            {
                bkPrev = getbkcolor();
                colPrev = getcolor();
                char *p = "";
                if (cursor.col2 > 0)
                    p = subStr(editor.row[i].text, 0, cursor.col2 - 1);
                outtextxy(x - currDisplayOffset, y - currDisplayOffset2, p);
                setcolor(hlText);
                setbkcolor(hlTextBk);
                char *p2 = subStr(editor.row[i].text, cursor.col2, strlen(editor.row[i].text));
                outtextxy(x - currDisplayOffset + textwidth(p), y - currDisplayOffset2, p2);
                i++;
                y += textheight(editor.row[i].text);
                while (i < cursor.lin)
                {
                    outtextxy(x - currDisplayOffset, y - currDisplayOffset2, editor.row[i].text);
                    i++;
                    y += textheight(editor.row[i].text);
                }
                textheight(editor.row[i].text);
                char *p3 = "";
                if (cursor.col > 0)
                    p3 = subStr(editor.row[i].text, 0, cursor.col - 1);
                outtextxy(x - currDisplayOffset, y - currDisplayOffset2, p3);
                char *p4 = subStr(editor.row[i].text, cursor.col, strlen(editor.row[i].text));
                setbkcolor(bkPrev);
                setcolor(colPrev);
                outtextxy(x - currDisplayOffset + textwidth(p3), y - currDisplayOffset2, p4);
            }
            else
                outtextxy(x - currDisplayOffset, y - currDisplayOffset2, editor.row[i].text);
            y += textheight(editor.row[i].text);
        }
    else
        for (int i = 0; i < editorWrap.rowCount; i++)
        {
            if (cursorWrap.lin == i && cursorWrap.lin == cursorWrap.lin2 && isHl)
            {

                bkPrev = getbkcolor();
                colPrev = getcolor();
                char *p = "";
                if (cursorWrap.col2 > 0)
                    p = subStr(editorWrap.row[i].text, 0, cursorWrap.col2 - 1);
                outtextxy(x - currDisplayOffset, y - currDisplayOffset2, p);
                setcolor(hlText);
                setbkcolor(hlTextBk);
                char *p2 = "";
                if (cursorWrap.col > 0)
                    subStr(editorWrap.row[i].text, cursorWrap.col2 - 0, cursorWrap.col - 1);
                outtextxy(x - currDisplayOffset + textwidth(p), y - currDisplayOffset2, p2);
                char *p3 = subStr(editorWrap.row[i].text, cursorWrap.col, strlen(editorWrap.row[i].text));
                setbkcolor(bkPrev);
                setcolor(colPrev);
                outtextxy(x - currDisplayOffset + textwidth(p) + textwidth(p2), y - currDisplayOffset2, p3);
            }
            // else if(cursorWrap.lin2 == i && cursorWrap.lin == i+1 && isHl)
            // {
            //     bkPrev = getbkcolor();
            //     colPrev = getcolor();
            //     char *p = subStr(editorWrap.row[i].text,0,cursorWrap.col2-1);
            //     outtextxy(x-currDisplayOffset,y-currDisplayOffset2,p);
            //     setcolor(hlText);
            //     setbkcolor(hlTextBk);
            //     char *p2 = subStr(editorWrap.row[i].text,cursorWrap.col2,strlen(editorWrap.row[i].text));
            //     outtextxy(x-currDisplayOffset+textwidth(p),y-currDisplayOffset2,p2);
            //     char *p3 = subStr(editorWrap.row[i+1].text,0,cursorWrap.col-1);
            //     y+=textheight(editorWrap.row[i].text);
            //     outtextxy(x-currDisplayOffset,y-currDisplayOffset2,p3);
            //     char *p4 = subStr(editorWrap.row[i+1].text,cursorWrap.col,strlen(editorWrap.row[i+1].text));
            //     setbkcolor(bkPrev);
            //     setcolor(colPrev);
            //     outtextxy(x-currDisplayOffset+textwidth(p3),y-currDisplayOffset2,p4);
            //     isHl = false;
            //     cout<<"Afisat";
            //     i++;
            // }
            else if (cursorWrap.lin2 == i && i + 1 <= cursorWrap.lin && isHl)
            {
                bkPrev = getbkcolor();
                colPrev = getcolor();
                char *p = "";
                if (cursorWrap.col2 > 0)
                    p = subStr(editorWrap.row[i].text, 0, cursorWrap.col2 - 1);
                outtextxy(x - currDisplayOffset, y - currDisplayOffset2, p);
                setcolor(hlText);
                setbkcolor(hlTextBk);
                char *p2 = subStr(editorWrap.row[i].text, cursorWrap.col2, strlen(editorWrap.row[i].text));
                outtextxy(x - currDisplayOffset + textwidth(p), y - currDisplayOffset2, p2);
                i++;
                y += textheight(editorWrap.row[i].text);
                while (i < cursorWrap.lin)
                {
                    outtextxy(x - currDisplayOffset, y - currDisplayOffset2, editorWrap.row[i].text);
                    i++;
                    y += textheight(editorWrap.row[i].text);
                }
                textheight(editorWrap.row[i].text);
                char *p3 = "";
                if (cursorWrap.col > 0)
                    p3 = subStr(editorWrap.row[i].text, 0, cursorWrap.col - 1);
                outtextxy(x - currDisplayOffset, y - currDisplayOffset2, p3);
                char *p4 = subStr(editorWrap.row[i].text, cursorWrap.col, strlen(editorWrap.row[i].text));
                setbkcolor(bkPrev);
                setcolor(colPrev);
                outtextxy(x - currDisplayOffset + textwidth(p3), y - currDisplayOffset2, p4);
            }
            else
                outtextxy(x - currDisplayOffset, y - currDisplayOffset2, editorWrap.row[i].text);
            y += textheight(editorWrap.row[i].text);
        }
    if (editor.isWordWrap == 0)
        drawCursor(cursor);
    else
        drawCursor(cursorWrap);
    setviewport(0, 0, winLength, winHeight, 1);
    drawBar();
    swapbuffers();
}

// void debugFunc()
// {
//     for (int i = 0; i < 10; i++)
//         editor.row[i].text = (char *)malloc(1000);
//     editor.row[0].text = "This is some text\n";
//     editor.row[1].text = "This is more text\n";
//     editor.row[2].text = "This is a very lonbgsrbgsebgsbgbsgsbfjhgsf   asfasfasfsafa   asgsegasbtabrsbtyrasny   rdsnydsrnydsrnysdrnydsrnyss";
//     editor.row[3].text = "row";
//     editor.row[4].text = "row";
//     editor.row[5].text = "row";
//     editor.row[6].text = "row another very long row that i think will not fit on the screen";
//     editor.row[7].text = "row";
//     editor.row[8].text = "row";
//     editor.row[9].text = "rowantgeantgeasnt";
//     editor.row[10].text = "row";
//     editor.row[11].text = "rowfageageeagb aes";
//     editor.row[12].text = "row";
//     editor.row[13].text = "row";
//     editor.row[14].text = "row aetbastbase";
//     editor.row[15].text = "row ateasbtaesta";
//     editor.row[16].text = "row";
//     editor.row[17].text = "rowabteabt";
//     editor.row[18].text = "row";
//     editor.row[19].text = "rowtaestnaesn tasetbaestnase tesantasbte";
//     editor.row[20].text = "row152";
//     editor.row[21].text = "row152";
//     editor.row[22].text = "row152";
//     editor.row[23].text = "row152";
//     editor.row[24].text = "row152";
//     editor.rowCount = 25;
//     for (int i = 0; i < 100; i++)
//     {
//         if (textwidth(editor.row[i].text) - winLength > displayOffset)
//         {
//             displayOffset = textwidth(editor.row[i].text) - winLength + 29; // 8 de la marginea din stanga + 21 in caz ca e nevoie de Scroll vertical
//             barRaport = (double)(winLength - 29) / (winLength + displayOffset - 29);
//         }
//     }
//     if (editor.rowCount * textheight(editor.row[0].text) > winHeight - saveButton.buttonHeight - 31)
//     {
//         displayOffset2 = editor.rowCount * textheight(editor.row[0].text) - winHeight + saveButton.iconHeight + 31;
//         barRaport2 = (double)(winHeight - saveButton.buttonHeight - 31) / (winHeight - saveButton.buttonHeight - 31 + displayOffset2);
//     }
//     setcolor(BLACK);
//     displayRows();
//     if (displayOffset > 0)
//         drawArrowsHorizontal();
//     if (displayOffset2 > 0)
//         drawArrowsVertical();
//     char curr = getch();
//     while (curr != 27)
//     {
//         curr = getch();
//     }
// }

void setTextFont()
{
    lengthError = 1;
    switch (font)
    {
    case 1:
        settextstyle(TRIPLEX_FONT, 0, 0);
        lengthError = 1.3;
        break;
    case 2:
        settextstyle(SMALL_FONT, 0, 0);
        lengthError = 1;
        break;
    case 3:
        settextstyle(SANS_SERIF_FONT, 0, 0);
        lengthError = 1;
        break;
    case 4:
        settextstyle(GOTHIC_FONT, 0, 0);
        lengthError = 1.2;
        break;
    case 5:
        settextstyle(SCRIPT_FONT, 0, 0);
        lengthError = 1;
        break;
    case 6:
        settextstyle(SIMPLEX_FONT, 0, 0);
        lengthError = 1.3;
        break;
    case 7:
        settextstyle(TRIPLEX_SCR_FONT, 0, 0);
        lengthError = 1;
        break;
    case 8:
        settextstyle(COMPLEX_FONT, 0, 10);
        lengthError = 1;
        break;
    case 9:
        settextstyle(EUROPEAN_FONT, 0, 0);
        lengthError = 1.3;
        break;
    case 10:
        settextstyle(BOLD_FONT, 0, 0);
        lengthError = 1.1;
        break;
    default:
        settextstyle(DEFAULT_FONT, 0, 0);
        lengthError = 1.2;
        break;
    }
    setcolor(BLACK);
}

void shiftLeft()
{
    currDisplayOffset = (currDisplayOffset > displayOffset) ? displayOffset : currDisplayOffset;
    currDisplayOffset2 = (currDisplayOffset2 > displayOffset2) ? displayOffset2 : currDisplayOffset2;
    int shiftBy = displayOffset / 30;
    if (currDisplayOffset >= 0 && currDisplayOffset <= displayOffset)
    {
        currDisplayOffset -= shiftBy;
        currDisplayOffset = (currDisplayOffset < 0) ? 0 : currDisplayOffset;
        displayRows();
    }
}

void shiftRight()
{
    currDisplayOffset = (currDisplayOffset > displayOffset) ? displayOffset : currDisplayOffset;
    currDisplayOffset2 = (currDisplayOffset2 > displayOffset2) ? displayOffset2 : currDisplayOffset2;
    int shiftBy = displayOffset / 30;
    if (currDisplayOffset >= 0 && currDisplayOffset <= displayOffset)
    {
        currDisplayOffset += shiftBy;
        currDisplayOffset = (currDisplayOffset > displayOffset) ? displayOffset : currDisplayOffset;
        displayRows();
    }
}

void shiftUp()
{
    currDisplayOffset = (currDisplayOffset > displayOffset) ? displayOffset : currDisplayOffset;
    currDisplayOffset2 = (currDisplayOffset2 > displayOffset2) ? displayOffset2 : currDisplayOffset2;
    int shiftBy = displayOffset2 / 30;
    if (currDisplayOffset2 >= 0 && currDisplayOffset2 <= displayOffset2)
    {
        currDisplayOffset2 -= shiftBy;
        currDisplayOffset2 = (currDisplayOffset2 < 0) ? 0 : currDisplayOffset2;
        displayRows();
    }
}

void shiftDown()
{
    currDisplayOffset = (currDisplayOffset > displayOffset) ? displayOffset : currDisplayOffset;
    currDisplayOffset2 = (currDisplayOffset2 > displayOffset2) ? displayOffset2 : currDisplayOffset2;
    int shiftBy = displayOffset2 / 30;
    if (currDisplayOffset2 >= 0 && currDisplayOffset2 <= displayOffset2)
    {
        currDisplayOffset2 += shiftBy;
        currDisplayOffset2 = (currDisplayOffset2 > displayOffset2) ? displayOffset2 : currDisplayOffset2;
        displayRows();
    }
}

void getButtonClick(int x, int y)
{
    setactivepage(getvisualpage());
    Button b[] = {copyButton, saveButton, pasteButton, fontButton};
    int buttCount = 4;
    for (int i = 0; i < buttCount; i++)
    {
        if (b[i].b.x <= x && x <= b[i].b.x + b[i].buttonWidth && b[i].b.y <= y && y <= b[i].b.y + b[i].buttonHeight)
        {
            cout << b[i].text << " ";
            if (strcmp(b[i].text, "Font") == 0)
            {
                font = (font + 1) % 11;
                setTextFont();
                typedText = true;
                displayRows();
                fontButton.font = font;
                drawButton(fontButton);
            }
        }
    }
    if (displayOffset > 0 && 0 <= x && x <= 20 && winHeight - 20 <= y && y <= winHeight)
        shiftLeft();
    if (displayOffset > 0 && winLength - 20 <= x && x <= winLength && winHeight - 20 <= y && y <= winHeight)
        shiftRight();
    if (displayOffset2 > 0 && (winLength - 20) <= x && x <= winLength && (saveButton.buttonHeight + 10) <= y && y <= (saveButton.buttonHeight + 30))
        shiftUp();
    if (displayOffset2 > 0 && (winLength - 20) <= x && x <= winLength && (winHeight - 40) <= y && y <= (winHeight - 20))
        shiftDown();

    if (wordWrap.center.x - wordWrap.radius <= x && x <= wordWrap.center.x + wordWrap.radius && wordWrap.center.y - wordWrap.radius <= y && y <= wordWrap.center.y + wordWrap.radius)
    {
        wordWrap.isSet = 1 - wordWrap.isSet;
        editor.isWordWrap = wordWrap.isSet;
        drawToggle(wordWrap);
        swapbuffers();
        isHl = false;
        cursor.lin2 = cursor.lin;
        cursor.col2 = cursor.col;
        cursorWrap.lin2 = cursorWrap.lin;
        cursorWrap.col2 = cursorWrap.col;
        typedText = true;
        displayRows();
    }
}

void getMouseHover(int x, int y)
{
    setactivepage(getvisualpage());
    Button b[] = {copyButton, saveButton, pasteButton, fontButton};
    int buttCount = 4;
    for (int i = 0; i < buttCount; i++)
    {
        if (b[i].b.x <= x && x <= b[i].b.x + b[i].buttonWidth && b[i].b.y <= y && y <= b[i].b.y + b[i].buttonHeight && b[i].bkcolor != COLOR(99, 110, 109))
        {
            int prevColor = getcolor();
            setlinestyle(0, 0, 1);
            setcolor(COLOR(99, 110, 109));
            rectangle(b[i].b.x + 1, b[i].b.y + 1, b[i].b.x + b[i].buttonWidth - 1, b[i].b.y + b[i].buttonHeight - 1);
            setcolor(prevColor);
        }
        else if (x <= b[buttCount - 1].b.x + b[buttCount - 1].buttonWidth + 5 && y <= b[buttCount - 1].b.y + b[buttCount - 1].buttonHeight + 5 && (b[i].b.x > x || x > b[i].b.x + b[i].buttonWidth || b[i].b.y > y || y > b[i].b.y + b[i].buttonHeight) && b[i].bkcolor != COLOR(177, 188, 187))
        {
            int prevColor = getcolor();
            setlinestyle(0, 0, 1);
            setcolor(COLOR(177, 188, 187));
            rectangle(b[i].b.x + 1, b[i].b.y + 1, b[i].b.x + b[i].buttonWidth - 1, b[i].b.y + b[i].buttonHeight - 1);
            setcolor(prevColor);
        }
    }
    swapbuffers();
}

void getRClickDown(int x, int y)
{
    Point cursorP;
    cursorP.x = x - 8 + currDisplayOffset;
    cursorP.y = y - saveButton.buttonHeight - 10 + currDisplayOffset2;

    int newLin = -1, newCol = -1;
    newLin = cursorP.y / textheight("String");
    if (editor.isWordWrap && newLin > editorWrap.rowCount - 1)
        newLin = editorWrap.rowCount - 1;
    if (!editor.isWordWrap && newLin > editor.rowCount - 1)
        newLin = editor.rowCount - 1;
    if (editor.isWordWrap)
    {
        for (int i = 0; editorWrap.row[newLin].text[i]; i++)
            if (textwidth(subStr(editorWrap.row[newLin].text, 0, i)) > cursorP.x)
            {
                newCol = i;
                // if(editorWrap.row[newLin].text[i] == '\n')
                //     newCol--;
                break;
            }
        if (newCol == -1)
            newCol = strlen(editorWrap.row[newLin].text);
        // if(editorWrap.row[newLin].text[newCol] == '\n')
        //     newCol--;
    }
    else
    {
        for (int i = 0; editor.row[newLin].text[i]; i++)
            if (textwidth(subStr(editor.row[newLin].text, 0, i)) > cursorP.x)
            {
                newCol = i;
                // if(editor.row[newLin].text[i] == '\n')
                //     newCol--;
                break;
            }
        if (newCol == -1)
            newCol = strlen(editor.row[newLin].text);
        // if(editor.row[newLin].text[newCol] == '\n')
        //     newCol--;
    }
    if (editor.isWordWrap)
    {
        cursorWrap.col = newCol;
        cursorWrap.lin = newLin;
    }
    else
    {
        cursor.col = newCol;
        cursor.lin = newLin;
    }
    // cout<<cursor.lin<<" "<<cursor.col;
    // displayRows();
}

void getRClickUp(int x, int y)
{
    Point cursorP;
    cursorP.x = x - 8 + currDisplayOffset;
    cursorP.y = y - saveButton.buttonHeight - 10 + currDisplayOffset2;

    int newLin = -1, newCol = -1;
    newLin = cursorP.y / textheight("String");
    if (editor.isWordWrap && newLin > editorWrap.rowCount - 1)
        newLin = editorWrap.rowCount - 1;
    if (!editor.isWordWrap && newLin > editor.rowCount - 1)
        newLin = editor.rowCount - 1;
    if (editor.isWordWrap)
    {
        for (int i = 0; editorWrap.row[newLin].text[i]; i++)
            if (textwidth(subStr(editorWrap.row[newLin].text, 0, i)) > cursorP.x)
            {
                newCol = i;
                // if(editorWrap.row[newLin].text[i] == '\n')
                //     newCol--;
                break;
            }
        if (newCol == -1)
            newCol = strlen(editorWrap.row[newLin].text);
        // if(editorWrap.row[newLin].text[newCol] == '\n')
        //     newCol--;
    }
    else
    {
        for (int i = 0; editor.row[newLin].text[i]; i++)
            if (textwidth(subStr(editor.row[newLin].text, 0, i)) > cursorP.x)
            {
                newCol = i;
                // if(editor.row[newLin].text[i] == '\n')
                //     newCol--;
                break;
            }
        if (newCol == -1)
            newCol = strlen(editor.row[newLin].text);
        // if(editor.row[newLin].text[newCol] == '\n')
        //     newCol--;
    }
    if (editor.isWordWrap)
    {
        cursorWrap.col2 = newCol;
        cursorWrap.lin2 = newLin;
    }
    else
    {
        cursor.col2 = newCol;
        cursor.lin2 = newLin;
    }
    if (editor.isWordWrap)
    {
        if (cursorWrap.lin == cursorWrap.lin2 && cursorWrap.col < cursorWrap.col2)
            swap(cursorWrap.col, cursorWrap.col2);
        if (cursorWrap.lin < cursorWrap.lin2)
        {
            swap(cursorWrap.col, cursorWrap.col2);
            swap(cursorWrap.lin, cursorWrap.lin2);
        }
    }
    else
    {
        if (cursor.lin == cursor.lin2 && cursor.col < cursor.col2)
            swap(cursor.col, cursor.col2);
        if (cursor.lin < cursor.lin2)
        {
            swap(cursor.col, cursor.col2);
            swap(cursor.lin, cursor.lin2);
        }
    }
    if (editor.isWordWrap)
    {
        printf("START(%i %i) FINISH(%i %i)\n", cursorWrap.lin2, cursorWrap.col2, cursorWrap.lin, cursorWrap.col);
        if (!(cursorWrap.lin == cursorWrap.lin2 && cursorWrap.col == cursorWrap.col2))
            isHl = true;
    }
    else
    {
        if (!(cursor.lin == cursor.lin2 && cursor.col == cursor.col2))
            isHl = true;
        printf("START(%i %i) FINISH(%i %i)\n", cursor.lin2, cursor.col2, cursor.lin, cursor.col);
    }
    displayRows();
}

void windowsInit()
{
    /// initializare fereastra
    // thread th1(changeCursorColor);
    winLength = 1280;
    winHeight = 720;
    initwindow(winLength, winHeight, "Notepad^2");
    setbkcolor(bkColor);
    cleardevice();
    drawIcons();
    setcolor(accentColor2);
    setlinestyle(0, 0, 2);
    line(0, saveButton.buttonHeight + 9, winLength, saveButton.buttonHeight + 9);
    initBuffer();
    setTextFont();
    line(8, saveButton.buttonHeight + 10, 8, saveButton.buttonHeight + 10 + textheight("String"));
    swapbuffers();
}

// void write(int left, int right)
// {
//     setTextFont();
//     for (int i = left; i <= right; i++)
//         if (text[i].c != 32 && text[i].c != 13)
//         {
//             bgiout << text[i].c;
//             setcolor(BLACK);
//             outstreamxy(text[i].x, text[i].y);
//         }
// }

/*void setPosChar(char *curr)
{
    setTextFont();
    if (*curr == 9) /// 9 = TAB aka indentare
        text[lgtext].length = lengthError * textwidth("    ");
    else
        text[lgtext].length = lengthError * (textwidth(curr)/2);

    offsetHeight = textheight(curr);
    if (*curr == 13)
    {
        y += offsetHeight;
        x = 8;
        currWordStart = currWordLength = 0;
        return;
    }
    if (*curr == 32 || *curr == 9) /// TAB pune 4 spatii, deci cred ca ar trebui inclus si el aici
    {
        x += text[lgtext].length;
        currWordLength = 0;
        currWordStart = lgtext + 1;
        return;
    }
    if (x + text[lgtext].length + 8 <= winLength)
    {

        bgiout << *curr;
        outstreamxy(x, y);

        text[lgtext].x = x;
        text[lgtext].y = y;
        x += text[lgtext].length;
        currWordLength += text[lgtext].length;
        return;
    }
    if (currWordLength + text[lgtext].length + 16 > winLength)
    {
        y = y + offsetHeight;
        x = 8;

        bgiout << *curr;
        outstreamxy(x, y);

        text[lgtext].x = x;
        text[lgtext].y = y;
        x += text[lgtext].length;
        currWordLength = text[lgtext].length;
        currWordStart = lgtext;
        return;
    }
    else
    {
        setfillstyle(0, COLOR(221, 234, 235));
        bar(0, saveButton.buttonHeight + 10, winLength, winHeight);
        write(0, currWordStart - 1);
        y = y + offsetHeight;
        x = 8;
        for (int i = currWordStart; i <= lgtext; i++)
        {
            text[i].y = y;
            text[i].x = x;
            x += text[i].length; /// bug
        }
        write(currWordStart, lgtext);
        currWordLength += text[lgtext].length;
        return;
    }
}*/

void alltextToNonWrap()
{
    editor.rowCount = 0;
    for (int i = 0; i < 10000; i++)
        editor.row[i].text[0] = '\0';
    int indexRand = 0;
    for (int i = 0; alltext[i]; i++)
    {
        editor.row[editor.rowCount].text[indexRand++] = alltext[i];
        /// cout<<editor.row[editor.rowCount].text[indexRand-1]<<'\n';
        editor.row[editor.rowCount].text[indexRand] = 0;
        if (alltext[i] == '\n')
        {
            indexRand = 0;
            editor.rowCount++;
        }
    }
    editor.rowCount -= -1;
    /**char *p;
    p = strtok (alltext, "\n");
    while (p)
          {
           strcpy(editor.row[editor.rowCount++].text,p);
           p = strtok (NULL, "\n");
          }*/
}

void wordWrapAll()
{
    setTextFont();
    for (int i = 0; i < 10000; i++)
        editorWrap.row[i].text[0] = '\0';
    if (editor.row[0].text[0] == NULL)
    {
        cout << "CANT WORDWRAP";
        return;
    }
    /**
    strcpy(alltext, "");
    for (int i = 0; i < editor.rowCount; i++)
        strcat(alltext, editor.row[i].text);
    */
    int left = 0, right, lg;
    char *p, *q;
    editorWrap.rowCount = 0;
    /// de editat
    cursorWrap.lin = cursorWrap.col = 0;
    lg = strlen(alltext);
    while (true)
    {
        for (right = left; right < lg; right++)
        {
            p = subStr(alltext, left, right);
            ///cout<<textwidth(p)<< '\n';
            if (textwidth(p) + 29 > winLength)
            {
                q = subStr(alltext, left, right-1);
                /// cuv mai mare decat tot randul
                if (editorWrap.row[cursorWrap.lin].text[0])
                    cursorWrap.lin++;
                strcpy(editorWrap.row[cursorWrap.lin].text, q);
                editorWrap.row[cursorWrap.lin].text[strlen(editorWrap.row[cursorWrap.lin].text) ] = 0;
                cursorWrap.lin++;
                right--;
                break;
            }
            if (alltext[right] == '\n' || alltext[right] == ' ' || right == lg - 1)
            {
                /// enter, spatiu sau finalul stringului mare
                if (textwidth(editorWrap.row[cursorWrap.lin].text) + textwidth(subStr(p, 0, strlen(p) - 2)) + 29 <= winLength)
                {
                    strcat(editorWrap.row[cursorWrap.lin].text, p);
                }
                else
                {
                    cursorWrap.lin++;
                    strcpy(editorWrap.row[cursorWrap.lin].text, p);
                }
                if (alltext[right] == '\n')
                    cursorWrap.lin++;
                break;
            }
        }
        left = right + 1;
        if (left == lg || !lg)
            break;
    }
    editorWrap.rowCount = cursorWrap.lin + 1;
    cursorWrap.col = strlen(editorWrap.row[cursorWrap.lin].text);
    indexToCurs(cursorToIndex(cursor.lin,cursor.col),cursorWrap.lin,cursorWrap.col);
    indexToCurs(cursorToIndex(cursor.lin,cursor.col),cursorWrap.lin2,cursorWrap.col2);
    /// cout<< cursor.lin << ' ' << cursor.col << '\n';
    /// de implementat cursorul pentru Wrap... cand citim textul
}

int cursorToIndex(int lin, int col)
{
    int nr = 0, i;
    for (i = 0; i < lin; i++)
        if (editor.isWordWrap == 0)
            nr += strlen(editor.row[i].text);
        else
            nr += strlen(editorWrap.row[i].text);
    nr += col;
    return nr;
}

void readText(char *location)
{
    int c1, c2;
    char temp[2];
    for (int i = 0; i < 10000; i++)
    {
        editor.row[i].text = (char *)malloc(10000);
        editorWrap.row[i].text = (char *)malloc(10000);
        // editor.row[i].text[0] = '\0';
        // editorWrap.row[i].text[0] = '\0';
    }
    openTxt(location);
    char curr;
    curr = getch();
    while (curr != 27) /// escape
    {
        if (GetAsyncKeyState(VK_UP))
            shiftUp();
        else if (GetAsyncKeyState(VK_DOWN))
            shiftDown();
        else if (GetAsyncKeyState(VK_LEFT))
            shiftLeft();
        else if (GetAsyncKeyState(VK_RIGHT))
            shiftRight();
        else if (curr == 8) /// BACKSPACE
        {
            typedText = true;
            if (isHl)
            {
                c1 = cursorToIndex(cursor.lin2, cursor.col2);
                c2 = cursorToIndex(cursor.lin, cursor.col);
                stergere(alltext, c1, c2);
                cursor.lin = cursor.lin2;
                cursor.col = cursor.col2;
                isHl = false;
                displayRows();
            }
            else if (cursor.lin != 0 || cursor.col != 0)
            {
                c1 = cursorToIndex(cursor.lin2, cursor.col2);
                stergere(alltext, c1 - 1, c1);
                if (cursor.col > 0)
                {
                    cursor.col--;
                    cursor.col2 = cursor.col;
                }
                else
                {
                    if (cursor.lin > 0)
                    {
                        cursor.lin--;
                        cursor.lin2 = cursor.lin;
                        cursor.col = strlen(editor.row[cursor.lin].text)-1;
                        cursor.col2 = cursor.col;
                    }
                }
                displayRows();
            }
        }
        else if (curr != NULL && (isprint(curr) || curr == 9 || curr == 13))
        {
            typedText = true;
            /// text[lgtext].c = curr;
            if (curr == 9) /// TAB
            {
                curr = ' ';
                /**editor.row[cursor.lin].text[cursor.col++] = ' ';
                editor.row[cursor.lin].text[cursor.col++] = ' ';
                editor.row[cursor.lin].text[cursor.col++] = ' '; */
                c2 = cursorToIndex(cursor.lin, cursor.col);
                c1 = cursorToIndex(cursor.lin2, cursor.col2);
                inserare(alltext, "    ", c1, c2);
                cursor.col2+=3;
                cursor.col = cursor.col2;
                cursor.lin = cursor.lin2;
                /// inserare(editor.row[cursor.lin].text,"    ",cursor.col)
            }
            else
            {
                temp[0] = curr;
                temp[1] = 0;
                c2 = cursorToIndex(cursor.lin, cursor.col);
                c1 = cursorToIndex(cursor.lin2, cursor.col2);
                cursor.col = cursor.col2;
                cursor.lin = cursor.lin2;
                inserare(alltext, temp, c1, c2);
            }
            /// setPosChar(&curr);
            if (curr == 13)
            {
                /**editor.row[cursor.lin].text[cursor.col] = '\n';
                editor.rowCount++;*/
                alltext[c1] = '\n';

                cursor.lin++;
                cursor.col = 0;
                cursor.lin2++;
                cursor.col2 = 0;
            }
            else
            {
                cursor.col++;
                cursor.col2++;
            }
            isHl = false;
            displayRows();
        }

        /// lgtext++;
        // cout<<cursor.col<<'\n';
        curr = getch();
    }
}

int main(int argn, char *argc[])
{
    windowsInit();
    // debugFunc();
    readText(argc[1]);
    closegraph();
    return 0;
}
