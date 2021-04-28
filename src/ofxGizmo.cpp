//
//  ofxGizmo.cpp
//  TerrainTest
//
//  Created by Nick Hardeman on 10/5/14.
//

#include "ofxGizmo.h"
#include "glm/gtx/matrix_decompose.hpp"

//--------------------------------------------------------------
ofxGizmo::ofxGizmo() {
    // setup the gizmo //
    gizmoMove   = CreateMoveGizmo();
    gizmoRotate = CreateRotateGizmo();
    gizmoScale  = CreateScaleGizmo();
    
    setType( OFX_GIZMO_MOVE );
    setViewDimensions( ofGetWidth(), ofGetHeight() );
    setDisplayScale( 1. );
    
//    enableMouseInput();
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
void ofxGizmo::setMatrix( glm::mat4 aMat ) {
    objectMatrix = aMat;
    auto vptr = glm::value_ptr( objectMatrix );
    
    gizmoRotate->SetEditMatrix( vptr );
    gizmoMove->SetEditMatrix( vptr );
    gizmoScale->SetEditMatrix( vptr );
    
//    gizmoRotate->SetEditMatrix( objectMatrix.getPtr() );
//    gizmoMove->SetEditMatrix( objectMatrix.getPtr() );
//    gizmoScale->SetEditMatrix( objectMatrix.getPtr() );
    
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
//        objectMatrix.set( vals );
        objectMatrix = glm::make_mat4(vals);
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
    
    if( mEvents == nullptr ) {
        setEvents(ofEvents());
        enableMouseInput();
    }
    
    if ( gizmo && isVisible() ) {
        ofPushStyle(); {
		
            ofMatrix4x4 mvm = aCam.getModelViewMatrix();
            ofMatrix4x4 pm = aCam.getProjectionMatrix();
            const float *mvMat = mvm.getPtr();
            const float *proMat = pm.getPtr();
		
            gizmoRotate->SetCameraMatrix( mvMat, proMat );
            gizmoMove->SetCameraMatrix( mvMat, proMat );
            gizmoScale->SetCameraMatrix( mvMat, proMat );
            
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
glm::mat4& ofxGizmo::getMatrix() {
    return objectMatrix;
}

//--------------------------------------------------------------
void ofxGizmo::apply( ofNode& anode ) {
    anode.setGlobalPosition(getTranslation());
    anode.setGlobalOrientation( getRotation() );
    anode.setScale( getScale() );
}

//--------------------------------------------------------------
glm::vec3 ofxGizmo::getTranslation() {
    if( gizmoMove != NULL ) {
        return glm::vec3(objectMatrix[3]);
//        return objectMatrix.getTranslation();
    }
    return glm::vec3();
}

//--------------------------------------------------------------
glm::quat ofxGizmo::getRotation() {
    if( gizmoRotate != NULL ) {
        return glm::quat( objectMatrix );
//        return objectMatrix.getRotate();
    }
    return glm::quat();
}

//--------------------------------------------------------------
glm::vec3 ofxGizmo::getScale() {
    if( gizmoScale != NULL ) {
//        return objectMatrix.getScale();
        // there's gotta be a better way to do this
        glm::vec3 scale;
        glm::quat orientation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(objectMatrix, scale, orientation, translation, skew, perspective);
        return scale;
    }
    return glm::vec3(1,1,1);
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
//    string outStr;
//    for( int i = 0; i < 4; i++ ) {
//        ofVec4f v = objectMatrix.getRowAsVec4f(i);
//        outStr += ofToString(v.x)+",";
//        outStr += ofToString(v.y)+",";
//        outStr += ofToString(v.z)+",";
//        outStr += ofToString(v.w);
//        if( i != 3 ) {
//            outStr += ",";
//        }
//    }
    stringstream ss;
    auto vptr = glm::value_ptr( objectMatrix );
    for( int i = 0; i < 16; i++ ) {
//        ss << objectMatrix.getPtr()[i];
        ss << ofToString(vptr[i], 12);
        if( i < 15 ) {
            ss << ",";
        }
    }
    return ss.str();
}

//--------------------------------------------------------------
bool ofxGizmo::save( string aFileName ) {
    ofBuffer tbuff;
    tbuff.append( getMatrixAsString() );
    return ofBufferToFile( aFileName, tbuff );
}

//--------------------------------------------------------------
bool ofxGizmo::saveTo( ofXml& axml, string aParamName ) {
    if( mMatStringParam.getName().length() < 1 ) {
        if( aParamName != "" ) {
            mMatStringParam.setName( aParamName );
        } else {
            mMatStringParam.setName( "Gizmo" );
        }
    }
    mMatStringParam = getMatrixAsString();
    ofSerialize( axml, mMatStringParam );
}

//--------------------------------------------------------------
bool ofxGizmo::load( string aFileName ) {
    ofBuffer tbuff = ofBufferFromFile( aFileName );
    string tstr = tbuff.getText();
    return setMatrix( tstr );
//    vector< string > tstrings = ofSplitString( tstr, "," );
//    if( tstrings.size() == 16 ) {
//        float vals[16];
//        for( int i = 0; i < tstrings.size(); i++ ) {
//            vals[i] = ( ofToFloat(tstrings[i] ));
//        }
////        objectMatrix.set( vals );
//        objectMatrix = glm::make_mat4(vals);
//        setMatrix( objectMatrix );
//        return true;
//    }
//    return false;
}

//--------------------------------------------------------------
void ofxGizmo::setEvents(ofCoreEvents& aEvents) {
    bool wasMouseInputEnabled = _bHasMouseEvents;// || !events;
    disableMouseInput();
    mEvents = &aEvents;
    if (wasMouseInputEnabled) {
        enableMouseInput();
    }
}

//--------------------------------------------------------------
void ofxGizmo::enableMouseInput() {
    if( !_bHasMouseEvents && mEvents ) {
        ofAddListener( mEvents->mouseMoved, this, &ofxGizmo::mouseMoved, OF_EVENT_ORDER_BEFORE_APP );
        ofAddListener( mEvents->mouseDragged, this, &ofxGizmo::mouseDragged, OF_EVENT_ORDER_BEFORE_APP );
        ofAddListener( mEvents->mousePressed, this, &ofxGizmo::mousePressed, OF_EVENT_ORDER_BEFORE_APP );
        ofAddListener( mEvents->mouseReleased, this, &ofxGizmo::mouseReleased, OF_EVENT_ORDER_BEFORE_APP );
    }
    _bHasMouseEvents = true;
}

//--------------------------------------------------------------
void ofxGizmo::disableMouseInput() {
    if( _bHasMouseEvents && mEvents ) {
        ofRemoveListener( mEvents->mouseMoved, this, &ofxGizmo::mouseMoved, OF_EVENT_ORDER_BEFORE_APP );
        ofRemoveListener( mEvents->mouseDragged, this, &ofxGizmo::mouseDragged, OF_EVENT_ORDER_BEFORE_APP );
        ofRemoveListener( mEvents->mousePressed, this, &ofxGizmo::mousePressed, OF_EVENT_ORDER_BEFORE_APP );
        ofRemoveListener( mEvents->mouseReleased, this, &ofxGizmo::mouseReleased, OF_EVENT_ORDER_BEFORE_APP );
    }
    _bInteracting       = false;
    _bHasMouseEvents    = false;
}

//--------------------------------------------------------------
bool ofxGizmo::mouseMoved( ofMouseEventArgs& args ) {
    return mouseMoved( args.x, args.y );
}

//--------------------------------------------------------------
bool ofxGizmo::mouseDragged( ofMouseEventArgs& args ) {
    return mouseDragged( args.x, args.y, args.button );
}

//--------------------------------------------------------------
bool ofxGizmo::mousePressed( ofMouseEventArgs& args ) {
    return mousePressed( args.x, args.y, args.button );
}

//--------------------------------------------------------------
bool ofxGizmo::mouseReleased( ofMouseEventArgs& args ) {
    return mouseReleased( args.x, args.y, args.button );
}

//--------------------------------------------------------------
bool ofxGizmo::mouseMoved(int x, int y) {
    if (gizmo && isVisible()) gizmo->OnMouseMove( x, y );
    return _bInteracting;
}

//--------------------------------------------------------------
bool ofxGizmo::mouseDragged(int x, int y, int button) {
    if (gizmo && isVisible()) gizmo->OnMouseMove( x, y );
    return _bInteracting;
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
bool ofxGizmo::mouseReleased(int x, int y, int button) {
    if (gizmo && isVisible()) gizmo->OnMouseUp( x, y );
    bool bWasInteracting = _bInteracting;
    _bInteracting = false;
    return bWasInteracting;
}









