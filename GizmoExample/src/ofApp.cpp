#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate( 60 );
    
    vector< ofColor > tcolors;
    tcolors.push_back( ofColor(243,226,149) );
    tcolors.push_back( ofColor(120,208,204) );
    tcolors.push_back( ofColor(119,119,119) );
    
    ofVec3f tpos(1,0,0);
    for( int i = 0; i < 10; i++ ) {
        tpos.rotate( ofRandom(0,360), ofVec3f(1,0,0 ));
        tpos.rotate( ofRandom(0,360), ofVec3f(0,1,0 ));
        tpos.rotate( ofRandom(0,360), ofVec3f(0,0,1 ));
        
        boxes.push_back( ofBoxPrimitive() );
        boxes.back().set( 3 );
        boxes.back().setPosition( tpos * ofRandom(-5, 5));
        vector< ofFloatColor >& colors = boxes.back().getMesh().getColors();
        colors.resize( boxes.back().getMesh().getVertices().size() );
        ofColor tcolor = tcolors[ofClamp( ofRandom(0, tcolors.size() ), 0, tcolors.size()-1)];
        for( int j = 0; j < colors.size(); j++ ) {
            colors[j] = tcolor;
        }
    }
	
    camera.disableMouseInput();
    camera.setDistance( 14 );
    camera.setPosition( 0, 1, -14 );
    camera.lookAt( ofVec3f(0,0,0) );
    
    gizmo.setDisplayScale(2.);
    
    for( int i = 0; i < boxes.size(); i++ ) {
        if(gizmo.load("box-"+ofToString(i,0)+".txt" )) {
            boxes[i].setTransformMatrix( gizmo.getMatrix() );
        }
    }
    
    boxIndex = 0;
    if( boxes.size() ) {
        gizmo.setNode( boxes[ boxIndex ] );
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    if( boxIndex > -1 ) {
        boxes[ boxIndex ].setTransformMatrix( gizmo.getMatrix() );
    }
    light.setPosition( camera.getPosition() );
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    ofBackgroundGradient( ofColor(238,238,238), ofColor(255, 255, 255) );
    
    ofEnableDepthTest();
    camera.begin();
    
    ofEnableLighting();
    light.enable();
    for( int i = 0; i < boxes.size(); i++ ) {
        boxes[i].draw();
    }
    light.disable();
	ofDisableLighting();

	// gizmo draw() disables depth test and lighting //
    gizmo.draw( camera );
    camera.end();
    ofDisableDepthTest();
    
    stringstream ss;
    ss << "Enable camera input (spacebar) " << endl;
    ss << "Enable Gizmo (e): " << gizmo.isVisible() << endl;
    ss << "Gizmo Rotate (r) " << endl;
    ss << "Gizmo Translate (g) " << endl;
    ss << "Gizmo Scale (s) " << endl;
    ss << "Next (right arrow) " << endl;
    ss << "Previous (left arrow) " << endl;
    
    ofDrawBitmapStringHighlight( ss.str(), 20, 20 );
    
}

//--------------------------------------------------------------
void ofApp::exit() {
    for( int i = 0; i < boxes.size(); i++ ) {
        gizmo.setNode( boxes[ i ] );
        gizmo.save("box-"+ofToString(i,0)+".txt" );
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == ' ' ) {
        camera.enableMouseInput();
    }
    if( key == 'r' ) {
        gizmo.setType( ofxGizmo::OFX_GIZMO_ROTATE );
    }
    if( key == 'g' ) {
        gizmo.setType( ofxGizmo::OFX_GIZMO_MOVE );
    }
    if( key == 's' ) {
        gizmo.setType( ofxGizmo::OFX_GIZMO_SCALE );
    }
    if( key == 'e' ) {
        gizmo.toggleVisible();
    }
    if( key == OF_KEY_RIGHT ) {
        boxIndex++;
        if( boxIndex >= boxes.size() ) {
            boxIndex = 0;
        }
        gizmo.setNode( boxes[ boxIndex ] );
    }
    if( key == OF_KEY_LEFT ) {
        boxIndex--;
        if( boxIndex < 0 ) {
            boxIndex = (int)boxes.size()-1;
        }
        gizmo.setNode( boxes[ boxIndex ] );
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    camera.disableMouseInput();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}