#include <iostream>
#include <SDL3/SDL.h>
#include <cstdlib>

using namespace std;

// Constants for screen size, grid cells, and colors
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600
#define CELL_SIZE 30
#define LINE_WIDTH 2
#define GRID_COLOR 0x1f1f1f1f
#define WHITE_COLOR 0xffffffff
#define GREEN_COLOR 0xff90ee90
#define RED_COLOR 0x00ff0000
#define BLACK_COLOR 0x00000000

// Calculate rows and columns based on screen size and cell size
const int ROWS = SCREEN_HEIGHT / CELL_SIZE;
const int COLUMNS = SCREEN_WIDTH / CELL_SIZE;

// Helper function to get a random number in a range [a, b]
int randomNumber(int a, int b)
{
    return a + rand() % (b - a + 1);
}

// Class representing one segment of the snake's body as a linked list node
class Snake
{
public:
    // Constructor: initialize position and movement direction
    Snake(int x, int y) : posX(x), posY(y), next(nullptr), dx(0), dy(0) {}

    // Destructor: recursively deletes the next segments to free all memory
    ~Snake()
    {
        delete next;
    }

    // Getters and setters for position and movement direction
    int getX() const { return posX; }
    int getY() const { return posY; }
    int getdX() const { return dx; }
    int getdY() const { return dy; }
    Snake *getNextPtr() const { return next; }
    void setX(int x) { posX = x; }
    void setY(int y) { posY = y; }
    void setdX(int d_x) { dx = d_x; }
    void setdY(int d_y) { dy = d_y; }
    void setNextPtr(Snake *snakePtr) { next = snakePtr; }

    // Calculate the current length of the snake (number of segments)
    int getScore() const
    {
        int length = 0;
        const Snake *ptr = this;
        while (ptr != nullptr)
        {
            length++;
            ptr = ptr->next;
        }
        return length;
    }

private:
    int posX, posY; // Position on the grid
    int dx, dy;     // Movement direction (change in x and y per step)
    Snake *next;    // Pointer to the next segment in the snake's body
};

// Class to manage the apple's position on the grid
class Apple
{
public:
    Apple(int x = 0, int y = 0) : posX(x), posY(y) {}

    // Get current apple position
    int getX() const { return posX; }
    int getY() const { return posY; }

    // Place apple at a random position, avoiding the snake's body
    void resetApple(const Snake *snakePtr)
    {
        while (true)
        {
            posX = randomNumber(0, COLUMNS - 1);
            posY = randomNumber(0, ROWS - 1);

            // Check if apple collides with any snake segment
            const Snake *ptr = snakePtr;
            bool collision = false;
            while (ptr != nullptr)
            {
                if (ptr->getX() == posX && ptr->getY() == posY)
                {
                    collision = true;
                    break;
                }
                ptr = ptr->getNextPtr();
            }

            if (!collision)
                break; // Valid position found
        }
    }

private:
    int posX, posY; // Apple position on the grid
};

// Main game class controlling game state, rendering, and input
class Game
{
public:
    // Initialize SDL, create window and surface, start with snake and apple at set positions
    Game() : snake(6, 9), apple(12, 18), running(true)
    {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Snake Game", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        surface = SDL_GetWindowSurface(window);
    }

    // Clean up SDL resources and snake linked list memory on game exit
    ~Game()
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        // snake destructor frees linked list
    }

    // Main game loop: handle input, update state, draw, and delay to control speed
    void run()
    {
        SDL_Event event;
        SDL_Rect fullRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

        while (running)
        {
            // Handle all pending SDL events (input, window close, etc.)
            while (SDL_PollEvent(&event))
            {
                handleEvent(event);
            }

            // Clear the screen to black before drawing new frame
            SDL_FillSurfaceRect(surface, &fullRect, BLACK_COLOR);

            // Move the snake in the current direction
            moveSnake();

            // Check if the snake collided with walls or itself (game over)
            if (checkCollision())
            {
                cout << "Game Over! Score: " << snake.getScore() << endl;
                break;
            }

            // If snake eats apple, grow and reset apple position
            if (snake.getX() == apple.getX() && snake.getY() == apple.getY())
            {
                growSnake();
                apple.resetApple(&snake);
            }

            // Draw the apple, snake, and grid on the screen
            drawCell(apple.getX(), apple.getY(), RED_COLOR);
            drawSnake();
            drawGrid();

            // Update the window surface to show new frame
            SDL_UpdateWindowSurface(window);

            // Wait 100ms to control game speed (~10 FPS)
            SDL_Delay(100);
        }
    }

private:
    SDL_Window *window;
    SDL_Surface *surface;
    Snake snake;
    Apple apple;
    bool running;

    // Draw a filled cell (rectangle) at grid position (x, y) with given color
    void drawCell(int x, int y, Uint32 color)
    {
        SDL_Rect rect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_FillSurfaceRect(surface, &rect, color);
    }

    // Draw the grid lines for visual reference of cells
    void drawGrid()
    {
        SDL_Rect line = {0, 0, SCREEN_WIDTH, LINE_WIDTH};
        for (int i = 0; i < ROWS; i++)
        {
            line.y = i * CELL_SIZE;
            SDL_FillSurfaceRect(surface, &line, GRID_COLOR);
        }
        line = {0, 0, LINE_WIDTH, SCREEN_HEIGHT};
        for (int i = 0; i < COLUMNS; i++)
        {
            line.x = i * CELL_SIZE;
            SDL_FillSurfaceRect(surface, &line, GRID_COLOR);
        }
    }

    // Draw each segment of the snake as green squares on the grid
    void drawSnake()
    {
        const Snake *ptr = &snake;
        while (ptr != nullptr)
        {
            drawCell(ptr->getX(), ptr->getY(), GREEN_COLOR);
            ptr = ptr->getNextPtr();
        }
    }

    // Handle keyboard input and window close events
    void handleEvent(const SDL_Event &event)
    {
        if (event.type == SDL_EVENT_QUIT)
            running = false;

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            // Get current movement direction
            int tempDX = snake.getdX();
            int tempDY = snake.getdY();

            // Change direction based on arrow keys, avoiding reversing direction
            switch (event.key.scancode)
            {
            case SDL_SCANCODE_RIGHT:
                tempDX = 1;
                tempDY = 0;
                break;
            case SDL_SCANCODE_LEFT:
                tempDX = -1;
                tempDY = 0;
                break;
            case SDL_SCANCODE_DOWN:
                tempDX = 0;
                tempDY = 1;
                break;
            case SDL_SCANCODE_UP:
                tempDX = 0;
                tempDY = -1;
                break;
            default:
                break;
            }

            // Prevent snake from reversing directly
            if (!(tempDX == -snake.getdX() && tempDY == -snake.getdY()))
            {
                snake.setdX(tempDX);
                snake.setdY(tempDY);
            }
        }
    }

    // Move the snake forward by updating positions of all segments
    void moveSnake()
    {
        int prevX = snake.getX();
        int prevY = snake.getY();

        // Move the head
        snake.setX(prevX + snake.getdX());
        snake.setY(prevY + snake.getdY());

        // Move each body segment to the position of the segment ahead of it
        Snake *current = snake.getNextPtr();
        while (current)
        {
            int tempX = current->getX();
            int tempY = current->getY();
            current->setX(prevX);
            current->setY(prevY);
            prevX = tempX;
            prevY = tempY;
            current = current->getNextPtr();
        }
    }

    // Add a new segment at the snake's tail
    void growSnake()
    {
        Snake *ptr = &snake;
        while (ptr->getNextPtr())
            ptr = ptr->getNextPtr();
        ptr->setNextPtr(new Snake(ptr->getX(), ptr->getY()));
    }

    // Check if snake hits wall or itself (game over condition)
    bool checkCollision()
    {
        // Check boundary collision
        if (snake.getX() < 0 || snake.getX() >= COLUMNS || snake.getY() < 0 || snake.getY() >= ROWS)
            return true;

        // Check self-collision
        const Snake *head = &snake;
        const Snake *ptr = head->getNextPtr();
        while (ptr)
        {
            if (head->getX() == ptr->getX() && head->getY() == ptr->getY())
                return true;
            ptr = ptr->getNextPtr();
        }
        return false;
    }
};

int main()
{
    Game game;
    game.run(); // Start the game loop
    return 0;
}
