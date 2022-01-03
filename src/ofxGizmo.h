//
//  ofxGizmo.h
//  TerrainTest
//
//  Created by Nick Hardeman on 10/5/14.
//

#pragma once
#include "ofMain.h"
#include "IGizmo.h"

class ofxGizmo {
public:
    ofxGizmo();
    ~ofxGizmo();
    
    enum ofxGizmoType {
        OFX_GIZMO_MOVE = 0,
        OFX_GIZMO_SCALE,
        OFX_GIZMO_ROTATE
    };
    
    void setMatrix( glm::mat4 aMat );
    bool setMatrix( string aString );
    void setNode( ofNode aNode );
    
    void draw( ofCamera& aCam );
    
    void setDisplayScale( float aScale );
    void setType( ofxGizmoType aType );
//    ofMatrix4x4& getMatrix();
    glm::mat4& getMatrix();
    void apply( ofNode& anode );
    glm::vec3 getTranslation();
    glm::quat getRotation();
    glm::vec3 getScale();
    
    void setTranslationAxisMask( unsigned int amask );
    void setRotationAxisMask( unsigned int amask );
    void setScaleAxisMask( unsigned int amask );
    
    void hide();
    void show();
    void toggleVisible();
    bool isVisible();
    
    // is the gizmo using the mouse?
    bool isInteracting();
    
    void setViewDimensions( float aw, float ah );
    
    string getMatrixAsString();
    bool save( string aFileName );
    void saveTo( ofXml& axml, string aParamName="" );
    // if you want to rename the node or what not //
    ofParameter<string>& getSaveParam() { return mMatStringParam; }
    bool load( string aFileName );
    void loadFrom( ofXml& axml, string aParamName="" );
    
    void setEvents(ofCoreEvents& aEvents);
    void enableMouseInput();
    void disableMouseInput();
    
    bool mouseMoved( ofMouseEventArgs& args );
    bool mouseDragged( ofMouseEventArgs& args );
    bool mousePressed( ofMouseEventArgs& args );
    bool mouseReleased( ofMouseEventArgs& args );
    
    bool mouseMoved(int x, int y);
    bool mouseDragged(int x, int y, int button);
    bool mousePressed(int x, int y, int button);
    bool mouseReleased(int x, int y, int button);
    
    bool isHovering() { return gizmo->isHovering(); }
    
protected:
    bool _bHasMouseEvents = false;
    bool _bHasKeyEvents = false;
    bool _bInteracting = false;
    bool _bVisible = true;
    
    float _windowW = 0, _windowH = 0;
    bool bNodeSet = false;
    
//    ofMatrix4x4 objectMatrix;
    glm::mat4 objectMatrix;
    IGizmo *gizmo       = NULL;
    IGizmo *gizmoMove   = NULL;
    IGizmo *gizmoRotate = NULL;
    IGizmo *gizmoScale  = NULL;
    
    ofParameter<string> mMatStringParam;
    ofCoreEvents * mEvents = nullptr;
};
