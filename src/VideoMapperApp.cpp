#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CinderImGui.h"

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
    void handleResize();

    void update() override;
    void draw() override;
    
    void selectAndLoadNewVideo();
    
    VideoHandler videoHandler = {};
    
    fs::path settingsFilePath;
    WarpList warps;
    
    WindowRef projectionWindow = {};
    WindowRef settingsWindow = {};
    
    bool settingsIsOpen = true;
};

void prepareSettings(VideoMapperApp::Settings *settings)
{
    settings->setWindowSize(ivec2(450, 200));
    settings->setHighDensityDisplayEnabled(true);
    settings->setMultiTouchEnabled(false);
    settings->setPowerManagementEnabled(false);
    settings->disableFrameRate();
    settings->setTitle("Video Mapper - Settings");
}

void VideoMapperApp::setup()
{
    ImGui::Initialize();

    warps.push_back(WarpPerspective::create());
    
    settingsWindow = getWindow();
    settingsWindow->getSignalClose().connect([this] { quit(); });
    settingsWindow->setAlwaysOnTop();
    
    projectionWindow = createWindow(Window::Format().size(800, 500));
    projectionWindow->getSignalClose().connect([this] { quit(); });
    projectionWindow->setTitle("Video Mapper - Projection");
}


void VideoMapperApp::cleanup()
{
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
                projectionWindow->setFullScreen(!projectionWindow->isFullScreen());
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
    if (getWindow() == projectionWindow)
    {
        handleResize();
    }
}

void VideoMapperApp::handleResize()
{
    videoHandler.setWindowSize(projectionWindow->getSize());
    videoHandler.handleResize();
    if (videoHandler.isLoaded())
    {
        warps[0]->setSize(videoHandler.getVideoRenderSize());
    }
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
    handleResize();
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
    handleResize();
}

void VideoMapperApp::update()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(settingsWindow->getSize() * ivec2(settingsWindow->getContentScale()));
    ImGui::Begin("Settings", &settingsIsOpen, ImGuiWindowFlags_NoTitleBar);
    ImGui::SetWindowFontScale(2.0f);
    if (ImGui::Button("open (O)"))
    {
        selectAndLoadNewVideo();
    }
    ImGui::NewLine();
    if (ImGui::Checkbox("looping", &videoHandler.looping))
    {
        videoHandler.loopingUpdated();
    }
    if (ImGui::Checkbox("audio", &videoHandler.soundOn))
    {
        videoHandler.soundOnUpdated();
    }
    
    ImGui::NewLine();
    if (ImGui::Button("toggle edit mode (W)"))
    {
        Warp::enableEditMode(!Warp::isEditModeEnabled());
    }
    if (ImGui::DragFloat2("scale x/y", &videoHandler.videoScale[0], /*speed*/ 0.005f, /*min*/ 1.0f, /*max*/ 4.0f))
    {
        videoHandler.videoScaleUpdated();
        resize();
    }
    if (ImGui::DragFloat2("offset x/y", &videoHandler.videoOffset[0], /*speed*/ 0.0025f, /*min*/ 0.0f, /*max*/ 1.0f))
    {
        videoHandler.videoScaleUpdated();
        resize();
    }
    if (ImGui::Button("toggle fullscreen (F)"))
    {
        projectionWindow->setFullScreen(!projectionWindow->isFullScreen());
    }
    
    ImGui::End();
}

void VideoMapperApp::draw()
{
    if (getWindow() == projectionWindow)
    {
        videoHandler.update();
        
        gl::clear(Color(0, 0, 0));
        for (const WarpRef& warp : warps)
        {
            warp->begin();
            videoHandler.draw();
            warp->end();
        }
    }
}

CINDER_APP(VideoMapperApp, RendererGl, prepareSettings)
