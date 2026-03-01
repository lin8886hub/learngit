#include "Game.h"
#include <SDL.h>



// 定义类内声明的静态渲染器指针，防止链接时找不到符号
SDL_Renderer* Game::renderer = nullptr;



Game::Game()
    : window(nullptr),isRunning(false)
{
}

Game::~Game()
{
    clean();
}
bool Game::init()
{
    // 初始化SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
            "SDL could not initialize! SDL_Error: %s",
            SDL_GetError());
        return false;
    }

    // 创建窗口
    window = SDL_CreateWindow(
        "Desk Pet",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WindowWidth,
        WindowHeight,
        SDL_WINDOW_BORDERLESS
    );

    if(window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
            "Window could not be created! SDL_Error: %s",
            SDL_GetError());
        return false;
    }

    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
            "Renderer could not be created! SDL_Error: %s",
            SDL_GetError());
        return false;
    }

    // 获取 Windows HWND
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    HWND hwnd = wmInfo.info.win.window;

    // 设置 Layered Window
    SetWindowLong(hwnd, GWL_EXSTYLE,
        GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    // ⭐ 黑色变透明（ColorKey）
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    // 初始化宠物
    if(!pet.init(renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
            "Failed to initialize pet!");
        return false;
    }

    isRunning = true;
    return true;
}
void Game::run()
{
    

   while(isRunning) {
        Uint32 frameStart = SDL_GetTicks();
        handleEvents();
        // 使用当前记录的 member deltatime 作为上一帧的 delta
        update(deltatime);
        render();
        Uint32 frameEnd = SDL_GetTicks();
        Uint32 frameTime = frameEnd - frameStart;
        if(frameTime < frameDelay) {
            SDL_Delay(static_cast<uint32_t>(frameDelay) - frameTime);
            // 实际帧时间为目标帧时间
            deltatime = frameDelay / 1000.0f;
        } else {
            // 实际帧时间为测量值
            deltatime = frameTime / 1000.0f; // seconds
        }
    }
}
void Game::handleEvents()
{
    SDL_Event event;
    while(SDL_PollEvent(&event)) {

        // 处理退出事件
        if(event.type == SDL_QUIT) {
            isRunning = false;
        }

        // 处理宠物相关事件
       pet.handleEvents(event);
    }
}

void Game::update(float delta)
{
    pet.update(delta);
}

void Game::render()
{
    // 使用黑色背景（给 ColorKey 抠掉）
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    pet.render(renderer);

    SDL_RenderPresent(renderer);
}
void Game::clean()
{
    
    // 释放资源并置空指针，防止二次释放
    if(renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if(window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}
