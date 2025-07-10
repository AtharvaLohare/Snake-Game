#include <raylib.h>
#include <iostream>
#include <deque>
using namespace std;

Color green = {173, 204, 96, 255};
Color darkgreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;










class Food{

public:
    /* Members required to describe the food bite.*/
    Vector2 position = GeneratRandomPosition();
    Texture2D texture;

    /* Rendering the texture to be used as food, 
       Custom Constructor this is. */
    Food(){
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    /* The Custom destructir used to unload the texture 
       as it is not required with the end of program execution.*/
    ~Food(){
        UnloadTexture(texture);
    }

    /* To draw the food in each iteration at a random position.*/
    void Draw(){
        DrawTexture(texture, position.x*cellSize, position.y*cellSize, WHITE);

    }

    /* Generates the randome position for the food in each iteration.*/
    Vector2 GeneratRandomPosition(){

        float x = GetRandomValue(0, cellCount-1);
        float y = GetRandomValue(0, cellCount-1);

        Vector2 newPosition = {x, y};
        return newPosition;
    }
};








int main() {   
    
    InitWindow(cellSize*cellCount, cellSize*cellCount, "Retro Snake");
    SetTargetFPS(60);

    while(WindowShouldClose() == false){



        /* begin drawing for the new iteration of the gameloop.*/
        BeginDrawing();

        /* clear background */
        ClearBackground(green);

        EndDrawing();

    }

    CloseWindow();
    return 0;
}