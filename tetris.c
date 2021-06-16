#include <cmoc.h>
#include <coco.h>


#include "tetris.h"

#define WIDTH   14
#define HEIGHT  12

struct tetris_level {
    int score;
    int nsec;
};

struct tetris_block {
    int w;
    int h;
    char data[5][5];
};

struct tetris {
    char game[WIDTH][HEIGHT];
    int level;
    int gameover;
    int score;
    struct tetris_block current;
    int x;
    int y;
} t;

struct tetris_block blocks[] =
{
    { 2, 2,
     {"##  ", 
      "##  ",
      "    ",
      "    ",
      "    "}
    },
    {3, 2,
     {" X  ",
      "XXX ",
      "    ",
      "    ",
      "    "}
    },
    {4, 1,
     {"@@@@",
      "    ",
      "    ",
      "    ",
      "    "}
    },
    {2, 3,
     {"OO  ",
      "O   ",
      "O   ",
      "    ",
      "    "}
    },
    {2, 3,
     {"&&  ",
      " &  ",
      " &  ",
      "    ",
      "    "}
    },
    {3, 2,
     {"ZZ  ",
      " ZZ ",
      "    ",
      "    ",
      "    "}
    }
};

struct tetris_level levels[]=
{
    {0,
        1200000},
    {1500,
        900000},
    {8000,
        700000},
    {20000,
        500000},
    {40000,
        400000},
    {75000,
        300000},
    {100000,
        200000}
};

#define TETRIS_PIECES (sizeof(blocks)/sizeof(struct tetris_block))
#define TETRIS_LEVELS (sizeof(levels)/sizeof(struct tetris_level))

void
tetris_init() {
    int x, y;
    t.level = 1;
    t.score = 0;
    t.gameover = 0;

    for (x=0; x<WIDTH; x++) {
        for (y=0; y<HEIGHT; y++)
            t.game[x][y] = ' ';
    }
}

void
tetris_print() {
    int x,y;
    for (x=0; x<30; x++)
        printf("\n");
    printf("[LEVEL: %d | SCORE: %d]\n", t.level, t.score);
    for (x=0; x<2*WIDTH+2; x++)
        printf("~");
    printf("\n");
    for (y=0; y<HEIGHT; y++) {
        printf ("!");
        for (x=0; x<WIDTH; x++) {
            if (x>=t.x && y>=t.y 
                    && x<(t.x+t.current.w) && y<(t.y+t.current.h) 
                    && t.current.data[y-t.y][x-t.x]!=' ')
                printf("%c ", t.current.data[y-t.y][x-t.x]);
            else
                printf("%c ", t.game[x][y]);
        }
        printf ("!\n");
    }
    for (x=0; x<2*WIDTH+2; x++)
        printf("~");
    printf("\n");
}

int
tetris_hittest() {
    int x,y,X,Y;
    struct tetris_block b=t.current;
    for (x=0; x<b.w; x++)
        for (y=0; y<b.h; y++) {
            X=t.x+x;
            Y=t.y+y;
            if (X<0 || X>=WIDTH)
                return 1;
            if (b.data[y][x]!=' ') {
                if ((Y>=HEIGHT) || 
                        (X>=0 && X<WIDTH && Y>=0 && t.game[X][Y]!=' ')) {
                    return 1;
                }
            }
        }
    return 0;
}

void
tetris_new_block() {
    t.current=blocks[rand()%TETRIS_PIECES];
    
    t.x=(WIDTH/2) - (t.current.w/2);
    t.y=0;
    if (tetris_hittest()) {
        t.gameover=1;
    }
}

void
tetris_print_block() {
    int x,y,X,Y;
    struct tetris_block b=t.current;
    for (x=0; x<b.w; x++)
        for (y=0; y<b.h; y++) {
            if (b.data[y][x]!=' ')
                t.game[t.x+x][t.y+y]=b.data[y][x];
        }
}

void
tetris_rotate() {
    struct tetris_block b=t.current;
    struct tetris_block s=b;
    int x,y;
    b.w=s.h;
    b.h=s.w;
    for (x=0; x<s.w; x++)
        for (y=0; y<s.h; y++) {
            b.data[x][y]=s.data[s.h-y-1][x];
        }
    x=t.x;
    y=t.y;
    t.x-=(b.w-s.w)/2;
    t.y-=(b.h-s.h)/2;
    t.current=b;
    if (tetris_hittest()) {
        t.current=s;
        t.x=x;
        t.y=y;
    }
}

void
tetris_gravity() {
    int x,y;
    t.y++;
    if (tetris_hittest()) {
        t.y--;
        tetris_print_block();
        tetris_new_block();
    }
}

void
tetris_fall(int l) {
    int x,y;
    for (y=l; y>0; y--) {
        for (x=0; x<WIDTH; x++)
            t.game[x][y]=t.game[x][y-1];
    }
    for (x=0; x<WIDTH; x++)
        t.game[x][0]=' ';
}

void
tetris_check_lines() {
    int x,y,l;
    int p=100;
    for (y=HEIGHT-1; y>=0; y--) {
        l=1;
        for (x=0; x<WIDTH && l; x++) {
            if (t.game[x][y]==' ') {
                l=0;
            }
        }
        if (l) {
            t.score += p;
            p*=2;
            tetris_fall(y);
            y++;
        }
    }
}

int
tetris_level() {
    int i;
    for (i=0; i<TETRIS_LEVELS; i++) {
        if (t.score>=levels[i].score) {
            t.level = i+1;
        } else break;
    }
    return levels[t.level-1].nsec;
}

void
tetris_run() {
    
    char cmd;
    int count=0;
    tetris_init();
    /*srand(993765);*/

    tetris_new_block();

    while (!t.gameover) {
        delay(1);
        count++;
        if (count%50 == 0) {
            tetris_print();
        }
        if (count%50 == 0) {
            tetris_gravity();
            tetris_check_lines();
        }
        while ((cmd=inkey())>0) {
            switch (cmd) {
                case 'Q':
                    t.x--;
                    if (tetris_hittest())
                        t.x++;
                    break;
                case 'D':
                    t.x++;
                    if (tetris_hittest())
                        t.x--;
                    break;
                case 'S':
                    tetris_gravity();
                    break;
                case ' ':
                    tetris_rotate();
                    break;
            }
           
        }
        
        //tm.tv_nsec=tetris_level();
    }

    tetris_print();
    printf("*** GAME OVER ***\n");
    printf("FEDE! %d %d", t.current.w, t.current.h);
}
