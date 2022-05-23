#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VideoMapperApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void VideoMapperApp::setup()
{
}

void VideoMapperApp::mouseDown( MouseEvent event )
{
}

void VideoMapperApp::update()
{
}

void VideoMapperApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( VideoMapperApp, RendererGl )
