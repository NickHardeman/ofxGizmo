//
//  ofxGizmo.cpp
//  TerrainTest
//
//  Created by Nick Hardeman on 10/5/14.
//

#include "ofxGizmo.h"

//--------------------------------------------------------------
ofxGizmo::ofxGizmo() {
    // setup the gizmo //
    gizmoMove   = CreateMoveGizmo();
    gizmoRotate = CreateRotateGizmo();
    gizmoScale  = CreateScaleGizmo();
    
    setType( OFX_GIZMO_MOVE );
    setViewDimensions( ofGetWidth(), ofGetHeight() );
    setDisplayScale( 1. );
    
    enableMouseInput();
    show();
    _bInteracting = false;
    bNodeSet = false;
    
    ofMatrix4x4 tmat;
    tmat.makeIdentityMatrix();
    setMatrix( tmat );
    
}

//--------------------------------------------------------------
ofxGizmo::~ofxGizmo() {
    disableMouseInput();
//    IGizmo *gizmo       = NULL; don't delete, since it points to one of the following //
    if( gizmoMove != NULL ) {
        delete gizmoMove;
        gizmoMove = NULL;
    }
    
    if( gizmoRotate != NULL ) {
        delete gizmoRotate;
        gizmoRotate = NULL;
    }
    
    if( gizmoScale != NULL ) {
        delete gizmoScale;
        gizmoScale = NULL;
    }
}

//--------------------------------------------------------------
void ofxGizmo::setMatrix( ofMatrix4x4 aMat ) {
    objectMatrix = aMat;
    
    gizmoRotate->SetEditMatrix( objectMatrix.getPtr() );
    gizmoMove->SetEditMatrix( objectMatrix.getPtr() );
    gizmoScale->SetEditMatrix( objectMatrix.getPtr() );
    
    gizmoRotate->SetScreenDimension( _windowW, _windowH );
    gizmoMove->SetScreenDimension( _windowW, _windowH );
    gizmoScale->SetScreenDimension( _windowW, _windowH );
    
    bNodeSet = true;
}

//--------------------------------------------------------------
bool ofxGizmo::setMatrix( string aString ) {
    vector< string > tstrings = ofSplitString( aString, "," );
    if( tstrings.size() == 16 ) {
        float vals[16];
        for( int i = 0; i < tstrings.size(); i++ ) {
            vals[i] = ( ofToFloat(tstrings[i] ));
        }
        objectMatrix.set( vals );
        setMatrix( objectMatrix );
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void ofxGizmo::setNode( ofNode aNode ) {
    setMatrix( aNode.getGlobalTransformMatrix() );
}

//--------------------------------------------------------------
void ofxGizmo::draw( ofCamera &aCam ) {
    
    if ( gizmo && isVisible() ) {
        ofPushStyle(); {
            gizmoRotate->SetCameraMatrix( aCam.getModelViewMatrix().getPtr(), aCam.getProjectionMatrix().getPtr() );
            gizmoMove->SetCameraMatrix( aCam.getModelViewMatrix().getPtr(), aCam.getProjectionMatrix().getPtr() );
            gizmoScale->SetCameraMatrix( aCam.getModelViewMatrix().getPtr(), aCam.getProjectionMatrix().getPtr() );
            
            if(bNodeSet) {
				ofDisableDepthTest();
				ofDisableLighting();
                gizmo->Draw();
            }
        } ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofxGizmo::setDisplayScale( float aScale ) {
    gizmoRotate->SetDisplayScale( aScale );
    gizmoMove->SetDisplayScale( aScale );
    gizmoScale->SetDisplayScale( aScale );
}

//--------------------------------------------------------------
void ofxGizmo::setType( ofxGizmoType aType ) {
    if( aType == OFX_GIZMO_ROTATE ) {
        gizmo = gizmoRotate;
        gizmo->SetLocation( IGizmo::LOCATE_LOCAL );
    }
    if( aType == OFX_GIZMO_MOVE ) {
        gizmo = gizmoMove;
        gizmo->SetLocation( IGizmo::LOCATE_WORLD );
    }
    if( aType == OFX_GIZMO_SCALE ) {
        gizmo = gizmoScale;
        gizmo->SetLocation( IGizmo::LOCATE_WORLD );
    }
    
    setViewDimensions( _windowW, _windowH );
}

//--------------------------------------------------------------
ofMatrix4x4& ofxGizmo::getMatrix() {
    return objectMatrix;
}

//--------------------------------------------------------------
void ofxGizmo::hide() {
    _bVisible = false;
    _bInteracting = false;
}

//--------------------------------------------------------------
void ofxGizmo::show() {
    _bVisible = true;
}

//--------------------------------------------------------------
void ofxGizmo::toggleVisible() {
    if( isVisible() ) hide();
    else show();
}

//--------------------------------------------------------------
bool ofxGizmo::isVisible() {
    return _bVisible;
}

//--------------------------------------------------------------
bool ofxGizmo::isInteracting() {
    return _bInteracting;
}

//--------------------------------------------------------------
void ofxGizmo::setViewDimensions( float aw, float ah ) {
    _windowW = aw;
    _windowH = ah;
    
    if( gizmo ) gizmo->SetScreenDimension( _windowW, _windowH );
}

//--------------------------------------------------------------
string ofxGizmo::getMatrixAsString() {
    string outStr;
    for( int i = 0; i < 4; i++ ) {
        ofVec4f v = objectMatrix.getRowAsVec4f(i);
        outStr += ofToString(v.x)+",";
        outStr += ofToString(v.y)+",";
        outStr += ofToString(v.z)+",";
        outStr += ofToString(v.w);
        if( i != 3 ) {
            outStr += ",";
        }
    }
    return outStr;
}

//--------------------------------------------------------------
bool ofxGizmo::save( string aFileName ) {
    ofBuffer tbuff;
    tbuff.append( getMatrixAsString() );
    return ofBufferToFile( aFileName, tbuff );
}

//--------------------------------------------------------------
bool ofxGizmo::load( string aFileName ) {
    ofBuffer tbuff = ofBufferFromFile( aFileName );
    string tstr = tbuff.getText();
    vector< string > tstrings = ofSplitString( tstr, "," );
    if( tstrings.size() == 16 ) {
        float vals[16];
        for( int i = 0; i < tstrings.size(); i++ ) {
            vals[i] = ( ofToFloat(tstrings[i] ));
        }
        objectMatrix.set( vals );
        setMatrix( objectMatrix );
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void ofxGizmo::enableMouseInput() {
    if( !_bHasMouseEvents ) {
        ofAddListener( ofEvents().mouseMoved, this, &ofxGizmo::mouseMoved );
        ofAddListener( ofEvents().mouseDragged, this, &ofxGizmo::mouseDragged );
        ofAddListener( ofEvents().mousePressed, this, &ofxGizmo::mousePressed );
        ofAddListener( ofEvents().mouseReleased, this, &ofxGizmo::mouseReleased );
    }
    _bHasMouseEvents = true;
}

//--------------------------------------------------------------
void ofxGizmo::disableMouseInput() {
    if( _bHasMouseEvents ) {
        ofRemoveListener( ofEvents().mouseMoved, this, &ofxGizmo::mouseMoved );
        ofRemoveListener( ofEvents().mouseDragged, this, &ofxGizmo::mouseDragged );
        ofRemoveListener( ofEvents().mousePressed, this, &ofxGizmo::mousePressed );
        ofRemoveListener( ofEvents().mouseReleased, this, &ofxGizmo::mouseReleased );
    }
    _bInteracting       = false;
    _bHasMouseEvents    = false;
}

//--------------------------------------------------------------
void ofxGizmo::mouseMoved( ofMouseEventArgs& args ) {
    mouseMoved( args.x, args.y );
}

//--------------------------------------------------------------
void ofxGizmo::mouseDragged( ofMouseEventArgs& args ) {
    mouseDragged( args.x, args.y, args.button );
}

//--------------------------------------------------------------
void ofxGizmo::mousePressed( ofMouseEventArgs& args ) {
    mousePressed( args.x, args.y, args.button );
}

//--------------------------------------------------------------
void ofxGizmo::mouseReleased( ofMouseEventArgs& args ) {
    mouseReleased( args.x, args.y, args.button );
}

//--------------------------------------------------------------
void ofxGizmo::mouseMoved(int x, int y) {
    if (gizmo && isVisible()) gizmo->OnMouseMove( x, y );
}

//--------------------------------------------------------------
void ofxGizmo::mouseDragged(int x, int y, int button) {
    if (gizmo && isVisible()) gizmo->OnMouseMove( x, y );
}

//--------------------------------------------------------------
bool ofxGizmo::mousePressed(int x, int y, int button) {
    if (gizmo && isVisible()) {
        if (gizmo->OnMouseDown( x, y )) {
            _bInteracting = true;
            return true;
        }
    }
    _bInteracting = false;
    return false;
}

//--------------------------------------------------------------
void ofxGizmo::mouseReleased(int x, int y, int button) {
    if (gizmo && isVisible()) gizmo->OnMouseUp( x, y );
    _bInteracting = false;
}









