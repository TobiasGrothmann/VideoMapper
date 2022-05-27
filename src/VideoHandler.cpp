#include "VideoHandler.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

VideoHandler::VideoHandler()
    : looping(true)
    , soundOn(false)
    , windowSize(vec2(100, 100))
{
    videoScale[0] = 1.0f;
    videoScale[1] = 1.0f;
    videoOffset[0] = 0.5f;
    videoOffset[1] = 0.5f;
}

bool VideoHandler::load(const string& pathToVideoFile)
{
    try
    {
        video = qtime::MovieGl::create(pathToVideoFile);
        loopingUpdated();
        soundOnUpdated();
        video->play();
        console() << "Playing: " << video->isPlaying() << std::endl;
    }
    catch (ci::Exception &exc)
    {
        console() << "Exception caught trying to load the video from path: " << pathToVideoFile << ", what: " << exc.what() << std::endl;
        video.reset();
        return false;
    }
    
    handleResize();
    frameTexture.reset();
    return true;
}

void VideoHandler::update()
{
    if (!video) return;
    frameTexture = video->getTexture();
}

void VideoHandler::draw()
{
    if (frameTexture)
    {
        const vec2 videoSize = video->getSize();
        const vec2 actualDrawnSize = videoSize / getVideoScale();
        const vec2 offsetInVideo = (videoSize - actualDrawnSize) * getVideoOffset();
        
        gl::draw(frameTexture, Area(offsetInVideo, offsetInVideo + actualDrawnSize), Rectf(ivec2(0, 0), getVideoRenderSize()));
    }
}

void VideoHandler::setWindowSize(const cinder::vec2& newWindowSize)
{
    windowSize = newWindowSize;
}

void VideoHandler::handleResize()
{
    if (!video) return;
    
    videoRenderSize = video->getSize();
    if (videoRenderSize.x > windowSize.x)
    {
        videoRenderSize *= windowSize.x / videoRenderSize.x;
    }
    if (videoRenderSize.y > windowSize.y)
    {
        videoRenderSize *= windowSize.y / videoRenderSize.y;
    }

    cout << "video is rendered at: " << videoRenderSize << " - window size is: " << windowSize << endl;
}

void VideoHandler::loopingUpdated()
{
    if (!video) return;
    video->setLoop(looping);
}

void VideoHandler::soundOnUpdated()
{
    if (!video) return;
    video->setVolume(soundOn ? 1.0f : 0.0f);
}

vec2 VideoHandler::getVideoOffset() const
{
    return vec2(videoOffset[0], videoOffset[1]);
}

vec2 VideoHandler::getVideoScale() const
{
    return vec2(videoScale[0], videoScale[1]);
}

void VideoHandler::videoScaleUpdated()
{
    if (!video) return;
    handleResize();
}

void VideoHandler::seekToFraction(float fraction /*0.0 to 1.0*/)
{
    if (!video) return;
    video->seekToTime(video->getDuration() * fraction);
}

void VideoHandler::seekToSeconds(float seconds)
{
    if (!video) return;
    video->seekToTime(seconds);
}

void VideoHandler::setPlaying(bool shouldPlay)
{
    if (!video) return;
    if (shouldPlay && !video->isPlaying())
    {
        video->play();
    }
    else if (video->isPlaying())
    {
        video->stop();
    }
}

bool VideoHandler::isLoaded() const
{
    return !!video;
}

bool VideoHandler::isLooping() const
{
    return looping;
}

bool VideoHandler::isPlaying() const
{
    return video && video->isPlaying();
}

float VideoHandler::getPlaybackSeconds() const
{
    if (!video) return 0.0;
    return video->getCurrentTime();
}

vec2 VideoHandler::getVideoRenderSize() const
{
    return videoRenderSize;
}
