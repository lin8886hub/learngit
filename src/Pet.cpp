#include "Pet.h"
#include "Game.h"
Pet::Pet()
{
    speedX = 0.0f;
    speedY = 0.0f;
    position = {0.0f, 0.0f};
    currentState = IDLE;
    idleAnimation=nullptr;
    boxAnimation=nullptr;
    exitingAnimation=nullptr;
    currentanimation=nullptr;

}

Pet::~Pet()
{
    Mix_CloseAudio();
    delete idleAnimation;
    idleAnimation=nullptr;
    delete boxAnimation;
    boxAnimation=nullptr;
    delete exitingAnimation;
    exitingAnimation=nullptr;
    clean();
}

bool Pet::init(SDL_Renderer *renderer)
{
     //初始化SDL_image库
    if(IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "Failed to initialize SDL_image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    //初始化mixer并载入音频
     if (Mix_OpenAudio(
            44100,           // 采样率
            MIX_DEFAULT_FORMAT,
            2,               // 立体声
            2048             // 缓冲区
        ) < 0)
    {
        SDL_Log("Mix_OpenAudio Error: %s", Mix_GetError());
        return false;
    }
    

    // 初始化动画
    idleAnimation = new Animation("asserts/fixed_soft.png", 10,0.25f); // idle动画有10帧
    if(!idleAnimation->init(renderer)) {
        std::cerr << "Failed to initialize idle animation!" << std::endl;
        return false;
    }
    boxAnimation = new Animation("asserts/box.png", 4,0.1f); // box动画有4帧
    if(!boxAnimation->init(renderer)) {
        std::cerr << "Failed to initialize box animation!" << std::endl;
        return false;
    }
    exitingAnimation = new Animation("asserts/yileina_q_fixed.png", 18,0.5f); // exiting动画有6帧
    if(!exitingAnimation->init(renderer)) {
        std::cerr << "Failed to initialize exiting animation!" << std::endl;
        return false;
    }
    //初始化状态
    currentState=IDLE;
    currentanimation=idleAnimation;
    return true;
}

void Pet::handleEvents(const SDL_Event &event)
{
    mousedragging(event);
}

void Pet::update(float deltaTime)
{
    StateChange();

    currentanimation->update(deltaTime);
   
}
void Pet::render(SDL_Renderer *renderer)
{
    // 当前动画的一帧（源矩形）
    SDL_Rect srcFrame;
    currentanimation->getCurrentFrame(srcFrame);

    //放置位置
   SDL_Rect dstFrame;
   dstFrame.x = (int)position.x;
   dstFrame.y = (int)position.y;
   dstFrame.w = srcFrame.w;
   dstFrame.h = srcFrame.h;

   SDL_RenderCopy(renderer, currentanimation->getTexture(), &srcFrame, &dstFrame);
}


void Pet::clean()
{
  //退出SDL_image
   IMG_Quit();
}

void Pet::StateChange()
{
    if(!dragging){
     auto keyboardstate=SDL_GetKeyboardState(NULL);
    //i键为idle，b键为box，E键为exiting,
    if(keyboardstate[SDL_SCANCODE_I]) {
        currentanimation=idleAnimation;
        currentState=IDLE;  
    }
    if(keyboardstate[SDL_SCANCODE_B]){
        currentanimation=boxAnimation;
        currentState=BOX;
    }
    if(keyboardstate[SDL_SCANCODE_E]) {
        currentanimation=exitingAnimation;
        currentState=EXITING;
    }
    if(keyboardstate[SDL_SCANCODE_K]){
        Mix_Music* bgm = Mix_LoadMUS("asserts/music.mp3");
    if (!bgm)
    {
    SDL_Log("Load Music Error: %s", Mix_GetError());
    }
       Mix_PlayMusic(bgm, 1); 
       
    }
    }
    else{
        currentanimation=idleAnimation;
        currentState=IDLE;
    }
}

void Pet::mousedragging(const SDL_Event &event) 
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        // 判断是否点中宠物
        SDL_Rect petRect = {
            (int)position.x,
            (int)position.y,
            300,
            300
        };

        SDL_Point mousepoint{mouseX, mouseY};
        if (SDL_PointInRect(&mousepoint, &petRect))
        {
            dragging = true;

            // ⭐ 记录鼠标相对宠物的偏移
            dragOffsetX = mouseX - (int)position.x;
            dragOffsetY = mouseY - (int)position.y;
        }
    }

    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        dragging = false;
    }

    else if (event.type == SDL_MOUSEMOTION)
    {
        if (dragging)
        {
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;

            // ⭐ 跟随鼠标（减去偏移）不能超过屏幕边界
            position.x = (float)(mouseX - dragOffsetX);
            position.y = (float)(mouseY - dragOffsetY);
            // 限制宠物位置在屏幕内
            Game& game=Game::getInstance();
            int SCREEN_WIDTH=game.getWindowWidth();
            int SCREEN_HEIGHT=game.getWindowHeight();
            if (position.x < 0) position.x = 0;
            if (position.y < 0) position.y = 0;
            if (position.x > SCREEN_WIDTH - 300) position.x = (float)(SCREEN_WIDTH - 300);
            if (position.y > SCREEN_HEIGHT - 300) position.y = (float)(SCREEN_HEIGHT - 300);
        }
    }
}

