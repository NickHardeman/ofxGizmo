#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    ofGLFWWindowSettings settings;
    settings.setSize(1200, 700 );
    settings.setGLVersion( 3, 2 );
    settings.title = "ofxGizmo";
    auto mainWin = ofCreateWindow( settings );
    
    auto app = make_shared<ofApp>();    
    ofRunApp( mainWin, app );
    ofRunMainLoop();

//    ofSetupOpenGL(1024,768, OF_WINDOW);            // <-------- setup the GL context
//
//    // this kicks off the running of my app
//    // can be OF_WINDOW or OF_FULLSCREEN
//    // pass in width and height too:
//    ofRunApp( new ofApp());

}
