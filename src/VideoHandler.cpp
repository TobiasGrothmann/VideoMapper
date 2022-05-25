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
        console() << "Exception caught trying to load the movie from path: " << pathToVideoFile << ", what: " << exc.what() << std::endl;
        video.reset();
        return false;
    }
    
    handleResize();
    frameTexture.reset();
    return true;
}

void VideoHandler::update()
{
    if (video)
    {
        frameTexture = video->getTexture();
    }
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
    video->setLoop(shouldLoop);
}

bool VideoHandler::isLoaded() const
{
    return !!video;
}

bool VideoHandler::isLooping() const
{
    return looping;
}

vec2 VideoHandler::getVideoRenderSize() const
{
    return videoRenderSize;
}
