#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<thread>

struct Player{
    char Name[30];
    int x;
    int y;
    int Hp=100;
    int Move=0;
    int Hard;
    int Storage[3] = {0,0,0};
    int Money=0;
    int Mode = 1;
    int server;
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
    if (p.Mode == 2) {
        if (x != 0) {
            gotoxy(x - 1, y);
            printf("(P)");
        }
        else {
            gotoxy(x, y);
            printf("P)");
        }
    }
    else  {
        if (x != 0) {
            gotoxy(x - 1, y);
            printf(" P ");
        }
        else {
            gotoxy(x, y);
            printf("P ");
        }
    }
}
/* ลบผู้เล่น */void erase_player(int x, int y) {
    if(x!=0){
    gotoxy(x-1 , y);
    printf("   ");}
    else {
        gotoxy(x , y);
        printf("  ");
    }
}
void erase_frame_shop(int line) {
    gotoxy(21, line); printf("                    ");
    gotoxy(21, ++line); printf(" ");
    gotoxy(40, line); printf(" ");
    gotoxy(21, ++line); printf("                    ");
}
void draw_frame_shop(int line) {
    gotoxy(21, line); printf("********************");
    gotoxy(21, ++line); printf("*");
    gotoxy(40, line); printf("*");
    gotoxy(21, ++line); printf("********************");
}
int shop() {
    //ร้านค้า
    setcolor(1, 0);
    gotoxy(47, 29), printf("Phiphusana Phingkhasan  64010597");
    setcolor(7, 0);
    gotoxy(0, 0); printf(" ");
    gotoxy(32, 2); printf("** ITEM SHOP **                My Money = %d    ",p.Money);
    gotoxy(28, 6);   printf("Shield                 100");
    gotoxy(24, 10);  printf("Invisibility P.            100");
    gotoxy(27, 14);  printf("HpPotion                100");
    gotoxy(26, 18); printf("BACK  MANU");
    
    setcolor(6, 0);
    gotoxy(1, 22); printf("*** Select ITEM( press 'w' to up // press 's' to down and 'k' to select ) ***");
    setcolor(7, 0);
    Sleep(300);
    char ccc=' ';
    int line = 5,hard=0;
    draw_frame_shop(line);
    do {
        if (_kbhit()) {
            ccc = _getch();
            if (ccc == 's') {
                erase_frame_shop(line);
                if (line < 17)
                    draw_frame_shop(line += 4), hard = line;
                else line = 5, draw_frame_shop(line), hard = line;
            }
            if (ccc == 'w') {
                erase_frame_shop(line);
                if (line > 5)
                    draw_frame_shop(line -= 4), hard = line;
                else line = 17, draw_frame_shop(line), hard = line;
            }
            fflush(stdin);
        }
        Sleep(100);
        hard = line;
    } while (ccc != 'k');
    hard = hard / 4 + 1;
    if (hard == 5) {
        clear_screen();
        return 100;
    }
    else if (p.Money < 100) {
            setcolor(6, 0);
            erase_frame_shop(line);
            gotoxy(43, 18); printf("YOU DON'T HAVE MONEY TO BUY IT");
            setcolor(7, 0);
            shop();
        }
    else if (p.Money >= 100) {
        setcolor(6, 0);
        erase_frame_shop(line);
        gotoxy(43, 18); printf("          YOU BUY IT          ");
        p.Money -= 100;
        setcolor(7, 0);
        shop();
        if(hard==2)p.Storage[0] += 1;
        if (hard == 3)p.Storage[1] += 1;
        if(hard == 4)p.Storage[2] += 1;

    }
    return 100;
}
/* วาดแผนที่ */void draw_map() {
    setcolor(2, 0);
        for (int i = 0; i <= 18 + ((p.Hard - 1) * 4) ; i++) {
            for (int j = 0; j <= 30 +((p.Hard - 1) * 8) ; j++) {
                gotoxy(j, i);
                if (i == 0 && j == 0)setcolor(7, 7), printf("  ");
                else if (j % 2 == 0 && j < 30 + ((p.Hard - 1) * 8) && i <= 15 + ((p.Hard - 1) * 4)) setcolor(2, 0), printf("*");
                else if (j == 30 + ((p.Hard - 1) * 8)) printf("|");
            }
        }

        //กล่องสุ่มตัวเลข
        setcolor(7, 0);
        {gotoxy(0 + ((p.Hard - 1) * 4), 17 + ((p.Hard - 1) * 4)), printf("            *****");
        gotoxy(0 + ((p.Hard - 1) * 4), 18 + ((p.Hard - 1) * 4)), printf("            *   *");
        gotoxy(0 + ((p.Hard - 1) * 4), 19 + ((p.Hard - 1) * 4)), printf("            *****"); }
}
/* สุ่มเดิน สุ่มของ */int show_rand() {
    srand(time(NULL));
    int R;
    for (int i = 0; i <= 20; i++) {
        gotoxy(14 + ((p.Hard - 1) * 4), 18 + ((p.Hard - 1) * 4));
        printf("%d", i % 9 + 1);
        Sleep(50);
    }
    gotoxy(14 + ((p.Hard - 1) * 4), 18 + ((p.Hard - 1) * 4));
    R = (rand() % 49);
    if (R == 48)R = 9;
    else if (R == 45 || R == 46 || R == 47)R = 8;
    else if (R == 42 || R == 43 || R == 44)R = 7;
    else R = (R / 7) + 1;
    printf("%d", R);
    return R;
}
/* กรอบเลือกความยาก */void draw_frame_level(int line) {
    gotoxy(31, line); printf("******************");
    gotoxy(31, ++line); printf("*");
    gotoxy(48, line); printf("*");
    gotoxy(31, ++line); printf("******************");
}
/* ลบกรอบเลือกความยาก */void erase_frame_level(int line) {
    gotoxy(31, line); printf("                  ");
    gotoxy(31, ++line); printf(" ");
    gotoxy(48, line); printf(" ");
    gotoxy(31, ++line); printf("                  ");
}
/* เลือกความยาก */int select_level() {
    setcolor(1, 0);
    gotoxy(47, 29), printf("Phiphusana Phingkhasan  64010597");
    setcolor(7, 0);
    gotoxy(0, 0); printf(" ");
    gotoxy(38, 2); printf("easy  ");
    gotoxy(37, 6); printf("Normal");
    gotoxy(38, 10); printf("hard");
    gotoxy(37, 14); printf("expert");
    gotoxy(37, 18); printf("*SHOP*");
    gotoxy(33, 22); printf("SAVE AND  EXIT");

    setcolor(6, 0);
    gotoxy(1, 26); printf("** Select Level ( press 'w' to up // press 's' to down and 'k' to select ) **");
    setcolor(7, 0);
    int line = 1, hard=0;
    char ch = ' ';
    draw_frame_level(line);
    //ความยาก
    do {
        if (_kbhit()) {
            ch = _getch();
            if (ch == 's') {
                erase_frame_level(line);
                if (line < 21)
                    draw_frame_level(line += 4), hard = line;
                else line = 1, draw_frame_level(line), hard = line;
            }
            if (ch == 'w') {
                erase_frame_level(line);
                if (line > 1)
                    draw_frame_level(line -= 4), hard = line;
                else line = 21, draw_frame_level(line), hard = line;
            }
            fflush(stdin);
        }
        Sleep(100);
        hard = line;
    } while (ch != 'k');
    hard = hard / 4 + 1 ;
    p.Hard = hard;
    if (hard == 5) {
        clear_screen();
        return 100;
    }
    else if(hard==6) {
        clear_screen();
        return 0;
    }
    else
    {
        clear_screen();
        return 1;
    }
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
    gotoxy(35 + ((p.Hard-1) * 8),1);  printf("STATUS");
    gotoxy(32 + ((p.Hard - 1) * 8), 3); printf("Name : %s", p.Name);
    gotoxy(32 + ((p.Hard - 1) * 8), 5); printf(" x = %d      y = %d  ", p.x , p.y);
    gotoxy(32 + ((p.Hard - 1) * 8), 7); printf(" Move = %d  ", p.Move);
    gotoxy(32 + ((p.Hard - 1) * 8), 9); printf(" Hp = %d  ", p.Hp);
    gotoxy(32 + ((p.Hard - 1) * 8), 11); printf(" Hard = %d", p.Hard);
    gotoxy(32 + ((p.Hard - 1) * 8), 13); printf(" Mode = %d", p.Mode);
    gotoxy(32 + ((p.Hard - 1) * 8), 15); printf(" My Money = %d  ", p.Money);
    gotoxy(32 + ((p.Hard - 1) * 8), 17); printf(" 1.Shield           %d  ", p.Storage[0]);
    gotoxy(32 + ((p.Hard - 1) * 8), 18); printf(" 2.Invisibility P.  %d  ", p.Storage[1]);
    gotoxy(32 + ((p.Hard - 1) * 8), 19); printf(" 3.HP               %d  ", p.Storage[2]);

    setcolor(7, 0);
}

int setConsole(int x, int y)
{
    HANDLE wHnd;
    SMALL_RECT windowSize = { 0,0,x - 1,y - 1 };
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
    return 0;
}
void login() {
    int i=4;
    setcolor(1, 0);
    gotoxy(47, 29), printf("Phiphusana Phingkhasan  64010597");
    setcolor(7, 0);
    setcolor(3, 0);
    gotoxy(0, 0); printf(" ");
    gotoxy(22, i); printf("*      ****** ******* ******* **   * ");
    gotoxy(22, i+1); printf("*      *    * *          *    * *  * ");
    gotoxy(22, i+2); printf("*      *    * *  ****    *    *  * * ");
    gotoxy(22, i+3); printf("*      *    * *     *    *    *   ** ");
    gotoxy(22, i+4); printf("*****  ****** ******* ******* *    * ");
    setcolor(1, 0);
    gotoxy(22, i+8); printf("************************************");
    gotoxy(22, i+9); printf("*                                  *");
    gotoxy(22, i+10); printf("************************************");
    
    setcolor(6, 0);
    gotoxy(14, i+15); printf("** Creat Your Plyer Name ( press enter to confirm ) **");
    
    gotoxy(24, i+9); setcursor(1);
       
    scanf("%[^\n]", &p.Name);
        setcolor(7, 0);
        clear_screen();
}
void start() {
    setcolor(1, 0);
    gotoxy(47, 29), printf("Phiphusana Phingkhasan  64010597");
    setcolor(7, 0);
    gotoxy(0, 0); printf(" ");
    int i = 10;setcursor(0);
    setcolor(6, 0);
    gotoxy(0, i);     printf("                                            *******    *      *     *   ****** ");
    gotoxy(0, i + 1); printf("                                            *        *   *   * *   * *  *      ");
    gotoxy(0, i + 2); printf("                                            *  **** * * * *  *  * *  *  ****** ");
    gotoxy(0, i + 3); printf("                                            *     * *     * *    *    * *      ");
    gotoxy(0, i + 4); printf("                                            ******* *     * *         * ****** ");
    setcolor(3, 0);
    gotoxy(2, i);     printf("***** *******    *     ******   ******* ");
    gotoxy(2, i + 1); printf("*        *     *   *   *    *      *    ");
    gotoxy(2, i + 2); printf("*****    *    * * * *  ******      *    ");
    gotoxy(2, i + 3); printf("    *    *    *     *  *  *        *    ");
    gotoxy(2, i + 4); printf("*****    *    *     *  *    *      *    ");
    
    setcolor(1, 0);
    Sleep(3000);
    gotoxy(0, i);     printf("                                                                               ");
    gotoxy(0, i + 1); printf("                                                                               ");
    gotoxy(0, i + 2); printf("                                                                               ");
    gotoxy(0, i + 3); printf("                                                                               ");
    gotoxy(0, i + 4); printf("                                                                               ");
    
}
void sound() {
    std::thread q(Beep, 850, 100);
    q.detach();
}
void rand_Bomb() {
    int x, y;
    for (int i = 0; i < p.Hard * 5; i++) {
        x = (rand() % (11 + (p.Hard * 4))) * 2;
        y = (rand() % (12 + (p.Hard * 4)));
        if (x == 0 && y == 0) {
            x = (rand() % (11 + (p.Hard * 4))) * 2;
            y = (rand() % (12 + (p.Hard * 4)));
        }
        if(x == p.x && y == p.y) {
            x = (rand() % (11 + (p.Hard * 4))) * 2;
            y = (rand() % (12 + (p.Hard * 4)));
        }
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
        x = (rand() % (11 + (p.Hard*4))) * 2;
        y = (rand() % (12 + (p.Hard * 4)));
        while (check != 0) {
            if (Map_Bomb[x][y] == 1 || (x==p.x&&y==p.y) || (x == 0 && y == 0)) {
                x = (rand() % (11 + (p.Hard * 4))) * 2;
                y = (rand() % (12 + (p.Hard * 4)));
            }
            else check = 0;
        }
        
        gotoxy(x, y); printf("@");
}
void player_Dead() {
    int i=7, j=1,kkk;
    setcolor(4, 0);
    gotoxy(j + ((p.Hard - 1) * 4), i - 1 + ((p.Hard - 1) * 2)); printf("                           ");
    gotoxy(j + ((p.Hard - 1) * 4), i + ((p.Hard - 1) * 2));     printf("  G A M E  O V E R !!!  5  ");
    gotoxy(j + ((p.Hard - 1) * 4), i + 1 + ((p.Hard - 1) * 2)); printf("                           ");
    Sleep(1000);
    gotoxy(j + ((p.Hard - 1) * 4), i + ((p.Hard - 1) * 2));     printf("  G A M E  O V E R !!!  4  ");
    Sleep(1000);
    gotoxy(j + ((p.Hard - 1) * 4), i + ((p.Hard - 1) * 2));     printf("  G A M E  O V E R !!!  3  ");
    Sleep(1000);
    gotoxy(j + ((p.Hard - 1) * 4), i + ((p.Hard - 1) * 2));     printf("  G A M E  O V E R !!!  2  ");
    Sleep(1000);
    gotoxy(j + ((p.Hard - 1) * 4), i + ((p.Hard - 1) * 2));     printf("  G A M E  O V E R !!!  1  ");
    Sleep(1000);
    gotoxy(j + ((p.Hard - 1) * 4), i + ((p.Hard - 1) * 2));     printf("  G A M E  O V E R !!!  0  ");
    Sleep(500);                                                   
    setConsole(80, 30);
    setcolor(7, 0);
    clear_screen();
    
}

void player_Win() {
    int i = 7, j = 1, kkk;
    setcolor(6, 0);

    gotoxy(j + ((p.Hard - 1) * 4), i - 1 + ((p.Hard - 1) * 2)); printf("                           ");
    gotoxy(j + ((p.Hard - 1) * 4), i + ((p.Hard - 1) * 2));     printf("  Y O U  ARE  W I N E R 5  ");
    gotoxy(j + ((p.Hard - 1) * 4), i + 1 + ((p.Hard - 1) * 2)); printf("                           ");
    Sleep(1000);
    gotoxy(j + ((p.Hard - 1) * 4), i + ((p.Hard - 1) * 2));     printf("  Y O U  ARE  W I N E R 4  ");
    Sleep(1000);
    gotoxy(j + ((p.Hard - 1) * 4), i + ((p.Hard - 1) * 2));     printf("  Y O U  ARE  W I N E R 3  ");
    Sleep(1000);
    gotoxy(j + ((p.Hard - 1) * 4), i + ((p.Hard - 1) * 2));     printf("  Y O U  ARE  W I N E R 2  ");
    Sleep(1000);
    gotoxy(j + ((p.Hard - 1) * 4), i + ((p.Hard - 1) * 2));     printf("  Y O U  ARE  W I N E R 1  ");
    Sleep(1000);
    gotoxy(j + ((p.Hard - 1) * 4), i + ((p.Hard - 1) * 2));     printf("  Y O U  ARE  W I N E R 0  ");
    Sleep(500);
    p.Money += (30*p.Hard);
    draw_status();
    setConsole(80, 30);
    setcolor(7, 0);

    clear_screen();
}

int play_Game() {
    char ch = ' ';
    int x, y, line = 1, hard = 1, level, item;
    int Ms=0,Mi=0;
    //p.Move = 0;
    p.Hp = 100;
    int j;
    j=select_level();
    while (j == 100) {
        j=shop();
        if (j == 100)j = select_level();
    }
    if (j == 0)return 555;
    x = 28 + ((p.Hard - 1) * 8);
    y = 15 + ((p.Hard - 1) * 4);
    p.x = x; p.y = y;
    clear_screen();
    start();
    if (p.Hard == 1) {
        setConsole(57, 22);
        gotoxy(24, 21); printf("Phiphusana Phingkhasan  64010597");
    }
    if (p.Hard == 2) {
        setConsole(67, 26);
        gotoxy(34, 25); printf("Phiphusana Phingkhasan  64010597");
    }
    if (p.Hard == 3) {
        setConsole(72, 30);
        gotoxy(39, 29); printf("Phiphusana Phingkhasan  64010597");
    }
    if (p.Hard == 4) {
        setConsole(84, 34);
        gotoxy(51, 33); printf("Phiphusana Phingkhasan  64010597");
    }
    
    draw_map(); /* วาดแผนที่ */
    rand_Bomb();
    draw_status();
    draw_player(x, y);
    /*0 แพ้  1 ชนะ*/
    do {
        if (p.Move == 0) {
            gotoxy(0 + ((p.Hard - 1) * 4), 20 + ((p.Hard - 1) * 4));
            printf(" **Press \"space bar\" to random**");
        }
        if (_kbhit()) {
            ch = _getch();
            if (ch == 'a' && x != 0 && p.Move != 0) {
                if (cursor(x - 2, y) != '@' || p.Mode == 3) {
                    if (cursor(x - 2, y) == '@' && p.Mode == 3) {
                        --Mi; 
                        if (Mi == 0) {
                            p.Mode = 1;
                            gotoxy(8 + ((p.Hard - 1) * 4), 20 + ((p.Hard - 1) * 4)), printf("             ");
                            draw_player(x , y);
                        }   
                    }
                    erase_player(x, y);
                    if (cursor(x - 2, y) != ' ') {
                        sound();
                        if (cursor(x - 2, y) == '*')p.Money += 1;
                    }
                    draw_player(x -= 2, y);
                    p.x = x;
                    p.y = y;
                    --p.Move;
                    check_pb();
                    draw_wall();
                    if (p.Mode == 2) { 
                        --Ms;
                        if (Ms == 0) { 
                            p.Mode = 1;
                            gotoxy(8 + ((p.Hard - 1) * 4), 20 + ((p.Hard - 1) * 4)), printf("             ");
                            draw_player(x, y);
                        }
                    }
                }
            }
            if (ch == 'd' && x != (20 + (p.Hard * 8)) && p.Move != 0) {
                if (cursor(x + 2, y) != '@' || p.Mode == 3) {
                    if (cursor(x + 2, y) == '@' && p.Mode == 3) {
                        --Mi;
                        if (Mi == 0) {
                            p.Mode = 1;
                            gotoxy(8 + ((p.Hard - 1) * 4), 20 + ((p.Hard - 1) * 4)), printf("             ");
                            draw_player(x, y);
                        }
                    }
                    erase_player(x, y);
                    if (cursor(x + 2, y) != ' ') {
                        sound();
                        if (cursor(x + 2, y) == '*')p.Money += 1;
                    }
                    draw_player(x += 2, y);
                    p.x = x;
                    p.y = y;
                    --p.Move;
                    check_pb();
                    if (p.Mode == 2) {
                        --Ms;
                        if (Ms == 0) {
                            p.Mode = 1;
                            gotoxy(8 + ((p.Hard - 1) * 4), 20 + ((p.Hard - 1) * 4)), printf("             ");
                            draw_player(x, y);
                        }
                    }
                }
            }
            if (ch == 's' && y != (11 + (p.Hard * 4)) && p.Move != 0) {
                if (cursor(x, y + 1) != '@' || p.Mode == 3) {
                    if (cursor(x, y + 1) == '@' && p.Mode == 3) {
                        --Mi;
                        if (Mi == 0) {
                            p.Mode = 1;
                            gotoxy(8 + ((p.Hard - 1) * 4), 20 + ((p.Hard - 1) * 4)), printf("             ");
                            draw_player(x, y);
                        }
                    }
                    erase_player(x, y);
                    if (cursor(x, y + 1) != ' ') {
                        sound();
                        if (cursor(x, y + 1) == '*')p.Money += 1;
                    }
                    draw_player(x, ++y);
                    p.x = x;
                    p.y = y;
                    --p.Move;
                    check_pb();
                    if (p.Mode == 2) {
                        --Ms;
                        if (Ms == 0) {
                            p.Mode = 1;
                            gotoxy(8 + ((p.Hard - 1) * 4), 20 + ((p.Hard - 1) * 4)), printf("             ");
                            draw_player(x, y);
                        }
                    }
                }
            }
            if (ch == 'w' && y != 0 && p.Move != 0) {
                if (cursor(x, y - 1) != '@' || p.Mode == 3) {
                    if (cursor(x, y - 1) == '@' && p.Mode == 3) {
                        --Mi;
                        if (Mi == 0) {
                            p.Mode = 1;
                            gotoxy(8 + ((p.Hard - 1) * 4), 20 + ((p.Hard - 1) * 4)), printf("             ");
                            draw_player(x, y);
                        }
                    }
                    erase_player(x, y);
                    if (cursor(x, y - 1) != ' ') {
                        sound();
                        if (cursor(x, y - 1) == '*')p.Money += 1;
                    }
                    draw_player(x, --y);
                    p.x = x;
                    p.y = y;
                    --p.Move;
                    check_pb();
                    draw_wall();
                    if (p.Mode == 2) {
                        --Ms;
                        if (Ms == 0) {
                            p.Mode = 1;
                            gotoxy(8 + ((p.Hard - 1) * 4), 20 + ((p.Hard - 1) * 4)), printf("             ");
                            draw_player(x, y);
                        }
                    }
                }
            }
            if (p.Hp <= 0) {
                draw_status();
                player_Dead();
                return 1;
            }
            if (p.x == 0 && p.y == 0) {
                player_Win();
                return 1;
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
                else if (item == 7) {/* โล่ */
                    p.Storage[0] += 1;
                }
                else if (item == 8) {/* ยาล่องหน */
                    p.Storage[1] += 1;
                }
                else if (item == 9) {/* ยาเพิ่มเลือด */
                    p.Storage[2] += 1;
                }
            }
            if (ch == '1' && p.Storage[0] > 0) {//โล่
                --p.Storage[0];
                p.Mode = 2;
                draw_player(x, y);
                Ms += 5;
                gotoxy(8 + ((p.Hard - 1) * 4), 20 + ((p.Hard - 1) * 4)), printf(" YOU USE ITEM");
            }
            if (ch == '2' && p.Storage[1] > 0) {//น้ำยาล่องหน
                --p.Storage[1];
                p.Mode = 3;
                draw_player(x, y);
                Mi += 3;
                gotoxy(8 + ((p.Hard - 1) * 4), 20 + ((p.Hard - 1) * 4)), printf(" YOU USE ITEM");
            }
            if (ch == '3' && p.Storage[2] > 0) {//HP Potion
                --p.Storage[2];
                p.Hp += 50;
                gotoxy(8 + ((p.Hard - 1) * 4), 20 + ((p.Hard - 1) * 4)), printf(" YOU USE ITEM");
            }

            fflush(stdin);
            draw_status();
        }
        Sleep(100);

    } while (ch != 'x'); /* เล่นเกมส์ */
    return 0;
}
void draw_frame_server(int line) {
    gotoxy(31, line); printf("******************");
    gotoxy(31, ++line); printf("*");
    gotoxy(48, line); printf("*");
    gotoxy(31, ++line); printf("******************");
}
void erase_frame_server(int line) {
    gotoxy(31, line); printf("                  ");
    gotoxy(31, ++line); printf(" ");
    gotoxy(48, line); printf(" ");
    gotoxy(31, ++line); printf("                  ");
}
void Delete_Server() {
    gotoxy(0, 0); printf(" ");
    gotoxy(33, 1); printf("DELETE  SERVER");
    gotoxy(36, 4); printf("SERVER 1");
    gotoxy(36, 8); printf("SERVER 2");
    gotoxy(36, 12); printf("SERVER 3");
    gotoxy(38, 16); printf("BACK");
    setcolor(6, 0);
    gotoxy(1, 22); printf("** Select Level ( press 'w' to up // press 's' to down and 'k' to select ) **");
    setcolor(7, 0);
    int line = 3, hard = 0;
    char ch = ' ';
    draw_frame_server(line);
    //ความยาก
    do {
        if (_kbhit()) {
            ch = _getch();
            if (ch == 's') {
                erase_frame_server(line);
                if (line < 15)
                    draw_frame_server(line += 4), hard = line;
                else line = 3, draw_frame_server(line), hard = line;
            }
            if (ch == 'w') {
                erase_frame_server(line);
                if (line > 3)
                    draw_frame_server(line -= 4), hard = line;
                else line = 15, draw_frame_server(line), hard = line;
            }
            fflush(stdin);
        }
        Sleep(100);
        hard = line;
    } while (ch != 'k');
    hard = hard / 4 + 1;
    p.Hard = hard;
    erase_frame_server(line);
    if (hard == 4) {

    }
    else
    {
        if(hard==1){
        FILE* fp;
        fp = fopen("player1.txt", "w");
        fclose(fp);
        }
        if (hard == 2) {
            FILE* fp;
            fp = fopen("player2.txt", "w");
            fclose(fp);
        }
        if (hard == 3) {
            FILE* fp;
            fp = fopen("player3.txt", "w");
            fclose(fp);
        }
    }
}
void Select_Server() {
    gotoxy(0, 0); printf(" ");
    gotoxy(33, 1); printf("SELECT  SERVER");
    gotoxy(36, 4); printf("SERVER 1");
    gotoxy(36, 8); printf("SERVER 2");
    gotoxy(36, 12); printf("SERVER 3");
    gotoxy(33, 16); printf("Delete  SERVER");
    setcolor(6, 0);
    gotoxy(1, 22); printf("** Select Level ( press 'w' to up // press 's' to down and 'k' to select ) **");
    setcolor(7, 0);
    int line = 3, hard = 0;
    char ch = ' ';
    draw_frame_server(line);
    //ความยาก
    do {
        if (_kbhit()) {
            ch = _getch();
            if (ch == 's') {
                erase_frame_server(line);
                if (line < 15)
                    draw_frame_server(line += 4), hard = line;
                else line = 3, draw_frame_server(line), hard = line;
            }
            if (ch == 'w') {
                erase_frame_server(line);
                if (line > 3)
                    draw_frame_server(line -= 4), hard = line;
                else line = 15, draw_frame_server(line), hard = line;
            }
            fflush(stdin);
        }
        Sleep(100);
        hard = line;
    } while (ch != 'k');
    erase_frame_server(line);
    hard = hard / 4 + 1;
    p.Hard = hard;
    if (hard == 4) {
        gotoxy(33, 16); printf("              ");
        Delete_Server();
        Select_Server();
    }
    else//เปิดอ่านไฟล์
    {
        p.server = hard;
        if (p.server == 1) {
            FILE* fp;
            fp = fopen("player1.txt", "r");
            fscanf(fp, "%d %d %d %d", &p.Money, &p.Storage[0], &p.Storage[1], &p.Storage[2]);
            fclose(fp);
        }
        if (p.server == 2) {
            FILE* fp;
            fp = fopen("player2.txt", "r");
            fscanf(fp, "%d %d %d %d", &p.Money, &p.Storage[0], &p.Storage[1], &p.Storage[2]);
            fclose(fp);
        }
        if (p.server == 3) {
            FILE* fp;
            fp = fopen("player3.txt", "r");
            fscanf(fp, "%d %d %d %d", &p.Money, &p.Storage[0], &p.Storage[1], &p.Storage[2]);
            fclose(fp);
        }
    }
    clear_screen();
}
void exit() {
    if (p.server == 1) {
        FILE* fp;
        fp = fopen("player1.txt", "w");
        fprintf(fp, "%d %d %d %d", p.Money, p.Storage[0], p.Storage[1], p.Storage[2]);
        fclose(fp);
    }
    if (p.server == 2) {
        FILE* fp;
        fp = fopen("player2.txt", "w");
        fprintf(fp, "%d %d %d %d", p.Money, p.Storage[0], p.Storage[1], p.Storage[2]);
        fclose(fp);
    }
    if (p.server == 3) {
        FILE* fp;
        fp = fopen("player3.txt", "w");
        fprintf(fp, "%d %d %d %d", p.Money, p.Storage[0], p.Storage[1], p.Storage[2]);
        fclose(fp);
    }

    
}
int main()
{
    setConsole(80, 30);
    setcolor(1, 0);
    gotoxy(47,29),printf("Phiphusana Phingkhasan  64010597");
    setcolor(7, 0);
    setcursor(0);
    srand(time(NULL));
    Select_Server();
    login();
    int j;
    j = play_Game();
    while (j == 1) {
        j = play_Game();
        }
    if (j == 555)exit();
    return 0;
   

    
}

