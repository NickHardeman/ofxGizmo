///////////////////////////////////////////////////////////////////////////////////////////////////
// LibGizmo
// File Name : 
// Creation : 10/01/2012
// Author : Cedric Guillemet
// Description : LibGizmo
//
///Copyright (C) 2012 Cedric Guillemet
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
//of the Software, and to permit persons to whom the Software is furnished to do
///so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 



//#include "stdafx.h"
#include "GizmoTransformRender.h"
//#ifdef MAC_OS
//#import <OpenGL/OpenGL.h>
//#else
//#include <GL/gl.h>
//#endif

void CGizmoTransformRender::DrawCircle(const tvector3 &orig,float r,float g,float b,const tvector3 &vtx,const tvector3 &vty)
{
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);
    ofFloatColor tcolor(r,g,b,1);
    
    ofMesh tmesh;
    tmesh.setMode( OF_PRIMITIVE_LINE_LOOP );
//	glBegin(GL_LINE_LOOP);
    ofVec3f torig(orig.x, orig.y, orig.z );
    ofVec3f tvtx(vtx.x, vtx.y, vtx.z );
    ofVec3f tvty(vty.x, vty.y, vty.z );
    
	for (int i = 0; i < 50 ; i++)
	{
		ofVec3f vt;
		vt = tvtx * cos((2*ZPI/50)*i);
		vt += tvty * sin((2*ZPI/50)*i);
		vt += torig;
//		glVertex3f(vt.x,vt.y,vt.z);
        tmesh.addVertex( vt );
	}
    
    ofSetColor( tcolor );
    tmesh.draw();
//	glEnd();
}


void CGizmoTransformRender::DrawCircleHalf(const tvector3 &orig,float r,float g,float b,const tvector3 &vtx,const tvector3 &vty,tplane &camPlan)
{
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);
	ofFloatColor tcolor(r,g,b,1);

//	glBegin(GL_LINE_STRIP);
    ofMesh tmesh;
    tmesh.setMode( OF_PRIMITIVE_LINE_STRIP );
    //	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 30; i++)
	{
		tvector3 vt;
		vt = vtx * cos((ZPI/30)*i);
		vt += vty * sin((ZPI/30)*i);
		vt +=orig;
		if (camPlan.DotNormal(vt)) {
//			glVertex3f(vt.x,vt.y,vt.z);
            tmesh.addVertex( ofVec3f(vt.x,vt.y,vt.z) );
        }
	}
    ofSetColor( tcolor );
    tmesh.draw();
//	glEnd();
}

void CGizmoTransformRender::DrawAxis(const tvector3 &orig, const tvector3 &axis, const tvector3 &vtx,const tvector3 &vty, float fct,float fct2,const tvector4 &col)
{
    ofFloatColor tcolor(col.x, col.y, col.z, col.w);
    ofSetColor( tcolor );
    glm::vec3 start( orig.x, orig.y, orig.z );
    glm::vec3 end( orig.x+axis.x,orig.y+axis.y,orig.z+axis.z );
    
    
    ofMatrix4x4 tmat;
    tmat.makeRotationMatrix(ofVec3f(0,-1,0), ofVec3f(axis.x, axis.y, axis.z ) );
    
    ofDrawLine( start, end );
    ofPushMatrix(); {
        ofTranslate( end );
        ofMultMatrix(tmat);
        ofDrawCone( glm::vec3(), fct2, fct2 * 3.13 );
    } ofPopMatrix();
    
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);
//	glColor4fv(&col.x);
//	glBegin(GL_LINES);
//	glVertex3fv(&orig.x);
//	glVertex3f(orig.x+axis.x,orig.y+axis.y,orig.z+axis.z);
//	glEnd();
//
//	glBegin(GL_TRIANGLE_FAN);
//	for (int i=0;i<=30;i++)
//	{
//		tvector3 pt;
//		pt = vtx * cos(((2*ZPI)/30.0f)*i)*fct;
//		pt+= vty * sin(((2*ZPI)/30.0f)*i)*fct;
//		pt+=axis*fct2;
//		pt+=orig;
//		glVertex3fv(&pt.x);
//		pt = vtx * cos(((2*ZPI)/30.0f)*(i+1))*fct;
//		pt+= vty * sin(((2*ZPI)/30.0f)*(i+1))*fct;
//		pt+=axis*fct2;
//		pt+=orig;
//		glVertex3fv(&pt.x);
//		glVertex3f(orig.x+axis.x,orig.y+axis.y,orig.z+axis.z);
//
//	}
//	glEnd();

}

void CGizmoTransformRender::DrawCamem(const tvector3& orig,const tvector3& vtx,const tvector3& vty,float ng)
{
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);
	int i = 0 ;
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ofEnableAlphaBlending();

	glDisable(GL_CULL_FACE);

    ofMesh tmesh;
    tmesh.setMode( OF_PRIMITIVE_TRIANGLE_FAN );
//	glColor4f(1,1,0,0.5f);
    ofFloatColor tcolor(1,1,0,0.5f);
//	glBegin(GL_TRIANGLE_FAN);
//	glVertex3fv(&orig.x);
    ofVec3f torig(orig.x, orig.y, orig.z);
    ofVec3f tvtx(vtx.x, vtx.y, vtx.z );
    ofVec3f tvty(vty.x, vty.y, vty.z );
    
    tmesh.addVertex( torig );
	for (i = 0 ; i <= 50 ; i++) {
		ofVec3f vt;
		vt = tvtx * cos(((ng)/50)*i);
		vt += tvty * sin(((ng)/50)*i);
		vt += torig;
//		glVertex3f(vt.x,vt.y,vt.z);
        tmesh.addVertex( vt );
	}
//	glEnd();
    ofSetColor( tcolor );
    tmesh.draw();

//	glDisable(GL_BLEND);
    ofDisableAlphaBlending();


	tcolor.set(1,1,0.2f,1);
    tmesh.clear();
    tmesh.setMode( OF_PRIMITIVE_LINE_LOOP );
//	tvector3 vt;
//	glBegin(GL_LINE_LOOP);

//	glVertex3fv(&orig.x);
    tmesh.addVertex( torig );
	for ( i = 0 ; i <= 50; i++) {
		ofVec3f vt;
		vt = tvtx * cos(((ng)/50)*i);
		vt += tvty * sin(((ng)/50)*i);
		vt += torig;
        tmesh.addVertex( vt );
//		glVertex3f(vt.x,vt.y,vt.z);
	}
    
    ofSetColor( tcolor );
    tmesh.draw();
//	glEnd();
}

void CGizmoTransformRender::DrawQuad(const tvector3& orig, float size, bool bSelected, const tvector3& axisU, const tvector3 &axisV)
{
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    ofEnableAlphaBlending();

//    glDisable(GL_CULL_FACE);

	ofVec3f pts[4];
	pts[0].set(orig.x, orig.y, orig.z );
	pts[1] = pts[0] + (ofVec3f(axisU.x, axisU.y, axisU.z) * size);
	pts[2] = pts[0] + (ofVec3f(axisU.x, axisU.y, axisU.z) + ofVec3f(axisV.x, axisV.y, axisV.z))*size;
	pts[3] = pts[0] + (ofVec3f(axisV.x, axisV.y, axisV.z) * size);
    
    ofFloatColor tcolor(1,1,1,0.6f);
	if (!bSelected)
		tcolor.set(1,1,0,0.5f);
//	else
//		glColor4f(1,1,1,0.6f);
    ofSetColor( tcolor );
    ofMesh tmesh;
    tmesh.setMode( OF_PRIMITIVE_TRIANGLES );
    tmesh.addVertex( pts[0] );
    tmesh.addVertex( pts[1] );
    tmesh.addVertex( pts[2] );
    
    tmesh.addVertex( pts[0] );
    tmesh.addVertex( pts[2] );
    tmesh.addVertex( pts[3] );
//	glBegin(GL_QUADS);
//	glVertex3fv(&pts[0].x);
//	glVertex3fv(&pts[1].x);
//	glVertex3fv(&pts[2].x);
//	glVertex3fv(&pts[3].x);
//	glEnd();
    tmesh.draw();
    
    tmesh.clear();
    tmesh.setMode( OF_PRIMITIVE_LINE_STRIP );
	if (!bSelected)
		tcolor.set(1,1,0.2f,1);
	else
		tcolor.set(1,1,1,0.6f);
    ofSetColor( tcolor );
    tmesh.addVertex( pts[0] );
    tmesh.addVertex( pts[1] );
    tmesh.addVertex( pts[2] );
    tmesh.addVertex( pts[3] );
    tmesh.addVertex( pts[0] );
//    
//	glBegin(GL_LINE_STRIP);
//	glVertex3fv(&pts[0].x);
//	glVertex3fv(&pts[1].x);
//	glVertex3fv(&pts[2].x);
//	glVertex3fv(&pts[3].x);
//	glEnd();
//
//	glDisable(GL_BLEND);
    ofDisableAlphaBlending();
}


void CGizmoTransformRender::DrawTri(const tvector3& orig, float size, bool bSelected, const tvector3& axisU, const tvector3& axisV)
{
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ofEnableAlphaBlending();

//    glDisable(GL_CULL_FACE);
    ofFloatColor tcolor;
	tvector3 pts[3];
	pts[0] = orig;

	pts[1] = (axisU );
	pts[2] = (axisV );

	pts[1]*=size;
	pts[2]*=size;
	pts[1]+=orig;
	pts[2]+=orig;

	if (!bSelected)
		tcolor.set(1,1,0,0.5f);
	else
		tcolor.set(1,1,1,0.6f);
    ofSetColor( tcolor );
    ofDrawTriangle(pts[0].x, pts[0].y, pts[0].z,
                   pts[1].x, pts[1].y, pts[1].z,
                   pts[2].x, pts[2].y, pts[2].z );
//	glBegin(GL_TRIANGLES);
//	glVertex3fv(&pts[0].x);
//	glVertex3fv(&pts[1].x);
//	glVertex3fv(&pts[2].x);
//	glVertex3fv(&pts[3].x);
//	glEnd();

	if (!bSelected)
		tcolor.set(1,1,0.2f,1);
	else
		tcolor.set(1,1,1,0.6f);
    
    ofSetColor( tcolor );
    ofMesh tmesh;
    tmesh.setMode( OF_PRIMITIVE_LINE_STRIP );
    tmesh.addVertex( ofVec3f(pts[0].x, pts[0].y, pts[0].z) );
    tmesh.addVertex( ofVec3f(pts[1].x, pts[1].y, pts[1].z) );
    tmesh.addVertex( ofVec3f(pts[2].x, pts[2].y, pts[2].z) );
    tmesh.addVertex( ofVec3f(pts[0].x, pts[0].y, pts[0].z) );
//	glBegin(GL_LINE_STRIP);
//	glVertex3fv(&pts[0].x);
//	glVertex3fv(&pts[1].x);
//	glVertex3fv(&pts[2].x);
//	glEnd();

//	glDisable(GL_BLEND);
    ofDisableAlphaBlending();
}
