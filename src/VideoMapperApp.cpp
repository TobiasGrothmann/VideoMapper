#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

// cinder block: Cinder-Warping
#include "Warp.h"

#include "VideoHandler.hpp"

using namespace ci;
using namespace ci::app;
using namespace ph::warping;
using namespace std;

class VideoMapperApp : public App
{
public:
    void setup() override;
    void cleanup() override;

    void keyDown(KeyEvent event) override;
    void fileDrop(FileDropEvent event) override;
    
    void mouseMove(MouseEvent event) override;
    void mouseDown(MouseEvent event) override;
    void mouseDrag(MouseEvent event) override;
    void mouseUp(MouseEvent event) override;
    
    void resize() override;

    void update() override;
    void draw() override;
    
    void selectAndLoadNewVideo();
    
    VideoHandler videoHandler = {};
    
    fs::path settingsFilePath;
    WarpList warps;
};

void VideoMapperApp::setup()
{
    disableFrameRate();
    enablePowerManagement(false);
//    hideCursor();

    // initialize warps
    settingsFilePath = getAssetPath("") / "warps.xml";
    cout << "settings are at: " << settingsFilePath << endl;
    if (fs::exists(settingsFilePath))
    {
        warps = Warp::readSettings(loadFile(settingsFilePath));
    }
    else
    {
        warps.push_back(WarpPerspective::create());
    }
    
    // load video
    selectAndLoadNewVideo();
    
    warps[0]->setSize(videoHandler.getVideoRenderSize());
}

void VideoMapperApp::cleanup()
{
    Warp::writeSettings(warps, writeFile(settingsFilePath));
}

void VideoMapperApp::keyDown(KeyEvent event)
{
    if (!Warp::handleKeyDown(warps, event))
    {
        switch (event.getCode())
        {
            // video playback
            case KeyEvent::KEY_RIGHT:
                videoHandler.seekToSeconds(videoHandler.getPlaybackSeconds() + 5);
                break;
            case KeyEvent::KEY_LEFT:
                videoHandler.seekToSeconds(videoHandler.getPlaybackSeconds() - 5);
                break;
            case KeyEvent::KEY_l:
                videoHandler.seekToSeconds(videoHandler.getPlaybackSeconds() + 10);
                break;
            case KeyEvent::KEY_j:
                videoHandler.seekToSeconds(videoHandler.getPlaybackSeconds() - 10);
                break;
            case KeyEvent::KEY_SPACE:
            case KeyEvent::KEY_k:
                videoHandler.setPlaying(!videoHandler.isPlaying());
                break;
            case KeyEvent::KEY_0:
                videoHandler.seekToFraction(0.0f);
                break;
            case KeyEvent::KEY_1:
                videoHandler.seekToFraction(0.1f);
                break;
            case KeyEvent::KEY_2:
                videoHandler.seekToFraction(0.2f);
                break;
            case KeyEvent::KEY_3:
                videoHandler.seekToFraction(0.3f);
                break;
            case KeyEvent::KEY_4:
                videoHandler.seekToFraction(0.4f);
                break;
            case KeyEvent::KEY_5:
                videoHandler.seekToFraction(0.5f);
                break;
            case KeyEvent::KEY_6:
                videoHandler.seekToFraction(0.6f);
                break;
            case KeyEvent::KEY_7:
                videoHandler.seekToFraction(0.7f);
                break;
            case KeyEvent::KEY_8:
                videoHandler.seekToFraction(0.8f);
                break;
            case KeyEvent::KEY_9:
                videoHandler.seekToFraction(0.9f);
                break;
            // other
            case KeyEvent::KEY_o:
                selectAndLoadNewVideo();
                break;
            case KeyEvent::KEY_f:
                setFullScreen(!isFullScreen());
                break;
            case KeyEvent::KEY_v:
                gl::enableVerticalSync(!gl::isVerticalSyncEnabled());
                break;
            case KeyEvent::KEY_w:
                Warp::enableEditMode(!Warp::isEditModeEnabled());
                break;
        }
    }
}

void VideoMapperApp::resize()
{
    videoHandler.handleResize();
    warps[0]->setSize(videoHandler.getVideoRenderSize());
    Warp::handleResize(warps);
}

void VideoMapperApp::mouseMove(MouseEvent event)
{
    if (!Warp::handleMouseMove(warps, event))
    {
    }
}

void VideoMapperApp::mouseDown(MouseEvent event)
{
    if (!Warp::handleMouseDown(warps, event))
    {
    }
}

void VideoMapperApp::mouseDrag(MouseEvent event)
{
    if (!Warp::handleMouseDrag(warps, event))
    {
    }
}

void VideoMapperApp::mouseUp(MouseEvent event)
{
    if (!Warp::handleMouseUp(warps, event))
    {
    }
}

void VideoMapperApp::fileDrop(FileDropEvent event)
{
    bool loadingSuccess = videoHandler.load(event.getFile(0));
    if (!loadingSuccess)
    {
        cout << "Error: failed to load dropped video at: " << event.getFile(0) << endl;
    }
}

void VideoMapperApp::selectAndLoadNewVideo()
{
    const fs::path videoFilePath = getOpenFilePath();
    if (!videoFilePath.empty())
    {
        bool loadingSuccess = videoHandler.load(videoFilePath);
        if (!loadingSuccess)
        {
            cout << "Error: failed to load video selected at: " << videoFilePath << endl;
        }
    }
}

void VideoMapperApp::update()
{
    videoHandler.update();
}

void VideoMapperApp::draw()
{
    gl::clear(Color(0, 0, 0));
    
    for (const WarpRef& warp : warps)
    {
        warp->begin();
        videoHandler.draw();
        warp->end();
    }
}

CINDER_APP(VideoMapperApp, RendererGl)
