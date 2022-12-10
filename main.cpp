#include <iostream>
#include <winbgim.h>
#include <graphics.h>
#include <cmath>
#include "display.h"
#include "siruri.h"
#define NMAX 10000
#include <fstream>
#include <stdio.h>

using namespace std;

ofstream fout("dd.txt");

double lengthError;
Button saveButton, copyButton, pasteButton, fontButton;
Toggle wordWrap;
int font = 8; /// 8 - font recomandat, fara niciun offset
double winLength, winHeight;
int bkColor = COLOR(221, 234, 235);      // Culoarea de fundal;
int accentColor1 = COLOR(177, 187, 188); // Fundal butoane
int accentColor2 = COLOR(99, 110, 109);  // Conturul de la hover la butoane
int accentColor3 = COLOR(12, 17, 17);    // Culoarea textului la butoane

double displayOffset = 0;
double displayOffset2 = 0;
double currDisplayOffset = 0;
double currDisplayOffset2 = 0;
double barRaport;
double barRaport2;
double currBarOffset = 0;

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
int indexText = 0;

struct Cursor
{
    int lin = 0, col = 0;
} cursor, cursorWrap;



void getButtonClick(int x, int y);
void setPosChar(char curr);
void getMouseHover(int x, int y);
void debugFunct();
void drawArrowsHorizontal();
void drawHorizBar();
void wordWrapAll();
void displayRows();

void drawCursor(Cursor cursor)
{
    int x = 8;

    if(editor.isWordWrap == 0)
    {
        if(editor.row[cursor.lin].text != NULL)
            x+=textwidth(subStr(editor.row[cursor.lin].text,0,cursor.col));
    }
    else if(editorWrap.row[cursor.lin].text != NULL)
        x+=textwidth(subStr(editorWrap.row[cursor.lin].text,0,cursor.col));
    int y = textheight("String")*cursor.lin;
    int prevColor = getcolor();
    setcolor(accentColor3);
    line(x-currDisplayOffset,y-currDisplayOffset2,x-currDisplayOffset,y-currDisplayOffset2+textheight("String"));
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
            fgets(editor.row[cursor.lin].text, 1000, myFile);
            cursor.lin++;
            editor.rowCount++;
        }
        cursor.col = strlen(editor.row[cursor.lin - 1].text);
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
    int barDisplayOffset = (double)(currDisplayOffset2) * (winHeight - saveButton.buttonHeight - 73) / (winHeight -saveButton.buttonHeight - 31+ displayOffset2);
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

void setTextFont();




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

int page = 0;

void displayRows()
{
    setactivepage(!page);
    page=!page;
    drawIcons();
    int x = 8, y = 0;
    setTextFont();
    setfillstyle(1, bkColor);
    if(editor.isWordWrap)
        calculateBar(editorWrap);
    else
        calculateBar(editor);
    currDisplayOffset = (currDisplayOffset > displayOffset) ? displayOffset : currDisplayOffset;
    currDisplayOffset2 = (currDisplayOffset2 > displayOffset2) ? displayOffset2 : currDisplayOffset2;
    bar(0, saveButton.buttonHeight + 10, winLength, winHeight);
    setviewport(0, saveButton.buttonHeight + 10, winLength, winHeight, 1);

    if (!editor.isWordWrap)
        for (int i = 0; i < editor.rowCount; i++)
        {
            outtextxy(x - currDisplayOffset, y - currDisplayOffset2, editor.row[i].text);
            y += textheight(editor.row[i].text);
        }
    else
        for (int i = 0; i < editorWrap.rowCount; i++)
        {
            outtextxy(x - currDisplayOffset, y - currDisplayOffset2, editorWrap.row[i].text);
            y += textheight(editorWrap.row[i].text);
        }
    if (editor.isWordWrap == 0)
        drawCursor(cursor);
    else    drawCursor(cursorWrap);
    setviewport(0, 0, winLength, winHeight, 1);
    drawBar();
    swapbuffers();
}

void debugFunc()
{
    for (int i = 0; i < 10; i++)
        editor.row[i].text = (char *)malloc(1000);
    editor.row[0].text = "This is some text\n";
    editor.row[1].text = "This is more text\n";
    editor.row[2].text = "This is a very lonbgsrbgsebgsbgbsgsbfjhgsf   asfasfasfsafa   asgsegasbtabrsbtyrasny   rdsnydsrnydsrnysdrnydsrnyss";
    editor.row[3].text = "row";
    editor.row[4].text = "row";
    editor.row[5].text = "row";
    editor.row[6].text = "row another very long row that i think will not fit on the screen";
    editor.row[7].text = "row";
    editor.row[8].text = "row";
    editor.row[9].text = "rowantgeantgeasnt";
    editor.row[10].text = "row";
    editor.row[11].text = "rowfageageeagb aes";
    editor.row[12].text = "row";
    editor.row[13].text = "row";
    editor.row[14].text = "row aetbastbase";
    editor.row[15].text = "row ateasbtaesta";
    editor.row[16].text = "row";
    editor.row[17].text = "rowabteabt";
    editor.row[18].text = "row";
    editor.row[19].text = "rowtaestnaesn tasetbaestnase tesantasbte";
    editor.row[20].text = "row152";
    editor.row[21].text = "row152";
    editor.row[22].text = "row152";
    editor.row[23].text = "row152";
    editor.row[24].text = "row152";
    editor.rowCount = 25;
    for (int i = 0; i < 100; i++)
    {
        if (textwidth(editor.row[i].text) - winLength > displayOffset)
        {
            displayOffset = textwidth(editor.row[i].text) - winLength + 29; // 8 de la marginea din stanga + 21 in caz ca e nevoie de Scroll vertical
            barRaport = (double)(winLength - 29) / (winLength + displayOffset - 29);
        }
    }
    if (editor.rowCount * textheight(editor.row[0].text) > winHeight - saveButton.buttonHeight - 31)
    {
        displayOffset2 = editor.rowCount * textheight(editor.row[0].text) - winHeight + saveButton.iconHeight + 31;
        barRaport2 = (double)(winHeight - saveButton.buttonHeight - 31) / (winHeight - saveButton.buttonHeight - 31 + displayOffset2);
    }
    setcolor(BLACK);
    displayRows();
    if (displayOffset > 0)
        drawArrowsHorizontal();
    if (displayOffset2 > 0)
        drawArrowsVertical();
    char curr = getch();
    while (curr != 27)
    {
        curr = getch();
    }
}

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
        settextstyle(SMALL_FONT, 0, 10);
        lengthError = 1;
        break;
    case 3:
        settextstyle(SANS_SERIF_FONT, 0, 10);
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
                displayRows();
                swapbuffers();
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
        if (editor.isWordWrap)
        {
            wordWrapAll();
            displayRows();
        }
        else
        {
            displayRows();
        }
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

void windowsInit()
{
    /// initializare fereastra
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
    line(8,saveButton.buttonHeight+10,8,saveButton.buttonHeight+10+textheight("String"));
    swapbuffers();
}

void write(int left, int right)
{
    setTextFont();
    for (int i = left; i <= right; i++)
        if (text[i].c != 32 && text[i].c != 13)
        {
            bgiout << text[i].c;
            setcolor(BLACK);
            outstreamxy(text[i].x, text[i].y);
        }
}

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

void wordWrapAll()
{
    setTextFont();
    if(editor.row[0].text[0] == NULL)
    {
        cout<<"CANT WORDWRAP";
        return;
    }
    /**
    strcpy(alltext, "");
    for (int i = 0; i < editor.rowCount; i++)
        strcat(alltext, editor.row[i].text);
    */
    for (int i = 0; i < 10000; i++)
    {
        delete editorWrap.row[i].text;
        editorWrap.row[i].text = new char[10000];
    }
    int left = 0, right, lg;
    char *p;
    editorWrap.rowCount = 0;
    /// de editat
    cursorWrap.lin = cursorWrap.col = 0;
    lg = strlen(alltext);
    while (1)
    {
        for (right = left; right < lg; right++)
        {
            p = subStr(alltext, left, right);
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
            if (textwidth(p) + 29 > winLength)
            {
                /// cuv mai mare decat tot randul
                if (editorWrap.row[cursorWrap.lin].text[0])
                    cursorWrap.lin++;
                strcpy(editorWrap.row[cursorWrap.lin].text, p);
                editorWrap.row[cursorWrap.lin].text[strlen(editorWrap.row[cursorWrap.lin].text) - 1] = 0;
                cursorWrap.lin++;
                editorWrap.row[cursorWrap.lin].text[0] = alltext[right];
                editorWrap.row[cursorWrap.lin].text[1] = 0;
                break;
            }
        }
        left = right + 1;
        if (left == lg || !lg)
            break;
    }
    editorWrap.rowCount = cursorWrap.lin + 1;
    cursorWrap.col = strlen(editorWrap.row[cursorWrap.lin].text);
    ///cout<< cursor.lin << ' ' << cursor.col << '\n';
}

void readText(char *location)
{
    for (int i = 0; i < 10000; i++)
    {
        editor.row[i].text = (char *)malloc(10000);
        editorWrap.row[i].text = (char *)malloc(10000);
        editor.row[i].text[0] = '\0';
        editorWrap.row[i].text[0] = '\0';
    }
    openTxt(location);
    char curr;
    curr = getch();
    while (curr != 27) /// escape
    {
        fflush(stdin);
        if (curr == KEY_UP)
            shiftUp();
        else if (curr == KEY_DOWN)
            shiftDown();
        else if (curr == KEY_LEFT)
            shiftLeft();
        else if (curr == KEY_RIGHT)
            shiftRight();
        else if(curr != NULL)
        {
            /// text[lgtext].c = curr;
            if (curr == 9)
            {
                curr=' ';
                editor.row[cursor.lin].text[cursor.col++] = ' ';
                editor.row[cursor.lin].text[cursor.col++] = ' ';
                editor.row[cursor.lin].text[cursor.col++] = ' ';
                alltext[indexText++] = curr;
                alltext[indexText++] = curr;
                alltext[indexText++] = curr;
                /// inserare(editor.row[cursor.lin].text,"    ",cursor.col)
            }
            editor.row[cursor.lin].text[cursor.col] = curr;
            editor.row[cursor.lin].text[cursor.col + 1] = 0;

            alltext[indexText++] = curr;
            alltext[indexText] = 0;

            /// setPosChar(&curr);
            if (curr == 13)
            {
                editor.row[cursor.lin].text[cursor.col] = '\n';
                cursor.lin++;
                editor.rowCount++;
                cursor.col = 0;

                alltext[indexText - 1] = '\n';
            }
            else
                cursor.col++;
            if (editor.isWordWrap) wordWrapAll();
            displayRows();
        }

        /// lgtext++;
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
