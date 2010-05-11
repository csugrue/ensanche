/*
 *  ensancheModelBarrio.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/8/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheModelBarrio.h"

EnsancheModelBarrio::EnsancheModelBarrio()
{
	bEnabled = false;
	center.set(0,0,0);
	rotation.set(0,0,0);
	lastMouse.set(0,0,0);
	drawMode	= MODEL_DRAW_MODE_3D;
	scale		= 1;
	editMode	= MODEL_EDIT_MODE_NORMAL;
	boundingBox = ofRectangle(0,0,0,0);
	pov.set(0,0,0);
}

EnsancheModelBarrio::~EnsancheModelBarrio()
{
	ofRemoveListener(ofEvents.mousePressed, this, &EnsancheModelBarrio::mousePressed);
	ofRemoveListener(ofEvents.mouseDragged, this, &EnsancheModelBarrio::mouseDragged);
}

EnsancheModelBarrio::EnsancheModelBarrio( const EnsancheModelBarrio & mom)
{
	buildings.assign( mom.buildings.begin(),mom.buildings.end() );
	
	bEnabled	= mom.bEnabled;
	center		= mom.center;
	rotation	= mom.rotation;
	lastMouse	= mom.lastMouse;
	scale		= mom.scale;
	editMode	= mom.editMode;
	boundingBox = mom.boundingBox;
	pov			= mom.pov;
	textureWall = mom.textureWall; // not sure this is good
}

void EnsancheModelBarrio::setup()
{
	ofAddListener(ofEvents.mousePressed, this, &EnsancheModelBarrio::mousePressed);
	ofAddListener(ofEvents.mouseDragged, this, &EnsancheModelBarrio::mouseDragged);
	
	ofImage txImg;
	txImg.loadImage("textures/wallTexture.jpg");
	textureWall.allocate(txImg.width,txImg.height,GL_RGB,false);
	textureWall.loadData(txImg.getPixels(),txImg.width,txImg.height,GL_RGB);
	textureWall.setTextureWrap(GL_REPEAT, GL_REPEAT);
	
	for( int i = 0; i < MODEL_T_TEXTURES; i++)
	{
		ofImage mtxImg;
		mtxImg.loadImage("textures/wallTexture_"+ofToString(i)+".jpg");
		//mtxImg.mirror(0,1);
		textureWalls[i].allocate(mtxImg.width,mtxImg.height,GL_RGB,false);
		textureWalls[i].loadData(mtxImg.getPixels(),mtxImg.width,mtxImg.height,GL_RGB);
		textureWalls[i].setTextureWrap(GL_REPEAT, GL_REPEAT);
	}
}

void EnsancheModelBarrio::clear()
{
	buildings.clear();
	center.set(0,0,0);
	rotation.set(0,0,0);
	scale = 1.f;
	boundingBox = ofRectangle(0,0,0,0);
	pov.set(0,0,0);

}

void EnsancheModelBarrio::draw2D()
{
	for( int i = 0; i < buildings.size(); i++)
	{
		buildings[i].draw( MODEL_DRAW_MODE_2D, true );
	}
}

void EnsancheModelBarrio::draw()
{
	// draw centered
	glPushMatrix();
		glTranslatef(-center.x,-center.y,-center.z);
	
		glPushMatrix();
			glTranslatef(center.x,center.y,center.z);
			glScalef(scale,scale,1);
			glRotatef(rotation.x,1,0,0);
			glRotatef(rotation.y,0,1,0);
			glRotatef(rotation.z,0,0,1);
			glTranslatef(-center.x,-center.y,-center.z);
			
			

			for( int i = 0; i < buildings.size(); i++)
			{
				buildings[i].draw( drawMode, true );
			}
			
			
			
			glBegin(GL_LINE_STRIP);
				glVertex3f(boundingBox.x,0,boundingBox.y);
				glVertex3f(boundingBox.x+boundingBox.width,0,boundingBox.y);
				glVertex3f(boundingBox.x+boundingBox.width,0,boundingBox.y+boundingBox.height);
				glVertex3f(boundingBox.x,0,boundingBox.y+boundingBox.height);
				glVertex3f(boundingBox.x,0,boundingBox.y);
			glEnd();
			
		glPopMatrix();
	glPopMatrix();
}

void EnsancheModelBarrio::drawWithCamera()
{
	
	// set up a screen so we translate the eye/center to the position in the map
	// this is set to be centered on screen but the viewport,w,h could be changed for a smaller preview etc.
	
	// this is same as ofSetupScreen() but we know the eye dist so can translate accordingly
	int w, h;
	
	w = ofGetWidth();
	h = ofGetHeight();
	
	float halfFov, theTan, screenFov, aspect;
	screenFov 		= 60.0f;
	
	float eyeX 		= (float)w / 2.0;
	float eyeY 		= (float)h / 2.0;
	halfFov 		= PI * screenFov / 360.0;
	theTan 			= tanf(halfFov);
	float dist 		= eyeY / theTan;
	float nearDist 	= dist / 1000.0;	// near / far clip plane
	float farDist 	= dist * 1000.0;
	aspect 			= (float)w/(float)h;
		
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(screenFov, aspect, nearDist, farDist);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);

	// new perspective
	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
  	glTranslatef(0, -h, 0);       // shift origin up to upper-left corner.
	
	glPushMatrix();
	
		// translate to be centered
		glTranslatef(ofGetWidth()/2.f,ofGetHeight()/2.f,dist);
	
	
		glPushMatrix();
			
			// translate to our map point of view position
			glTranslatef(-pov.x,pov.y,-pov.z);
			
			// translate back to make rotations from this position
			glTranslatef(pov.x,-pov.y,pov.z);
				glRotatef(rotation.x,1,0,0);
				glRotatef(rotation.y,0,1,0);
			glTranslatef(-pov.x,pov.y,-pov.z);
			
			// draw the buildings
			for( int i = 0; i < buildings.size(); i++)
			{
				//buildings[i].draw( drawMode, true );
			}
			
			for( int i = 0; i < buildingsAdv.size(); i++)
			{
				buildingsAdv[i].draw( drawMode, true );
			}
			
		glPopMatrix();
	
	glPopMatrix();
	
	// reset view
	ofSetupScreen();
}


void EnsancheModelBarrio::addBuildings(EnsancheBarrio * barrio)
{
	for( int i = 0; i < barrio->buildings.size(); i++)
	{
		buildings.push_back( EnsancheModelBuilding() );
		buildings[i].setup();
		
		for( int j = 0; j < barrio->buildings[i].nFloors; j++)
			buildings[i].addBuildingFloor( barrio->buildings[i].buildingPoly);
		
		// multi texture
		buildingsAdv.push_back( EnsancheModelBuildingAdv() );
		buildingsAdv[i].setup();
		
		for( int j = 0; j < barrio->buildings[i].nFloors; j++)
			buildingsAdv[i].addBuildingFloor( barrio->buildings[i] );
		
		//cout << "setting texture " << i << endl;
		
		//buildings[i].setWallTexture(&textureWall);
	}
	
	boundingBox = barrio->getGroupBoundingBox();
	center.set( boundingBox.x+boundingBox.width*.5, 0.f, boundingBox.y + boundingBox.height*.5);
	
	for( int i = 0; i < barrio->buildings.size(); i++){
			buildings[i].setWallTexture(&textureWall);
			buildings[i].generateModel(4.f);
	}
	
	for( int i = 0; i < barrio->buildings.size(); i++){
		for( int j = 0; j < MODEL_T_TEXTURES; j++)
			buildingsAdv[i].setWallTexture(&textureWalls[j],j);
		buildingsAdv[i].generateModel(4.f);
	}
	
	for( int i = 0; i < barrio->buildings.size(); i++)	
		cout << i << " use texture " << buildings[i].bSetWallTexture << endl;
	// calc bounding and center for all
}

void EnsancheModelBarrio::mouseDragged(ofMouseEventArgs& event)
{
	if(!bEnabled) return;
	
	if( editMode == MODEL_EDIT_MODE_SHIFT)
	{
		scale += .1 * (event.y-lastMouse.y);
		
	}else if( event.button != 0 )
	{
		rotation.x += event.y-lastMouse.y;
	}else{
		rotation.y += event.x-lastMouse.x;
	}
	
	lastMouse.set( event.x, event.y );
}

void EnsancheModelBarrio::mousePressed(ofMouseEventArgs& event)
{
	if(!bEnabled) return;
	lastMouse.set( event.x, event.y );
	
	int modifier = glutGetModifiers();
	
	if( modifier == GLUT_ACTIVE_SHIFT) editMode = MODEL_EDIT_MODE_SHIFT;
	else editMode = MODEL_EDIT_MODE_NORMAL;

}

