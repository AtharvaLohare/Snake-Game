#include <raylib.h>
#include <iostream>
#include <deque>
#include <raymath.h>
using namespace std;

Color green = {173, 204, 96, 255};
Color darkgreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;
double lastUpdatetime = 0;

/* Some smaller helper function for the code modularity */

bool eventTriggered( double interval ){
    double currentTime = GetTime();

    if( (currentTime - lastUpdatetime) > interval ){
        lastUpdatetime = currentTime;
        return true;
    }
    return false;
}

bool element_inDeque( Vector2 element, deque<Vector2>& dq){

    for(unsigned int i = 0; i<dq.size(); i++ ){
        
        if(Vector2Equals( element, dq[i])) return true;
    }
    return false;
}


class Snake{

public:
    deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction = {1,0};
    bool addSegment = false;

    void Draw(){
        /* draw the complete body of the snake */
        for(unsigned int i = 0; i<body.size(); i++){

            float x = body[i].x;
            float y = body[i].y;
            Rectangle body_seg = Rectangle{offset + x*cellSize, offset + y*cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(body_seg, 0.5, 6, darkgreen);
        }
    }

    void Update(){
        /* update the snake : remove the last block and add the new one in front.*/

        body.push_front(Vector2Add(body[0], direction));

        if(addSegment == true){
            addSegment =  false;
        }
        else{
            body.pop_back();
        } 
    }

    void Reset(){
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = {1,0};
    }

};

class Food{

public:
    /* Members required to describe the food bite.*/
    Vector2 position;
    Texture2D texture;

    /* Rendering the texture to be used as food, 
       Custom Constructor this is. */
    Food(deque<Vector2> snakebody){

        position = GenerateRandomPosition(snakebody);
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
        DrawTexture(texture,offset + position.x*cellSize, offset + position.y*cellSize, WHITE);

    }

    Vector2 GenerateRandomCell(){

        float x = GetRandomValue(0, cellCount-1);
        float y = GetRandomValue(0, cellCount-1);
        Vector2 position = Vector2{x, y};

        return position;
    }

    /* Generates the randome position for the food in each iteration.*/
    Vector2 GenerateRandomPosition(deque<Vector2> Snakebody){

        Vector2 newPosition = GenerateRandomCell();
        while(element_inDeque(newPosition, Snakebody)){

            newPosition = GenerateRandomCell();
        }
        return newPosition;
    }
};

class Game{

public:
    
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;

    void Draw(){
        food.Draw();
        snake.Draw();
    }

    void Update(){

        if(running){

            snake.Update();
            checkCollisionWithFood();
            checkCollisionWithEdges();
            checkCollisionWithTail();
        }
    }

    void checkCollisionWithFood(){

        if(Vector2Equals(snake.body[0], food.position)){
            food.position = food.GenerateRandomPosition(snake.body);
            snake.addSegment = true;
        }
    }

    void checkCollisionWithEdges(){

        if(snake.body[0].x == cellCount || snake.body[0].x == -1){
            GameOver();
        }

        if(snake.body[0].y == cellCount || snake.body[0].y == -1){
            GameOver();
        }

    }

    void checkCollisionWithTail(){

        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();

        if(element_inDeque(snake.body[0], headlessBody)){
            GameOver();
        }


    }

    void GameOver(){
        snake.Reset();
        food.position = food.GenerateRandomPosition(snake.body);
        running = false;
    }
};


int main() {   
    
    InitWindow( 2*offset + cellSize*cellCount, 2*offset + cellSize*cellCount, "Retro Snake" );
    SetTargetFPS(60);

    Game game = Game();

    while(WindowShouldClose() == false){

        /* begin drawing for the new iteration of the gameloop.*/
        BeginDrawing();
        
        if(eventTriggered(0.2)){
            game.Update();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = {0, 1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = {1, 0};
            game.running = true;
        }


        /* clear background */
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5,
                                       (float)offset - 5,
                                       (float)cellSize*cellCount + 10,
                                       (float)cellSize*cellCount + 10}, 
                                        5, darkgreen); 
        
                                        
        game.Draw();

        EndDrawing();

    }

    CloseWindow();
    return 0;
}