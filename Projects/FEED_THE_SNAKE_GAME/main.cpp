#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;
Color beige = {211, 176, 131, 255};
Color darkGreen = {43, 51, 24, 255};

int cellsize = 25;
int cellcount = 20;
int offset = 55;
double lastupdatetime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for (unsigned int i = 0; i <deque.size(); i++)
    {
        if(Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval)
{
    double currenttime = GetTime();
    if(currenttime - lastupdatetime >= interval)
    {
        lastupdatetime = currenttime;
        return true;
    }
    return false;

}

class Snake
 {
    public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    bool addsegment = false;


    void Draw()
    {
        for(unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x * cellsize, offset + y * cellsize, (float)cellsize, (float)cellsize};
            DrawRectangleRounded(segment, 0.5, 6, darkGreen);
        }
    }

    void Update()
    {
        body.push_front(Vector2Add(body[0], direction));
        if(addsegment == true)
        {
            addsegment = false;
        }
        else
        {
        body.pop_back();
        
        }
    }

    void Reset()
    {
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = {1,0};
    }
};

class Food {
public:
    Vector2 position;
    Texture2D texture;

    // Constructor
    Food(deque<Vector2> snakeBody)
     {
        Image image = LoadImage("Graphics/food.png");  // Load image from file
        texture = LoadTextureFromImage(image);         // Convert image to texture
        UnloadImage(image);        
        position = GenerateRandomPos(snakeBody);                  // Unload image from memory
    }

    // Destructor
    ~Food() {
        UnloadTexture(texture);                        // Free texture resources
    }

    // Draw function
    void Draw() {
        DrawTexture(texture, offset + position.x * cellsize, offset + position.y * cellsize, WHITE);
    }

    Vector2 GenerateRandomCell()
    {        float x = GetRandomValue(0, cellcount - 1);
        float y = GetRandomValue(0, cellcount - 1);
        return Vector2{x, y};
    }
    
    Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
    {
        Vector2 position = GenerateRandomCell();
        while(ElementInDeque(position, snakeBody))
        {
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Game 
{
    public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;
    Sound eatSound;
    Sound wallSound;

    Game()
    {
        InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
        wallSound = LoadSound("Sounds/wall.mp3");
    }

    ~Game()
    {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }


    void draw()
    {
        snake.Draw();
        food.Draw();
    }

    void update()
    {
        if (running)
        {
            snake.Update();
            Checkcollisionwithfood();
            Checkcollisionwithedge();
            Checkcollisionwithtail();
        }
        }
        

    void Checkcollisionwithfood()
    {
        if(Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addsegment = true;
            score ++;
            PlaySound(eatSound);
        }


    }

    void Checkcollisionwithedge()
    {
        if(snake.body[0].x == cellcount || snake.body[0].x == -1)
        {
            GameOver();
        }
        if(snake.body[0].y == cellcount || snake.body[0].y == -1)
        {
            GameOver();
        }
    
    }

    void GameOver()
    {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wallSound);
    }

    void Checkcollisionwithtail()
    {
        deque<Vector2> headlessbody = snake.body;
        headlessbody.pop_front();
        if (ElementInDeque(snake.body[0],headlessbody))
        {
            GameOver();
        }
    }
};

int main() {
    cout << "Starting the game....." << endl;
    
    // Initialize window
    InitWindow(2*offset + cellsize * cellcount, 2*offset + cellsize * cellcount, "retro snake");
    SetTargetFPS(60);
    Game game = Game();



    // Game loop
    while (!WindowShouldClose()) {  // Simplified check for window closing
        BeginDrawing();
        if (eventTriggered(0.2))
        {
            game.update();
        }
        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0, 1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1, 0};
            game.running = true;
        }

        // Clear the background
        ClearBackground(beige);
        DrawText("FEED THE SNAKE", offset -5, 20, 40, darkGreen);
        DrawText(TextFormat("%i", game.score), offset - 5, offset+cellsize * cellcount+10, 40, darkGreen);
        // Draw food
        game.draw();
        DrawRectangleLinesEx(Rectangle{(float)offset-5,(float)offset-5, (float)cellsize * cellcount+10, (float)cellsize * cellcount+10}, 5, darkGreen);

        EndDrawing();
    }

    // Close window and cleanup
    CloseWindow();

    return 0;
}
