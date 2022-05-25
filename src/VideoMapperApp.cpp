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
    
    // load movie
    selectAndLoadNewVideo();
    
    warps[0]->setSize(videoHandler.getVideoRenderSize());
}

void VideoMapperApp::cleanup()
{
    Warp::writeSettings(warps, writeFile(settingsFilePath));
}

void VideoMapperApp::keyDown(KeyEvent event)
{
    if (event.getChar() == 'o')
    {
        selectAndLoadNewVideo();
    }
    
    if (!Warp::handleKeyDown(warps, event))
    {
        switch (event.getCode())
        {
            case KeyEvent::KEY_ESCAPE:
                quit();
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
    const fs::path moviePath = getOpenFilePath();
    if (!moviePath.empty())
    {
        bool loadingSuccess = videoHandler.load(moviePath);
        if (!loadingSuccess)
        {
            cout << "Error: failed to load video selected at: " << moviePath << endl;
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
