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
    void seekToFraction(float fraction /*0.0 to 1.0*/);
    void seekToSeconds(float seconds);
    void setPlaying(bool shouldPlay);
    
    bool isLoaded() const;
    bool isLooping() const;
    bool isPlaying() const;
    float getPlaybackSeconds() const;
    cinder::vec2 getVideoRenderSize() const;
    
private:
    
    cinder::gl::TextureRef frameTexture;
    cinder::qtime::MovieGlRef video;
    
    cinder::vec2 videoRenderSize;
    
    bool looping = false;
};
