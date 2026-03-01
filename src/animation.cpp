#include "animation.h"
#include <string>

Animation::Animation(char*file,int frames,float ptime)
     :
    filePath(file),
    texture(nullptr), frameWidth(0), frameHeight(0), currentFrameIndex(0),
    totalFrames(frames), timePerFrame(ptime), elapsedTime(0.0f)
{

}

Animation::~Animation()
{
    clean();
}

bool Animation::init(SDL_Renderer *renderer)
{
    // 按传入路径加载纹理
    texture = IMG_LoadTexture(renderer, filePath);
    if(texture == nullptr) {
        std::cerr << "Failed to load texture: " << filePath
                  << " IMG_Error: " << IMG_GetError() << std::endl;
        return false;
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    // 获取纹理宽高
    int textureWidth = 0;
    int textureHeight = 0;
    if(SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight) != 0) {
        std::cerr << "Failed to query texture: " << filePath
                  << " SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(texture);
        texture = nullptr;
        return false;
    }
    //不同sprite sheet有不同的处理方式
    //sprit sheet是水平的
    if(textureHeight<=textureWidth){
        frameWidth = textureWidth / totalFrames;
        frameHeight = textureHeight;
        isframestraight=true;

    }
    //sprite sheet 是竖直的
    else if(textureHeight>textureWidth){
        frameWidth = textureWidth;
        frameHeight = textureHeight/totalFrames;
        isframestraight=false;
    }
    return true;
}

void Animation::update(float deltaTime)
{
        elapsedTime += deltaTime;
        
        if(elapsedTime >= timePerFrame) {
            currentFrameIndex++;
            elapsedTime = 0.0f; // 重置时间
        }
        if(currentFrameIndex >= totalFrames) {
                currentFrameIndex = 0; // 循环播放
        }
}

void Animation::getCurrentFrame(SDL_Rect &frame)
{
    if(isframestraight){
    frame.x = currentFrameIndex * frameWidth;
    frame.y = 0;
    frame.w = frameWidth;
    frame.h = frameHeight;
    }
   else{
    frame.x = 0;
    frame.y = currentFrameIndex * frameHeight;
    frame.w = frameWidth;
    frame.h = frameHeight;
}

}

void Animation::clean()
{
    if(texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
