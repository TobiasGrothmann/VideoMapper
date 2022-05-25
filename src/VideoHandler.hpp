#pragma once
#include "cinder/qtime/QuickTimeGl.h"
#include "cinder/gl/gl.h"

class VideoHandler
{
public:
    
    VideoHandler() = default;
    
    bool load(const std::string& pathToVideoFile);
    void update();
    void draw();
    
    void handleResize();
    
    void setLooping(bool shouldLoop);
    
    bool isLoaded() const;
    bool isLooping() const;
    cinder::vec2 getVideoRenderSize() const;
    
private:
    
    cinder::gl::TextureRef frameTexture;
    cinder::qtime::MovieGlRef video;
    
    cinder::vec2 videoRenderSize;
    
    bool looping = false;
};
