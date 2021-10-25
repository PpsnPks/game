#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<thread>


struct Player{
    int x;
    int y;
    int Hp=100;
    int Move=1000;
    int Hard;
    int Storage[10] = {0,0,0,0,0,0,0,0,0,0};
}p;
int Map_Bomb[55][31];
/* สี */void setcolor(int fg, int bg)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
/* ตำแหน่ง */void gotoxy(int x, int y) {
    COORD c = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
/* แสดงเคอร์เซอร์ */void setcursor(bool visible)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = 20;
    SetConsoleCursorInfo(console, &lpCursor);
}
/* เคลียร์หน้าจอ */void clear_screen(){
    setcursor(0);
    for (int i = 0; i <= 100; i++) {
        for (int j = 0; j <= 80; j++) {
            gotoxy(j, i);
            printf(" ");
        }
    }
}
/* วาดผู้เล่น */void draw_player(int x, int y) {
    if (x != 0) {
        gotoxy(x - 1, y);
        printf(" P ");
    }
    else {
        gotoxy(x, y);
        printf("P ");
    }
}
/* ลบผู้เล่น */void erase_player(int x, int y) {
    if(x!=0){
    gotoxy(x-1, y);
    printf("   ");}
    else {
        gotoxy(x , y);
        printf("  ");
    }
}
/* วาดแผนที่ */void draw_map() {
    setcolor(2, 0);
    for (int i = 0; i <= 18; i++) {
        for (int j = 0; j <= 30; j++) {
            gotoxy(j , i);
            if (i == 0 && j == 0)setcolor(7, 7), printf("  ");
            else if(j%2==0 && j<30 && i<=15) setcolor(2, 0), printf("*");
            else if(j==30) printf("|");
        }
    }
    //กล่องสุ่มตัวเลข
    setcolor(7, 0);
    {gotoxy(0, 17), printf("            *****");
    gotoxy(0, 18), printf("            *   *");
    gotoxy(0, 19), printf("            *****"); }    
}
/* สุ่มเดิน สุ่มของ */int show_rand() {
    srand(time(NULL));
    int R;
    for (int i = 0; i <= 20; i++) {
        gotoxy(14, 18);
        printf("%d", i % 9 + 1);
        Sleep(50);
    }
    gotoxy(14, 18);
    R = (rand() % 49);
    if (R == 48)R = 9;
    else if (R == 45 || R == 46 || R == 47)R = 8;
    else if (R == 42 || R == 43 || R == 44)R = 7;
    else R = (R / 7) + 1;
    printf("%d", R);
    return R;
}
void login() {
    gotoxy(13, 2); printf("LOGIN");
}
/* เลือกความยาก */void select_level() {
    gotoxy(13, 2); printf("easy  ");
    gotoxy(12, 5); printf("Normal");
    gotoxy(13, 8); printf("hard");
    gotoxy(12, 11); printf("expert");
    setcolor(6, 0);
    gotoxy(5, 13); printf("** Select Level ( press 'w' to up // press 's' to down ) **");
    setcolor(7, 0);
}
/* กรอบเลือกความยาก */void draw_frame_level(int line) {
    gotoxy(10, line); printf("**********");
    gotoxy(10, ++line); printf("*");
    gotoxy(19, line); printf("*");
    gotoxy(10, ++line); printf("**********");
}
/* ลบกรอบเลือกความยาก */void erase_frame_level(int line) {
    gotoxy(10, line); printf("          ");
    gotoxy(10, ++line); printf(" ");
    gotoxy(19, line); printf(" ");
    gotoxy(10, ++line); printf("          ");
}
/* เชคที่ตำแหน่งนั้น */char cursor(int x, int y)
{
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    char buf[2]; COORD c = { x,y }; DWORD num_read;
    if (!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))  return '\0';
    else  return buf[0];
}
void draw_status(){
    setcolor(2, 0);
    gotoxy(35,1);  printf("STATUS");
    gotoxy(32, 3); printf(" x = %d      y = %d", p.x , p.y);
    gotoxy(32, 5); printf(" Move = %d", p.Move);
    gotoxy(32, 7); printf(" Hp = %d", p.Hp);
    gotoxy(32, 9); printf(" Hard = %d", p.Hard);
    setcolor(7, 0);
}

void start() {
    gotoxy(0, 0); printf(" ");
    gotoxy(0, 1); printf(" ");
    gotoxy(0, 2); printf(" ");
    gotoxy(0, 3); printf(" ");
    gotoxy(0, 4); printf(" ");
    setcursor(1);
    gotoxy(13, 5); printf("START  GAME");
    Sleep(1000);
    gotoxy(13, 5); printf("           ");
    setcursor(0);
}
void sound() {
    std::thread q(Beep, 850, 100);
    q.detach();
}
void rand_Bomb() {
    int x, y;
    for (int i = 0; i < p.Hard * 5; i++) {
        x = (rand() % 15) * 2;
        y = (rand() % 19) ;
        Map_Bomb[x][y] = 1;
        gotoxy(x, y); printf("#");
    }
}
/* เชคคนกับระเบิด */void check_pb(){
    if (Map_Bomb[p.x][p.y] == 1) {
        p.Hp = p.Hp - 50;
        erase_player(p.x, p.y);
        Sleep(50);
        draw_player(p.x, p.y);
        Sleep(50);
        erase_player(p.x, p.y);
        Sleep(50);
        draw_player(p.x, p.y);
        Sleep(50);
        erase_player(p.x, p.y);
        Sleep(50);
        draw_player(p.x, p.y);
        Sleep(50);
        erase_player(p.x, p.y);
        Sleep(50);
        draw_player(p.x, p.y);
        Map_Bomb[p.x][p.y] = 0;
    }
        
}
void draw_wall() {
    int x, y,check=1;
    for (int i = 0; i < 2; i++) {
        x = (rand() % (12+ (p.Hard*4))) * 2;
        y = (rand() % (15 + (p.Hard * 4)));
        while (check != 0) {
            if (Map_Bomb[x][y] == 1 || (x==p.x&&y==p.y) || (x == 0 && y == 0)) {
                x = (rand() % (12 + (p.Hard * 4))) * 2;
                y = (rand() % (15 + (p.Hard * 4)));
            }
            else check = 0;
        }
        gotoxy(x, y); printf("@");
    }
}
int main()
{
    char ch = ' ';
    int x = 28, y = 15, line = 1, hard =1 , level, item;
    setcursor(0);
    login(); Sleep(5000);
    select_level();
    draw_frame_level(line);
    do {
        if (_kbhit()) {
            ch = _getch();
            if (ch == 's') {
                erase_frame_level(line);
                if (line < 10)
                    draw_frame_level(line += 3), hard = line;
                else line = 1, draw_frame_level(line),hard=line;
            }
            if (ch == 'w') {
                erase_frame_level(line);
                if (line > 1)
                    draw_frame_level(line -= 3), hard = line;
                else line = 10, draw_frame_level(line), hard = line;
            }
            fflush(stdin);
        }
        Sleep(100);
        hard = line;
    } while (ch != 'k'); /* เลือกความยาก */
    hard = hard / 3 + 1;
    
    p.x = x; p.y = y;
    p.Hard = hard;
    clear_screen();
    start();
    draw_map(); /* วาดแผนที่ */
    rand_Bomb();
    draw_status();
    draw_player(x, y);
    do{
        if (p.Move == 0) {
            gotoxy(20, 18);
            printf(" **Press \"space bar\" to random**");
        }
        if (_kbhit()) {
            ch = _getch();
            if (ch == 'a' && x != 0 && p.Move != 0) {
                erase_player(x, y);
                if (cursor(x - 2, y) != '\0') {
                    sound();
                }
                draw_player(x -= 2, y);
                p.x = x;
                p.y = y;
                --p.Move;
                check_pb();
                draw_wall();
            }
            if (ch == 'd' && x != 96 && p.Move != 0) {
                erase_player(x, y);
                if (cursor(x - 2, y) != '\0') {
                    sound();
                }
                draw_player(x += 2, y);
                p.x = x;
                p.y = y;
                --p.Move;
                check_pb();
            }
            if (ch == 's' && y != 44 && p.Move != 0) {
                erase_player(x, y);
                if (cursor(x - 2, y) != '\0') {
                    sound();
                }
                draw_player(x, ++y);
                p.x = x;
                p.y = y;
                --p.Move;
                check_pb();
            }
            if (ch == 'w' && y != 0 && p.Move != 0) {
                erase_player(x, y);
                if (cursor(x - 2, y) != '\0') {
                    sound();
                }
                draw_player(x, --y);
                p.x = x;
                p.y = y;
                --p.Move;
                check_pb();
            }
            if (ch == ' ' && p.Move == 0) {
                item = show_rand();
                if (item == 1) {
                    p.Move += 1;
                }
                else if (item == 2) {
                    p.Move += 2;
                }
                else if (item == 3) {
                    p.Move += 3;
                }
                else if (item == 4) {
                    p.Move += 4;
                }
                else if (item == 5) {
                    p.Move += 5;
                }
                else if (item == 6) {
                    p.Move += 6;
                }
                else if (item == 7) {
                    p.Storage[0] += 1;
                }
                else if (item == 8) {
                    p.Storage[1] += 1;
                }
                else if (item == 9) {
                    p.Storage[2] += 1;
                }
                    }
              fflush(stdin);
              draw_status();
             }
         Sleep(100);
        } while (ch != 'x'); /* เล่นเกมส์ */
    gotoxy(0, 22); 
    printf("%d   %d ", p.x, p.y);
    return 0;
}

