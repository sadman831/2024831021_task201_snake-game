#include<SDL2/SDL.h>
#include<iostream>
#include<deque>
#include<string>
#include<cstdlib>
#include<ctime>

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 20;

// Remove tail if no food was eaten
enum Direction { UP, DOWN, LEFT, RIGHT };

struct GameState {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = true;
    Direction dir = RIGHT;
    int score = 0;
    std::deque<SDL_Rect> snake;
    SDL_Rect food;
};

// SpawnFood must be declared before Initialize and Update
void SpawnFood(GameState& state) {
    state.food.w = GRID_SIZE;
    state.food.h = GRID_SIZE;
    state.food.x = (rand() % (SCREEN_WIDTH / GRID_SIZE)) * GRID_SIZE;
    state.food.y = (rand() % (SCREEN_HEIGHT / GRID_SIZE)) * GRID_SIZE;
}

bool Initialize(GameState& state) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL Initialization Error: " << SDL_GetError() << std::endl;
        return false;
    }

    state.window = SDL_CreateWindow("Snake Game | Play Now!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!state.window) return false;

    state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED);
    if (!state.renderer) return false;

    // REQUIREMENT 1: Medium Starting Length 
    int startingLength = 5; 
    for (int i = 0; i < startingLength; i++) {
        SDL_Rect segment = {(SCREEN_WIDTH / 2) - (i * GRID_SIZE), SCREEN_HEIGHT / 2, GRID_SIZE, GRID_SIZE};
        state.snake.push_back(segment);
    }

    srand(time(0));
    SpawnFood(state);
    return true;
}

void ProcessInput(GameState& state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            state.running = false;
        } 
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:    if (state.dir != DOWN) state.dir = UP; break;
                case SDLK_DOWN:  if (state.dir != UP) state.dir = DOWN; break;
                case SDLK_LEFT:  if (state.dir != RIGHT) state.dir = LEFT; break;
                case SDLK_RIGHT: if (state.dir != LEFT) state.dir = RIGHT; break;
            }
        }
    }
}

void ShowScoreboard(GameState& state) {
    // REQUIREMENT 5: Scoreboard Pop-up at the end 
    std::string scoreboardText = "Ooh! Game Over.\n\nYour Final Score: " + std::to_string(state.score) + "\n\nPress OK to exit.";
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Snake Scoreboard", scoreboardText.c_str(), state.window);
}

void Cleanup(GameState& state) {
    if (state.renderer) SDL_DestroyRenderer(state.renderer);
    if (state.window) SDL_DestroyWindow(state.window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    GameState state; 

    if (!Initialize(state)) {
        Cleanup(state);
        return 1;
    }

    // Main Game Loop
    while (state.running) {
        ProcessInput(state);
    }

    ShowScoreboard(state);
    Cleanup(state);
    return 0;
}
