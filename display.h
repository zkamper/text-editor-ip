struct Point{
    int x;
    int y;
};

struct Button{
    Point upperleft;
    Point lowerright;
    char *icon;
    int iconWidth;
    int iconHeight;
};