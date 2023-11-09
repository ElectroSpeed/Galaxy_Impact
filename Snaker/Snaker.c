#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SFML/Window.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>

typedef int bool;
#define true 1
#define false 0

float PositionX = 0;
float PositionY = 0;
int Direction = 1;
int WindowsX = 1920;
int WindowsY = 1080;
sfClock* MooveTimer;
int LenCase = 60;
int FruitX = 0;
int FruitY = 0;
bool player_dead = false;
float TailX[500];
float TailY[500];
int Score = 0;
int Minimumtail = 3;

sfRenderWindow* window;
sfRectangleShape* shape;
sfRectangleShape* Fruitshape;
sfRectangleShape* Tailshape[500];

int main() {
    srand(time(0));
    MooveTimer = sfClock_create();
    sfVideoMode mode = { WindowsX, WindowsY, 32 };
    window = sfRenderWindow_create(mode, "CSFML Test", sfResize|sfClose, NULL);
    sfRenderWindow_setFramerateLimit(window, 60);

    shape = sfRectangleShape_create();
    sfRectangleShape_setSize(shape, (sfVector2f) { LenCase, LenCase });
    sfRectangleShape_setFillColor(shape, sfGreen);

    FruitSpawn();
    for (int i = 0; i < 500; i++) {
        Tailshape[i] = sfRectangleShape_create();
        sfRectangleShape_setSize(Tailshape[i], (sfVector2f) { LenCase, LenCase });
        sfRectangleShape_setFillColor(Tailshape[i], sfGreen);
        sfRectangleShape_setPosition(Tailshape[i], (sfVector2f) { -1000, -1000 });
        TailX[i] = -1000;
    }

    sfFont* font = sfFont_createFromFile("Arial.ttf");
    sfText* text = sfText_create();
    sfText_setFont(text, font);
    char str[256];


    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        snprintf(str, 256, "Score: %d", Score);
        sfText_setString(text, str);

        if (sfKeyboard_isKeyPressed(sfKeyEscape)) {
            sfRenderWindow_close(window);
        }

        for (int i = 0; i < Score + Minimumtail; i++) {
            if (TailX[i] == PositionX && TailY[i] == PositionY) {
                player_dead = true;
            }
        }


        if (!player_dead) {
            moove();
        }

        if (PositionX == FruitX && PositionY == FruitY) {
            FruitSpawn();
            Score++;
        }

        if (PositionX < 0 || PositionX >= WindowsX || PositionY < 0 || PositionY >= WindowsY) {
            player_dead = true;
        }

        sfRectangleShape_setPosition(shape, (sfVector2f) { PositionX, PositionY });
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawRectangleShape(window, shape, NULL);
        sfRenderWindow_drawRectangleShape(window, Fruitshape, NULL);
        sfRenderWindow_drawText(window, text, NULL);

        for (int i = 0; i < 500; i++) {
            sfRectangleShape_setPosition(Tailshape[i], (sfVector2f) { TailX[i], TailY[i] });
            sfRenderWindow_drawRectangleShape(window, Tailshape[i], NULL);
        }
      
        sfRenderWindow_display(window);
    }
    sfRectangleShape_destroy(shape);
    sfRectangleShape_destroy(Fruitshape);
    sfRenderWindow_destroy(window);

    return 0;
}

int moove() {
    if (sfKeyboard_isKeyPressed(sfKeyLeft) && !(Direction == 1)) {
        Direction = 0;
    }
    if (sfKeyboard_isKeyPressed(sfKeyRight) && !(Direction == 0)) {
        Direction = 1;
    }
    if (sfKeyboard_isKeyPressed(sfKeyUp) && !(Direction == 3)) {
        Direction = 2;
    }
    if (sfKeyboard_isKeyPressed(sfKeyDown) && !(Direction == 2)) {
        Direction = 3;
    }
    sfTime Timer = sfClock_getElapsedTime(MooveTimer);
    int Millisecondes = sfTime_asMilliseconds(Timer);
    if (Millisecondes >= 300) {
        tail();
        sfClock_restart(MooveTimer);
        switch (Direction)
        {
        case 0:
            if (PositionX - LenCase < 0) {
                player_dead = true;
            }
            else {
                PositionX -= LenCase;
            }
            break;
        case 1:
            if (PositionX + LenCase >= WindowsX) {
                player_dead = true;
            }
            else {
                PositionX += LenCase;
            }
            break;
        case 2:
            if (PositionY - LenCase < 0) {
                player_dead = true;
            }
            else {
                PositionY -= LenCase;
            }
            break;
        case 3:
            if (PositionY + LenCase >= WindowsY) {
                player_dead = true;
            }
            else {
                PositionY += LenCase;
            }
            break;
        }
    }
}

int FruitSpawn() {
    Fruitshape = sfRectangleShape_create();
    sfRectangleShape_setSize(Fruitshape, (sfVector2f) { LenCase, LenCase });
    FruitX = (rand() % (WindowsX / LenCase)) * LenCase;
    FruitY = (rand() % (WindowsY / LenCase)) * LenCase;
    sfRectangleShape_setPosition(Fruitshape, (sfVector2f) { FruitX, FruitY });
    sfRectangleShape_setFillColor(Fruitshape, sfYellow);
}

int tail() {
    float OldPositionX = 0;
    float OldPositionY = 0;
    for (int i = 0;i < (Score + Minimumtail);i++) {
        float SaveX = TailX[i];
        float SaveY = TailY[i];

        if (i != 0) {
            TailX[i] = OldPositionX;
            TailY[i] = OldPositionY;
        }

        if (i == 0) {
            TailX[i] = PositionX;
            TailY[i] = PositionY;
            SaveX = TailX[i];
            SaveY = TailY[i];
        }
        OldPositionX = SaveX;
        OldPositionY = SaveY;
    }

}
