#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>
#include<SDL_image.h>
#include<iostream>

class Animation {
public:
    Animation(char*file,int frames,float ptime);
    ~Animation();
    bool init(SDL_Renderer *renderer);
    void update(float deltaTime);
    void getCurrentFrame(SDL_Rect &frame);
    void clean();
    SDL_Texture* getTexture() const { return texture; };

private:
    SDL_Texture* texture=nullptr;
    int frameWidth, frameHeight;
    int currentFrameIndex;
    int totalFrames;
    float timePerFrame; // 每帧持续时间（秒）
    float elapsedTime; // 累积的时间
    char* filePath; // 纹理文件路径
    bool isframestraight=true;//判断sprite sheet是不是水平，或者竖直
};

#endif // ANIMATION_H