#pragma once
#include "cinder/qtime/QuickTimeGl.h"
#include "cinder/gl/gl.h"

class VideoHandler
{
public:
    VideoHandler();
    ~VideoHandler() = default;
    
    bool load(const std::string& pathToVideoFile);
    void update();
    void draw();
    
    void setWindowSize(const cinder::vec2& windowSize);
    void handleResize();
    
    bool looping;
    void loopingUpdated();
    
    bool soundOn;
    void soundOnUpdated();
    
    float videoOffset[2];
    cinder::vec2 getVideoOffset() const;
    
    float videoScale[2];
    cinder::vec2 getVideoScale() const;
    void videoScaleUpdated();
    
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
    cinder::vec2 windowSize;
};
