#ifndef PET_H
#define PET_H   

#include "animation.h"
#include <SDL.h>
#include <SDL_image.h>
#include<SDL_mixer.h>
#include<iostream>


class Pet {
public:
    Pet();
    ~Pet();
    bool init(SDL_Renderer *renderer);
    void handleEvents(const SDL_Event& event);
    void update(float deltaTime);
    void render(SDL_Renderer *renderer);
    void clean();
    void StateChange();
    void mousedragging(const SDL_Event& event);

private:
    //状态
    enum State { IDLE, BOX, EXITING } currentState;
   //移动速度
   float speedX, speedY;
   //重力效果
   float gravity = 800.0f; // 像素/秒²
   //位置
   SDL_FPoint position;

   //不同状态下的animation
   Animation* idleAnimation;
   Animation* boxAnimation;
   Animation* exitingAnimation;
   //当前状态对应的animation
    Animation* currentanimation;
    //鼠标
    bool dragging = false;
    int dragOffsetX = 0;
    int dragOffsetY = 0;

};

#endif // PET_H
