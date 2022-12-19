#include <iostream>
#include <winbgim.h>
#include <graphics.h>
#include <cmath>
#include "display.h"
#include "siruri.h"
#define NMAX 10000
#include <fstream>
#include <stdio.h>
// #include <thread>
#include <windows.h>

using namespace std;

double lengthError;
Button saveButton, copyButton, pasteButton, fontButton, openButton, cutButton, findButton, timeButton;
Button lnColInd;
Toggle wordWrap, numLock, capsLock;
Menu findMenu, fontMenu;

int font = 8; /// 8 - font recomandat, fara niciun offset
double winLength, winHeight;
int bkColor = COLOR(221, 234, 235);      // Culoarea de fundal;
int accentColor1 = COLOR(177, 187, 188); // Fundal butoane
int accentColor2 = COLOR(99, 110, 109);  // Conturul de la hover la butoane
int accentColor3 = COLOR(12, 17, 17);    // Culoarea textului la butoane
int hlTextBk = COLOR(37, 150, 190);      // Culoarea de Highlight
int hlText = WHITE;                      // COLOR(opus la tema normala)
int cursColor = accentColor3;
int toggleOnColor = COLOR(22, 242, 95);  // Indicator ON
int toggleOffColor = COLOR(130, 10, 10); // Indicator OFF

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
bool changedText;

double offsetHeight, offsetLength;
double currWordLength = 0;
int currWordStart = 0;
int insideFindMenu = false;

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

char *clipboard;
char toFind[100];
char toReplace[100];
int findMenuBox = 0;
char alltext[1000000];
int indexStart = 0, indexFinish = 0;

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

void drawFindMenu()
{
    setactivepage(getvisualpage());
    setfillstyle(1, accentColor1);
    bar(findMenu.b.x, findMenu.b.y, findMenu.b.x + findMenu.width, findMenu.b.y + findMenu.height+30);
    setfillstyle(1, WHITE);
    bar(findMenu.b.x + 10, findMenu.b.y + 10, findMenu.b.x + findMenu.width - 10, findMenu.b.y + 30);
    bar(findMenu.b.x + 10, findMenu.b.y + 40, findMenu.b.x + findMenu.width - 10, findMenu.b.y + 60);
    settextstyle(0, 0, 0);
    int oldbkcolor = getbkcolor();
    setbkcolor(accentColor1);
    outtextxy(findMenu.b.x + findMenu.width / 6 - textwidth("FIND") / 2, findMenu.b.y + findMenu.height - textheight("FIND") / 2 - 20, "FIND");
    outtextxy(findMenu.b.x + findMenu.width / 6 + findMenu.width / 3 * 1 - textwidth("REPLACE ") / 2, findMenu.b.y + findMenu.height - 20 - textheight("REPLACE") / 2, "REPLACE");
    outtextxy(findMenu.b.x + findMenu.width / 6 + findMenu.width / 3 * 2 - textwidth("REPLACE  ALL") / 2, findMenu.b.y + findMenu.height - 20 - textheight("REPLACE ALL") / 2, "REPLACE ALL");
    setbkcolor(oldbkcolor);
    bar(findMenu.b.x + 10, findMenu.b.y + 10, findMenu.b.x + findMenu.width - 10, findMenu.b.y + 30);
    bar(findMenu.b.x + 10, findMenu.b.y + 40, findMenu.b.x + findMenu.width - 10, findMenu.b.y + 60);
    int oldColor = getcolor();
    int oldBkColor = getbkcolor();
    setcolor(BLACK);
    setbkcolor(WHITE);
    settextstyle(0, 0, 0);
    setviewport(findMenu.b.x + 10, findMenu.b.y + 10, findMenu.b.x + findMenu.width - 10, findMenu.b.y + 30, 1);
    outtextxy(0, 0, toFind);
    setviewport(findMenu.b.x + 10, findMenu.b.y + 40, findMenu.b.x + findMenu.width - 10, findMenu.b.y + 60, 1);
    outtextxy(0, 0, toReplace);
    setcolor(oldColor);
    setbkcolor(oldBkColor);
    setviewport(0, 0, winLength, winHeight, 0);
    setTextFont();
}

void insertTime()
{
    char timeTemp[100];
    strcpy(timeTemp, getCurrentDate());
    timeTemp[strlen(timeTemp) - 1] = '\0';
    inserare(alltext, timeTemp, indexStart, indexFinish);
    indexStart = indexFinish = indexStart + strlen(timeTemp);
    typedText = true;
    displayRows();
}

int howManyFound(char *alltext, char *toBeFound)
{
    int ans = -1;
    char *p = alltext;
    while (p)
    {
        p = strstr(p, toBeFound);
        if(p==NULL)
            {ans++;
            break;}
            
        p++;
        ans++;
    }
    return ans;
}

bool findFirst(char *textToSearch, char *toBeFound)
{
    char *p = strstr(textToSearch, toBeFound);
    if (!p)
        {cout<<"Nu am gasit";
        return false;}
    indexStart = (p - alltext);
    indexFinish = indexStart + strlen(toBeFound);
    isHl = typedText = true;
    displayRows();
    return true;
}

void replaceFirst(char *alltext, char *toReplace)
{
    inserare(alltext, toReplace, indexStart, indexFinish);
    indexFinish = indexStart = indexStart + strlen(toReplace);
    isHl = false;
    typedText = true;
    displayRows();
}

void copy()
{
    if (!isHl)
        return;
    clipboard = subStr(alltext, indexStart, indexFinish - 1);
    cout << clipboard << endl;
}

void cut()
{
    copy();
    stergere(alltext, indexStart, indexFinish);
    indexFinish = indexStart;
    typedText = true;
    isHl = false;
    displayRows();
}

void paste()
{
    if (!clipboard)
        return;
    inserare(alltext, clipboard, indexStart, indexFinish);
    indexStart += strlen(clipboard);
    indexFinish = indexStart;
    typedText = true;
    isHl = false;
    displayRows();
}

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
    lin = 0, col = 0;
    if (!editor.isWordWrap)
    {
        while (i > 0)
        {
            if (i - (int)strlen(editor.row[lin].text) > 0)
            {
                i -= strlen(editor.row[lin].text);
                lin = lin + 1;
            }
            else
            {
                col = i;
                i -= strlen(editor.row[lin].text);
                if (editor.row[lin].text[col - 1] == '\n')
                {
                    lin++, col = 0;
                }
            }
        }
    }
    else
    {
        while (i > 0)
        {
            if (i - (int)strlen(editorWrap.row[lin].text) > 0)
            {
                i -= strlen(editorWrap.row[lin].text);
                lin = lin + 1;
            }
            else
            {
                col = i;
                i -= strlen(editorWrap.row[lin].text);
                if (editorWrap.row[lin].text[col - 1] == '\n')
                {
                    lin++, col = 0;
                }
            }
        }
    }
    // printf("INDEX: %d => [%d %d]\n", index, lin, col);
}

char lncol[100];

void drawCursor(Cursor cursor)
{
    Point cursorP = cursorPosition(cursor);
    int x = cursorP.x;
    int y = cursorP.y;
    int prevColor = getcolor();
    setcolor(cursColor);
    line(x - currDisplayOffset, y - currDisplayOffset2, x - currDisplayOffset, y - currDisplayOffset2 + textheight("String"));
    setcolor(prevColor);
    Point cursorNormal = cursorPosition(::cursor);
    
    lncol[0]='\0';
    
    int lin,col;
    char ln[100],cl[100];
    int oldWrap = editor.isWordWrap;
    editor.isWordWrap = 0;
    indexToCurs(indexFinish,lin,col);
    editor.isWordWrap = oldWrap;
    
    itoa(lin+1,ln,10);
    itoa(col+1,cl,10);
    strcat(lncol,"Ln ");
    strcat(lncol,ln);
    strcat(lncol,", Col ");
    
    strcat(lncol,cl);
    
    lnColInd.text = lncol;
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
    wordWrap.toggleWidth = wordWrap.radius * 2 + 85;

    numLock.toggleHeight = 40;
    numLock.radius = 8;
    numLock.text = "Num";
    numLock.isSet = false;
    numLock.toggleWidth = numLock.radius * 2 + 35;

    capsLock.toggleHeight = 40;
    capsLock.radius = 8;
    capsLock.text = "Caps";
    capsLock.isSet = false;
    capsLock.toggleWidth = capsLock.radius * 2 + 35;

    saveButton.bkcolor = COLOR(177, 187, 188);
    copyButton.bkcolor = COLOR(177, 187, 188);
    pasteButton.bkcolor = COLOR(177, 187, 188);
    fontButton.bkcolor = COLOR(177, 187, 188);
    openButton.bkcolor = COLOR(177, 187, 188);
    cutButton.bkcolor = COLOR(177, 187, 188);
    findButton.bkcolor = COLOR(177, 187, 188);
    timeButton.bkcolor = COLOR(177, 187, 188);
    lnColInd.bkcolor = bkColor;

    if (GetKeyState(VK_CAPITAL) & 1)
    {
        capsLock.bkcolor = GREEN;
    }
    else
    {
        capsLock.bkcolor = RED;
    }

    if (GetKeyState(VK_NUMLOCK) & 1)
    {
        numLock.bkcolor = GREEN;
    }
    else
    {
        numLock.bkcolor = RED;
    }

    saveButton.icon = "icons\\save_icon.gif";
    copyButton.icon = "icons\\copy_icon.gif";
    pasteButton.icon = "icons\\paste_icon.gif";
    openButton.icon = "icons\\open_icon.gif";
    cutButton.icon = "icons\\cut_icon.gif";

    saveButton.text = "Save";
    copyButton.text = "Copy";
    pasteButton.text = "Paste";
    fontButton.text = "Font";
    cutButton.text = "Cut";
    openButton.text = "Open";
    findButton.text = "Find & Replace";
    timeButton.text = "Time/Date";
    if(lnColInd.text == NULL) lnColInd.text ="Ln 1, Col 1";

    fontButton.font = font;

    findButton.iconWidth = cutButton.iconWidth = openButton.iconWidth = saveButton.iconWidth = pasteButton.iconWidth = copyButton.iconWidth = 32;
    findButton.iconHeight = cutButton.iconHeight = openButton.iconHeight = saveButton.iconHeight = pasteButton.iconHeight = copyButton.iconHeight = 32;

    int offset = 5;
    timeButton.buttonWidth = findButton.buttonWidth = openButton.buttonWidth = cutButton.buttonWidth = saveButton.buttonWidth = pasteButton.buttonWidth = copyButton.buttonWidth = fontButton.buttonWidth = 100;
    timeButton.buttonHeight = findButton.buttonHeight = openButton.buttonHeight = cutButton.buttonHeight = saveButton.buttonHeight = pasteButton.buttonHeight = copyButton.buttonHeight = fontButton.buttonHeight = 40;

    lnColInd.buttonHeight = 40;
    lnColInd.buttonWidth = 145;

    findButton.buttonWidth = 140;
    saveButton.b.x = offset;
    openButton.b.x = saveButton.b.x + saveButton.buttonWidth + offset;
    findButton.b.x = openButton.b.x + openButton.buttonWidth + offset;
    copyButton.b.x = findButton.b.x + findButton.buttonWidth + 2 * offset;
    cutButton.b.x = copyButton.b.x + copyButton.buttonWidth + offset;
    pasteButton.b.x = cutButton.b.x + cutButton.buttonWidth + offset;
    fontButton.b.x = pasteButton.b.x + pasteButton.buttonWidth + offset;
    timeButton.b.x = fontButton.b.x + fontButton.buttonWidth + offset;
    wordWrap.b.x = timeButton.b.x + timeButton.buttonWidth + offset * 3;
    numLock.b.x = wordWrap.b.x + wordWrap.toggleWidth + offset;
    capsLock.b.x = numLock.b.x + numLock.toggleWidth + offset;
    lnColInd.b.x = winLength-lnColInd.buttonWidth-offset;

    lnColInd.b.y = numLock.b.y = capsLock.b.y = timeButton.b.y = findButton.b.y = openButton.b.y = cutButton.b.y = saveButton.b.y = copyButton.b.y = pasteButton.b.y = fontButton.b.y = wordWrap.b.y = offset;

    wordWrap.center.x = wordWrap.b.x + wordWrap.radius + 5;
    wordWrap.center.y = wordWrap.b.y + wordWrap.toggleHeight / 2;

    numLock.center.x = numLock.b.x + numLock.radius + 5;
    numLock.center.y = numLock.b.y + numLock.toggleHeight / 2;

    capsLock.center.x = capsLock.b.x + capsLock.radius + 5;
    capsLock.center.y = capsLock.b.y + capsLock.toggleHeight / 2;

    findMenu.b.x = findButton.b.x;
    findMenu.b.y = findButton.b.y + 10 + findButton.buttonHeight;

    drawToggle(capsLock);
    drawToggle(numLock);
    drawToggle(wordWrap);

    drawButton(saveButton);
    drawButton(copyButton);
    drawButton(pasteButton);
    drawButton(fontButton);
    drawButton(openButton);
    drawButton(cutButton);
    drawButton(findButton);
    drawButton(timeButton);
    drawButton(lnColInd);

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
    setcolor(accentColor2);
    setlinestyle(0, 0, 2);
    drawToggle(numLock);
    drawToggle(capsLock);
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
    int x = 8, y = 0;
    setbkcolor(bkColor);
    if (editor.isWordWrap)
        calculateBar(editorWrap);
    else
        calculateBar(editor);
    setfillstyle(1, bkColor);
    bar(0, saveButton.buttonHeight + 10, winLength, winHeight);

    drawIcons();
    drawToggle(numLock);
    drawToggle(capsLock);
    setTextFont();
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
                    p2 = subStr(editorWrap.row[i].text, cursorWrap.col2 - 0, cursorWrap.col - 1);
                outtextxy(x - currDisplayOffset + textwidth(p), y - currDisplayOffset2, p2);
                char *p3 = subStr(editorWrap.row[i].text, cursorWrap.col, strlen(editorWrap.row[i].text));
                setbkcolor(bkPrev);
                setcolor(colPrev);
                outtextxy(x - currDisplayOffset + textwidth(p) + textwidth(p2), y - currDisplayOffset2, p3);
            }
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
    alltextToNonWrap();
    if (editor.isWordWrap == 0)
        drawCursor(cursor);
    else
        drawCursor(cursorWrap);
    setviewport(0, 0, winLength, winHeight, 1);
    drawButton(lnColInd);
    drawBar();
    swapbuffers();
}

void setTextFont()
{
    lengthError = 1;
    switch (font)
    {
    case 1:
        settextstyle(TRIPLEX_FONT, 0, 5);
        lengthError = 1.3;
        break;
    case 2:
        settextstyle(SMALL_FONT, 0, 10);
        lengthError = 1;
        break;
    case 3:
        settextstyle(SANS_SERIF_FONT, 0, 5);
        lengthError = 1;
        break;
    case 4:
        settextstyle(GOTHIC_FONT, 0, 5);
        lengthError = 1.2;
        break;
    case 5:
        settextstyle(SCRIPT_FONT, 0, 5);
        lengthError = 1;
        break;
    case 6:
        settextstyle(SIMPLEX_FONT, 0, 5);
        lengthError = 1.3;
        break;
    case 7:
        settextstyle(TRIPLEX_SCR_FONT, 0, 5);
        lengthError = 1;
        break;
    case 8:
        settextstyle(COMPLEX_FONT, 0, 5);
        lengthError = 1;
        break;
    case 9:
        settextstyle(EUROPEAN_FONT, 0, 5);
        lengthError = 1.3;
        break;
    case 10:
        settextstyle(BOLD_FONT, 0, 5);
        lengthError = 1.1;
        break;
    default:
        settextstyle(DEFAULT_FONT, 0, 5);
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

char *locationFound;

void getButtonClick(int x, int y)
{
    setactivepage(getvisualpage());
    Button b[] = {copyButton, saveButton, pasteButton, fontButton, openButton, cutButton, findButton, timeButton};
    int buttCount = 8;
    if (!insideFindMenu)
    {
        for (int i = 0; i < buttCount; i++)
        {
            if (b[i].b.x <= x && x <= b[i].b.x + b[i].buttonWidth && b[i].b.y <= y && y <= b[i].b.y + b[i].buttonHeight)
            {
                cout << b[i].text << " ";
                if (strcmp(b[i].text, "Copy") == 0)
                    copy();
                if (strcmp(b[i].text, "Paste") == 0)
                    paste();
                if (strcmp(b[i].text, "Cut") == 0)
                    cut();
                if (strcmp(b[i].text, "Font") == 0)
                {
                    font = (font + 1) % 11;
                    setTextFont();
                    typedText = true;
                    displayRows();
                    fontButton.font = font;
                    drawButton(fontButton);
                }
                if (strcmp(b[i].text, "Time/Date") == 0)
                    insertTime();
                if (strcmp(b[i].text, "Find & Replace") == 0)
                {
                    insideFindMenu = true;
                    findMenuBox = 1;
                    toFind[0] = '\0';
                    toReplace[0] = '\0';
                    locationFound = alltext;
                    drawFindMenu();
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
            changedText = true;
            cursor.lin2 = cursor.lin;
            cursor.col2 = cursor.col;
            cursorWrap.lin2 = cursorWrap.lin;
            cursorWrap.col2 = cursorWrap.col;
            typedText = true;
            displayRows();
        }
    }
    else
    {
        if (findMenu.b.x <= x && x <= findMenu.b.x + findMenu.width / 3 && findMenu.b.y + 60 <= y && y <= findMenu.b.y + findMenu.height)
        {
            cout <<howManyFound(alltext,toFind)<<endl;
            if(!findFirst(locationFound, toFind) && locationFound == alltext)
                return;
            if(!findFirst(locationFound, toFind) && locationFound > alltext)
                locationFound=alltext;
            findFirst(locationFound,toFind);
            locationFound = strstr(locationFound,toFind);
            locationFound+=1;
            displayRows();
            drawFindMenu();
            //(findMenu.b.x, findMenu.b.y, findMenu.b.x + findMenu.width, findMenu.b.y + findMenu.height+30);
            char apparitions[100]="There are ";
            char count[100];
            itoa(howManyFound(alltext,toFind),count,10);
            strcat(apparitions,count);
            strcat(apparitions," match(es).");
            int oldColor = getcolor();
            int oldBkColor = getbkcolor();
            setcolor(BLACK);
            setbkcolor(accentColor1);
            settextstyle(0, 0, 0);
            outtextxy(findMenu.b.x+10,findMenu.b.y+findMenu.height,apparitions);
            setcolor(oldColor);
            setcolor(oldBkColor);
            setTextFont();
        }
        if (findMenu.b.x + findMenu.width / 3 <= x && x <= findMenu.b.x + findMenu.width / 3 * 2 && findMenu.b.y + 60 <= y && y <= findMenu.b.y + findMenu.height)
        {
            if(!findFirst(locationFound, toFind) && locationFound == alltext)
                return;
            if(!findFirst(locationFound, toFind) && locationFound > alltext)
                locationFound=alltext;
            if(strcmp(subStr(alltext,indexStart,indexFinish-1),toReplace)!=0)
            {
                    locationFound = alltext;
                    findFirst(locationFound,toFind);
            }
            replaceFirst(locationFound,toReplace);
            locationFound=alltext;
            findFirst(locationFound,toFind);
            displayRows();
            drawFindMenu();
        }
        if (findMenu.b.x + findMenu.width / 3 * 2 <= x && x <= findMenu.b.x + findMenu.width && findMenu.b.y + 60 <= y && y <= findMenu.b.y + findMenu.height)
        {
            while(findFirst(alltext,toFind))
            {
                replaceFirst(alltext,toReplace);
                locationFound=alltext;
            }
            insideFindMenu=false;
            displayRows();
        }
    }
}

void getMouseHover(int x, int y)
{
    setactivepage(getvisualpage());
    Button b[] = {copyButton, saveButton, pasteButton, fontButton, openButton, cutButton, findButton, timeButton};
    int buttCount = 8;
    if (!insideFindMenu)
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
    if (insideFindMenu)
        return;
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
    if (insideFindMenu)
        return;
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
        // printf("START(%i %i) FINISH(%i %i)\n", cursorWrap.lin2, cursorWrap.col2, cursorWrap.lin, cursorWrap.col);
        if (!(cursorWrap.lin == cursorWrap.lin2 && cursorWrap.col == cursorWrap.col2))
            isHl = true;
        indexStart = cursorToIndex(cursorWrap.lin2, cursorWrap.col2);
        indexFinish = cursorToIndex(cursorWrap.lin, cursorWrap.col);
    }
    else
    {
        // printf("START(%i %i) FINISH(%i %i)\n", cursor.lin2, cursor.col2, cursor.lin, cursor.col);
        if (!(cursor.lin == cursor.lin2 && cursor.col == cursor.col2))
            isHl = true;
        indexStart = cursorToIndex(cursor.lin2, cursor.col2);
        indexFinish = cursorToIndex(cursor.lin, cursor.col);
    }
    displayRows();
}

void windowsInit()
{
    findMenu.width = 300;
    findMenu.height = 100;
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
    indexToCurs(indexStart, cursor.lin2, cursor.col2);
    indexToCurs(indexFinish, cursor.lin, cursor.col);
}

void wordWrapAll()
{
    Cursor cursAux = cursorWrap;
    setTextFont();
    for (int i = 0; i < 10000; i++)
        editorWrap.row[i].text[0] = '\0';
    if (editor.row[0].text[0] == NULL)
    {
        cout << "Cannot wordwrap.\n";
        editor.isWordWrap = 1 - wordWrap.isSet;
        wordWrap.isSet = 1 - wordWrap.isSet;
        drawToggle(wordWrap);
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
            /// cout<<textwidth(p)<< '\n';
            if (textwidth(p) + 29 > winLength)
            {
                q = subStr(alltext, left, right - 1);
                /// cuv mai mare decat tot randul
                if (editorWrap.row[cursorWrap.lin].text[0])
                    cursorWrap.lin++;
                strcpy(editorWrap.row[cursorWrap.lin].text, q);
                editorWrap.row[cursorWrap.lin].text[strlen(editorWrap.row[cursorWrap.lin].text)] = 0;
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

    indexToCurs(indexStart, cursorWrap.lin2, cursorWrap.col2);
    indexToCurs(indexFinish, cursorWrap.lin, cursorWrap.col);

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
    while (curr != 27 || GetKeyState(VK_CAPITAL) >> 7 || GetKeyState(VK_NUMLOCK) >> 7) /// escape
    {
        if (!insideFindMenu)
        {
            printf("Caracterul scris: %d\n", curr);
            fflush(stdin);
            if (GetKeyState(VK_CONTROL) >> 7 && GetKeyState('A') >> 7)
            {
                indexStart = 0;
                indexFinish = strlen(alltext);
                isHl = true;
                displayRows();
            }
            else if (GetKeyState(VK_CONTROL) >> 7 && GetKeyState('C') >> 7)
                copy();
            else if (GetKeyState(VK_CONTROL) >> 7 && GetKeyState('V') >> 7)
                paste();
            else if (GetKeyState(VK_CONTROL) >> 7 && GetKeyState('X') >> 7)
                cut();
            else if (GetKeyState(VK_UP))
            {
                shiftUp();
                curr = 0;
            }
            else if (GetKeyState(VK_DOWN))
            {
                shiftDown();
                curr = 0;
            }
            else if (GetKeyState(VK_LEFT))
            {
                shiftLeft();
                curr = 0;
            }
            else if (GetKeyState(VK_RIGHT))
            {
                shiftRight();
                curr = 0;
            }
            // else if (GetKeyState(VK_CAPITAL)>>7)
            // {
            //     cout<<"CAPS";
            //     if (GetKeyState(VK_CAPITAL) & 1)
            //         capsLock.bkcolor = GREEN;
            //     else
            //         capsLock.bkcolor = RED;
            //     displayRows();
            // }
            // else if (GetKeyState(VK_NUMLOCK)>>7)
            // {
            //     cout<<"NUM";
            //     if (GetKeyState(VK_NUMLOCK) & 1)
            //         numLock.bkcolor = GREEN;
            //     else
            //         numLock.bkcolor = RED;
            //     displayRows();
            // }
            else if (curr == 8) /// BACKSPACE
            {
                typedText = true;
                if (isHl)
                {
                    stergere(alltext, indexStart, indexFinish);
                    isHl = false;
                    indexFinish = indexStart;
                    displayRows();
                }
                else if (indexStart > 0)
                {
                    stergere(alltext, indexStart - 1, indexStart);
                    indexStart--;
                    indexFinish = indexStart;
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
                    inserare(alltext, "    ", indexStart, indexFinish);
                    indexStart += 3;
                    indexFinish = indexStart;
                    /// inserare(editor.row[cursor.lin].text,"    ",cursor.col)
                }
                else
                {
                    temp[0] = curr;
                    temp[1] = 0;
                    inserare(alltext, temp, indexStart, indexFinish);
                }
                /// setPosChar(&curr);
                if (curr == 13)
                {
                    /**editor.row[cursor.lin].text[cursor.col] = '\n';
                    editor.rowCount++;*/
                    alltext[indexStart] = '\n';

                    indexStart++;
                    indexFinish = indexStart;
                }
                else
                {
                    indexStart++;
                    indexFinish = indexStart;
                }
                isHl = false;
                displayRows();
            }
        }
        else
        {
            if (insideFindMenu)
            {
                if (curr != 13)
                {
                    if (findMenuBox == 1)
                    {
                        cout << "TOFIND";
                        if (isprint(curr))
                        {
                            int len = strlen(toFind);
                            toFind[len] = curr;
                            toFind[len + 1] = '\0';
                            cout << toFind << endl;
                        }
                        else if (curr == 8)
                        {
                            toFind[strlen(toFind) - 1] = '\0';
                        }
                    }
                    else if (findMenuBox == 2)
                    {
                        cout << "TOREPLACE";
                        if (isprint(curr))
                        {
                            int len = strlen(toReplace);
                            toReplace[len] = curr;
                            toReplace[len + 1] = '\0';
                            cout << toReplace << endl;
                        }
                        else if (curr == 8)
                        {
                            toReplace[strlen(toReplace) - 1] = '\0';
                        }
                    }
                }
                else
                    findMenuBox++;
                drawFindMenu();
            }
        }
        curr = getch();
        if (curr == 27 && insideFindMenu)
        {
            insideFindMenu = false;
            curr = 0;
            displayRows();
        }
        fflush(stdin);
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
