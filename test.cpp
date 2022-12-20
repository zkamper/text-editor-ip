#include <iostream>
#include <time.h>
#include <windows.h>
#include <graphics.h>
#include <bitset>
using namespace std;

int main()
{
    initwindow(100, 100);
    for (int i = 0; i < 1000; i++)
    {
        cout << kbhit();
        getch();
        delay(100);
    }
}