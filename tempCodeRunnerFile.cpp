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

void insertTime()