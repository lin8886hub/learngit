#ifndef GAME_H
#define GAME_H

#include"Pet.h"
#include<SDL_syswm.h>
#include<windows.h>

class Game {
public:
    // 单例访问（静态方法）
    static Game& getInstance() {
        static Game instance;
        return instance;
    }
    ~Game();
    bool init();
    void run();
    int getWindowWidth() const { return WindowWidth; }
    int getWindowHeight() const { return WindowHeight; }
    SDL_Renderer* getRenderer() const { return renderer; }
    SDL_Window*  getWindow() const { return window; }
    
private:
    // 禁止复制和赋值
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    Game();
    void handleEvents();
    void update(float delta);
    void render();
    void clean();
    SDL_Window* window;
    static SDL_Renderer* renderer;
    bool isRunning=true;
    int WindowWidth=800;
    int WindowHeight=800;
    Pet pet;
    int FPS=60;
    float frameDelay=1000.0f/FPS; // 每帧的时间（毫秒）
    float deltatime=1.0f/FPS; // 假设每帧16ms
 
};


#endif // GAME_H

