#include "VideoHandler.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

bool VideoHandler::load(const string& pathToVideoFile)
{
    try
    {
        video = qtime::MovieGl::create(pathToVideoFile);
        setLooping(looping);
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
//            Rectf centeredRect = Rectf(mFrameTexture->getBounds()).getCenteredFit(getWindowBounds(), true);
        const Rectf drawBounds = Rectf(vec2(0, 0), videoRenderSize);
        gl::draw(frameTexture, drawBounds);
    }

}

void VideoHandler::handleResize()
{
    if (!video) return;
    const vec2 windowSize = getWindowSize();
    
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

void VideoHandler::setLooping(bool shouldLoop)
{
    looping = shouldLoop;
    if (!video) return;
    video->setLoop(shouldLoop);
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
