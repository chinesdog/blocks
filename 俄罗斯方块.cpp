#include<cstdio>
#include<stdlib.h>
#include<time.h>
#include<graphics.h>
#include<Windows.h>
#include<conio.h>
#include<iostream>
#pragma comment(lib, "Winmm.lib")
using namespace std;
IMAGE img_darkblue, img_lightblue, img_yellow, img_orange, img_green, img_purple, img_red, img_ding;
RECT r_text, r_score;
enum color {
    red = 0,
    darkblue = 1,
    lightblue = 2,
    yellow = 3,
    orange = 4,
    green = 5,
    purple = 6,
};
struct node {
    int loc[4][4];
    int x, y;
    int live;
    int color;
}Blocks[7], Block;//7个方块模板，一个移动方块
int map[20][10], tmp[20][10], visit[20], flag, cnt;//整个有方块的地方用map数组存
int add_score;//每次增加的分数
char score;//分数用字符表示
void menu();//开始菜单
void init();//资源加载函数
void img_print(node block);
void img_background();//可以将方块移动时的图像打印与消除放在一起，抽象与移动
void img_erase(node block);
void play();
bool ismovedown(node block);
bool ismoveleft(node block);
bool ismoveright(node block);
void movedown(node& block);
void moveleft(node& block);
void moveright(node& block);
bool isrotate(node block);
bool isdie(node block);
void rotate(node& block);
bool eraseline();
void printblock();
bool gameover();
int main()
{
    initgraph(375, 500);//大小为10*20
    menu(); //菜单
    init();//初始化游戏资源
    play();//游戏进行部分   
    system("pause");
    return 0;
    //PlaySound(TEXT("Data\\1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
void menu() {
    RECT r = { 0, 0, 250, 500 };//定义矩形大小
    setlinecolor(YELLOW);//设置画线的颜色
    rectangle(75, 200, 175, 300);//画无填充的矩形框
    drawtext(_T("开始游戏"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//打印文字
    MOUSEMSG mouse;//鼠标函数
    while (1) {
        mouse = GetMouseMsg();//鼠标获取信息
        if (mouse.uMsg == WM_LBUTTONDOWN) {
            if (mouse.x > 75 && mouse.x < 175 && mouse.y>200 && mouse.y <= 300) {
                cleardevice();
                return;
            }
        }
    }
}
void init() {
    loadimage(&img_darkblue, _T("darkblue.png"), 25, 25);
    loadimage(&img_lightblue, _T("lightblue.png"), 25, 25);
    loadimage(&img_green, _T("green.png"), 25, 25);
    loadimage(&img_red, _T("red.png"), 25, 25);
    loadimage(&img_purple, _T("purple.png"), 25, 25);
    loadimage(&img_orange, _T("orange.png"), 25, 25);
    loadimage(&img_yellow, _T("yellow.png"), 25, 25);
    loadimage(&img_ding, _T("ding.png"), 375, 500);
    Blocks[red].color = red;
    Blocks[red].x = 0; Blocks[red].y = 0;
    Blocks[red].loc[0][0] = red + 1; Blocks[red].loc[0][1] = red + 1; Blocks[red].loc[0][2] = red + 1; Blocks[red].loc[0][3] = red + 1;

    Blocks[green].color = green;
    Blocks[green].x = 0; Blocks[green].y = 0;
    Blocks[green].loc[0][0] = green + 1; Blocks[green].loc[0][1] = green + 1; Blocks[green].loc[1][0] = green + 1; Blocks[green].loc[1][1] = green + 1;

    Blocks[purple].color = purple;
    Blocks[purple].x = 0; Blocks[purple].y = 0;
    Blocks[purple].loc[0][2] = purple + 1; Blocks[purple].loc[1][0] = purple + 1; Blocks[purple].loc[1][1] = purple + 1; Blocks[purple].loc[1][2] = purple + 1;

    Blocks[orange].color = orange;
    Blocks[orange].x = 0; Blocks[orange].y = 0;
    Blocks[orange].loc[0][1] = orange + 1; Blocks[orange].loc[1][0] = orange + 1; Blocks[orange].loc[1][1] = orange + 1; Blocks[orange].loc[1][2] = orange + 1;

    Blocks[darkblue].color = darkblue;
    Blocks[darkblue].x = 0; Blocks[darkblue].y = 0;
    Blocks[darkblue].loc[0][0] = darkblue + 1; Blocks[darkblue].loc[1][0] = darkblue + 1; Blocks[darkblue].loc[1][1] = darkblue + 1; Blocks[darkblue].loc[1][2] = darkblue + 1;

    Blocks[lightblue].color = lightblue;
    Blocks[lightblue].x = 0; Blocks[lightblue].y = 0;
    Blocks[lightblue].loc[0][0] = lightblue + 1; Blocks[lightblue].loc[0][1] = lightblue + 1; Blocks[lightblue].loc[1][1] = lightblue + 1; Blocks[lightblue].loc[1][2] = lightblue + 1;

    Blocks[yellow].color = yellow;
    Blocks[yellow].x = 0; Blocks[yellow].y = 0;
    Blocks[yellow].loc[0][1] = yellow + 1; Blocks[yellow].loc[0][2] = yellow + 1; Blocks[yellow].loc[1][0] = yellow + 1; Blocks[yellow].loc[1][1] = yellow + 1;
}


void play() {   
    settextcolor(RED);
    setbkmode(YELLOW);
    score = '0';
    r_text = { 250,0,375,500 };
    r_score = { 250,50,375,500 };
    srand(time(NULL));
    while (1) {     
        PlaySound(NULL, 0, 0);
        if (Block.live == 0) {
            if (eraseline()) {
                score = score + add_score;//分数累加
                img_background();
                PlaySound(TEXT("audio\\crash.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                Sleep(1000);
            }
            if (gameover()) {//判断游戏是否结束
                cleardevice();
                return;
            }
            Block = Blocks[rand() % 7];//用随机数防止溢出
            Block.live = 1;
            img_print(Block);//打印出现的方块
        }
        else if (Block.live == 1) {          
            if (!_kbhit()&&isdie(Block)) {
                Block.live = 0;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        if (Block.loc[i][j]) map[Block.x + i][Block.y + j] = Block.color + 1;
                    }
                }
            }
        }       
        if (_kbhit()) {
            char c;
            c = _getch();
            switch (c)
            {
            case 'a':
                if (Block.live == 1 && ismoveleft(Block)) {//方块是否能左移
                    moveleft(Block);//方块左移                 
                    img_background();
                }
                break;
            case 'd':
                if (Block.live == 1 && ismoveright(Block)) {//判断方块是否能右移
                    moveright(Block);//右移                   
                    img_background();
                }
                break;
            case 's':
                if (Block.live == 1 && ismovedown(Block)) {
                    movedown(Block);                 
                    img_background();
                }
                break;
            case 'j':
                if (Block.live == 1 && isrotate(Block)) {//判断方块的否能旋转
                    rotate(Block);                  
                    img_background();
                }
                break;
            case 'p':
                system("pause");
                break;
            default:
                break;
            }
        }
        else {
            if (Block.live == 1 && ismovedown(Block)) {//判断方块是否能下移
                movedown(Block);
                img_background();
                Sleep(100);
                if (_kbhit())continue;
                Sleep(100);
                if (_kbhit())continue;
                Sleep(100);
                if (_kbhit())continue;
                Sleep(100);
                if (_kbhit())continue;
                Sleep(100);
                if (_kbhit())continue;
                Sleep(100);
                if (_kbhit())continue;
                Sleep(100);
            }
        }
    }
}
void img_print(node block) {
    if (block.live == 0) return;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block.loc[i][j]) {
                switch (block.color)
                {
                case red:
                    putimage((j + block.y) * 25, (i + block.x) * 25, &img_red);
                    break;
                case green:
                    putimage((j + block.y) * 25, (i + block.x) * 25, &img_green);
                    break;
                case yellow:
                    putimage((j + block.y) * 25, (i + block.x) * 25, &img_yellow);
                    break;
                case darkblue:
                    putimage((j + block.y) * 25, (i + block.x) * 25, &img_darkblue);
                    break;
                case lightblue:
                    putimage((j + block.y) * 25, (i + block.x) * 25, &img_lightblue);
                    break;
                case purple:
                    putimage((j + block.y) * 25, (i + block.x) * 25, &img_purple);
                    break;
                case orange:
                    putimage((j + block.y) * 25, (i + block.x) * 25, &img_orange);
                    break;
                default:
                    break;
                }
            }
        }
    }
}
bool isdie(node block) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block.loc[i][j]) {
                if (block.x + i + 1 == 20 || map[block.x + i + 1][block.y + j]) return true;               
            }
        }
    }
    return false;
}
bool ismovedown(node block) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block.loc[i][j]) {
                if (block.x + i + 1 >=20) {
                    return false;
                }
                if (map[block.x + i + 1][block.y + j]) return false;
            }
        }
    }
    return true;
}
bool ismoveleft(node block) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block.loc[i][j]) {
                if (block.y + j - 1 < 0) return false;
                if (map[block.x + i][block.y + j - 1]) return false;
            }
        }
    }
    return true;
}
bool ismoveright(node block) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block.loc[i][j]) {
                if (block.y + j + 1 >=10) return false;
                if (map[block.x + i][block.y + j + 1]) return false;
            }
        }
    }
    return true;
}
bool isrotate(node block) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block.loc[i][j]) {
                if (block.x + j + 1 > 20 || block.y + 3 - i < 0 || block.y + 3 - i + 1>10) return false;
                if (map[block.x + j][block.y + 3 - i]) return false;
            }
        }
    }
    return true;
}
void movedown(node& block) {
    img_erase(block);
    block.x += 1;
}
void moveleft(node& block) {
    img_erase(block);
    block.y -= 1;
}
void moveright(node& block) {
    img_erase(block);
    block.y += 1;
}
void rotate(node& block) {
    img_erase(block);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i < j) swap(block.loc[i][j], block.loc[j][i]);
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0, k = 3; j < k; j++, k--) swap(block.loc[i][j], block.loc[i][k]);
    }
}
void img_erase(node block) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block.loc[i][j]) {
                solidrectangle((block.y + j) * 25, (block.x + i) * 25, (block.y + j + 1) * 25, (block.x + i + 1) * 25);
            }
        }
    }
}
bool eraseline() {
    add_score = 0;
    int res = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (map[i][j]) solidrectangle(j * 25, i * 25, (j + 1) * 25, (i + 1) * 25);
        }
    }
    memset(visit, 0, sizeof(visit));
    memset(tmp, 0, sizeof(tmp));
    for (int i = 19; i >= 0; i--) {
        flag = 0;
        for (int j = 0; j < 10; j++) {
            if (map[i][j] == 0) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) visit[i] = 1, res = 1, add_score++;//该行方块已满，分数加1
    }
    for (int j = 0; j < 10; j++) {
        cnt = 20;
        for (int i = 19; i >= 0; i--) {
            if (visit[i] == 0) {
                tmp[--cnt][j] = map[i][j];
            }
        }
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            map[i][j] = tmp[i][j];
        }
    }
    printblock();
    return res;
}
bool gameover() {
    for (int j = 0; j < 10; j++) {
        if (map[0][j]) return true;
    }
    return false;
}
void printblock() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (map[i][j]) {
                switch (map[i][j])
                {
                case red + 1:
                    putimage(j * 25, i * 25, &img_red);
                    break;
                case green + 1:
                    putimage(j * 25, i * 25, &img_green);
                    break;
                case yellow + 1:
                    putimage(j * 25, i * 25, &img_yellow);
                    break;
                case orange + 1:
                    putimage(j * 25, i * 25, &img_orange);
                    break;
                case darkblue + 1:
                    putimage(j * 25, i * 25, &img_darkblue);
                    break;
                case lightblue + 1:
                    putimage(j * 25, i * 25, &img_lightblue);
                    break;
                case purple + 1:
                    putimage(j * 25, i * 25, &img_purple);
                    break;
                default:
                    break;
                }
            }
        }
    }
}
void img_background() {
    putimage(0, 0, &img_ding);
    printblock();
    fillrectangle(251, 0, 251, 500);
    drawtext(_T("得分:"), &r_text, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
    drawtext(score, &r_score, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
    img_print(Block);
}
//nice!
