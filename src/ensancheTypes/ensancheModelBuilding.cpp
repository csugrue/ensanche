/*
 *  ensancheModelBuilding.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/8/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheModelBuilding.h"


EnsancheModelBuilding::EnsancheModelBuilding()
{
	
	offSet.set(0,0,0);
	center.set(0,0,0);
	rotation.set(0,0,0);
	lastMouse.set(0,0);
	
	nPts		= 0;
	nFloorPts	= 0;
	nTexPts		= 0;
	scale		= 1;
	
	editMode = MODEL_EDIT_MODE_NORMAL;
	
	bSetWallTexture = false;
}

EnsancheModelBuilding::~EnsancheModelBuilding()
{
	if( nPts > 0 )		delete [] pts;
	if(nFloorPts > 0)	delete [] floorPts;
	if(nTexPts > 0)		delete [] texPts;
	
	ofRemoveListener(ofEvents.mousePressed, this, &EnsancheModelBuilding::mousePressed);
	ofRemoveListener(ofEvents.mouseDragged, this, &EnsancheModelBuilding::mouseDragged);
	
	buildingFloors.clear();
}

EnsancheModelBuilding::EnsancheModelBuilding(const EnsancheModelBuilding & mom )
{
	bEnabled	= mom.bEnabled;
	offSet		= mom.offSet;
	center		= mom.center;
	rotation	= mom.rotation;
	scale		= mom.scale;
	editMode	= mom.editMode;
	
	buildingFloors.assign(mom.buildingFloors.begin(),mom.buildingFloors.end());
	
	lastMouse	= mom.lastMouse;
	nFloors		= mom.nFloors;
		
	nPts		= mom.nPts;
	nFloorPts	= mom.nFloorPts;
	nTexPts		= mom.nTexPts;
	
	if(nPts > 0 ){
		pts = new float[nPts];
		memcpy(pts,mom.pts,mom.nPts*sizeof(float));
	}
	
	if(nFloorPts > 0 ){
		floorPts = new float[nFloorPts];
		memcpy(floorPts,mom.floorPts,mom.nFloorPts*sizeof(float));
	}
	
	if(nTexPts > 0 ){
		texPts = new float[nTexPts];
		memcpy(texPts,mom.texPts,mom.nTexPts*sizeof(float));
	}
	// NOTE: does not keep texture
	bSetWallTexture = false;	
}

void EnsancheModelBuilding::setup()
{
	ofAddListener(ofEvents.mousePressed, this, &EnsancheModelBuilding::mousePressed);
	ofAddListener(ofEvents.mouseDragged, this, &EnsancheModelBuilding::mouseDragged);	
}

void EnsancheModelBuilding::clear()
{
	
	offSet.set(0,0,0);
	center.set(0,0,0);
	rotation.set(0,0,0);
	nFloors = 0;
	
	if( nPts > 0 )		delete [] pts;
	if(nFloorPts > 0)	delete [] floorPts;
	if(nTexPts > 0 )	delete [] texPts;
	
	nPts = 0;
	nFloorPts = 0;
	nTexPts = 0;
	
	
	buildingFloors.clear();
	
	bSetWallTexture = false;
}

void EnsancheModelBuilding::setWallTexture(ofTexture * tex)
{
	textureWall = tex;
	bSetWallTexture = true;
}

void EnsancheModelBuilding::draw(int drawMode, bool bDrawWOffset)
{
	if( drawMode == MODEL_DRAW_MODE_3D ) draw3D( bDrawWOffset );
	else								 draw2D( bDrawWOffset );
}

void EnsancheModelBuilding::draw2D(bool bDrawWOffset)
{
	glPushMatrix();
	
	if(bDrawWOffset) glTranslatef(offSet.x,offSet.y,0);
	
	for(int i = 0; i < buildingFloors.size(); i++)
	{
		
			buildingFloors[i].bDrawnWithPoints = false;
			buildingFloors[i].draw();
	}
	
	glPopMatrix();

	
}

void EnsancheModelBuilding::draw3D(bool bDrawWOffset)
{

	int nDrawPts = nPts/3.f;
	
	glPushMatrix();
	
	
	if( bDrawWOffset) glTranslatef(offSet.x,0.f,offSet.y);
	
	glTranslatef(center.x,center.y,center.z);
	glScalef(scale,scale,scale);
	glRotatef(rotation.x,1,0,0);
	glRotatef(rotation.y,0,1,0);
	glRotatef(rotation.z,0,0,1);
	glTranslatef(-center.x,-center.y,-center.z);
	
	
	if(bSetWallTexture){
		
		//textureWall->setTextureWrap(GL_REPEAT, GL_REPEAT);
		glEnable(textureWall->texData.textureTarget);
		
		// bind the texture
		glBindTexture( textureWall->texData.textureTarget, (GLuint)textureWall->texData.textureID );
		
		glTexParameterf(textureWall->texData.textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(textureWall->texData.textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		
		
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );

		glTexCoordPointer(2, GL_FLOAT, 0, texPts);

	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, pts);
	//glColorPointer( 4, GL_FLOAT, 0, color);
	
	//glDrawArrays(GL_TRIANGLE_FAN, 0, nDrawPts);
	//glDrawElements(GL_QUADS, 0, nDrawPts);
	glDrawArrays(GL_QUADS, 0, nDrawPts);
	
	//glDisableClientState(GL_COLOR_ARRAY);
	if(bSetWallTexture)  glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glDisableClientState(GL_VERTEX_ARRAY);
	
	//if(bSetWallTexture)  glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	if(bSetWallTexture) glDisable(textureWall->texData.textureTarget);

	glPopMatrix();
}
/*
 void EnsancheModelRoom::setFromPolygon( polyEditable poly, float wallHeigh, bool bOffset  )
 {
 
 }
 */

void EnsancheModelBuilding::addBuildingFloor( polySimple & buildingPoly, bool bOffset )
{
	
	if( nFloors == 0 ){
	 ofPoint c = buildingPoly.getCentroid();
	 center.set(c.x,0,c.y);
	}
	
	nFloors++;
	polySimple poly;
	buildingFloors.push_back(poly);
	
	int bdOn =  buildingFloors.size()-1;
	
	buildingFloors[ bdOn ].bDrawnWithPoints = false;
	
	for(int i = 0; i < buildingPoly.pts.size(); i++)
	{
		buildingFloors[ bdOn ].pushVertex(buildingPoly.pts[i]);
		
	}
	
	if(buildingFloors[ bdOn ].pts.size() > 0 ) buildingFloors[ bdOn ].pushVertex(buildingFloors[ bdOn ].pts[0]);
	
	ofRectangle boundingBox = buildingPoly.getBoundingBox();
	offSet.set(boundingBox.x,boundingBox.y);
	
	if(bOffset) 
	{
		for(int i = 0; i < buildingFloors[ bdOn ].pts.size(); i++)
		{
			buildingFloors[ bdOn ].pts[i].x -= boundingBox.x;
			buildingFloors[ bdOn ].pts[i].y -= boundingBox.y;
		}
	}
	
}

void EnsancheModelBuilding::generateModel( float wallHeight )
{
	
	// y inverted
	if(wallHeight > 0 ) wallHeight *= -1;
	
	center.y = (buildingFloors.size()*wallHeight ) * .5;
	
	// allocate memory for all walls and all floors
	int total = 0;
	int txTotal=0;
	
	for( int i = 0; i < buildingFloors.size(); i++)
		total += buildingFloors[i].pts.size();
		
	txTotal = (total*4)*2;
	total = (total * 4) * 3;
	
	texPts = new float[txTotal];
	pts = new float[total];
		
	// leave, maybe need offset later??
	ofRectangle boundingBox = ofRectangle(0,0,0,0);
	
	float tx0 = 0;
	float ty0 = 0;
	float tx1 = (bSetWallTexture) ? 2.5*textureWall->texData.tex_t : 1;
	float ty1 = (bSetWallTexture) ? textureWall->texData.tex_u : 1;
	
	nPts = 0;
	nTexPts = 0;
	
	for( int i = 0; i < buildingFloors.size(); i++)
	{
		for( int j = 0; j < buildingFloors[i].pts.size()-1; j++)
		{
		
			pts[nPts] = buildingFloors[i].pts[j].x-boundingBox.x;
			pts[nPts+1] = i * wallHeight;
			pts[nPts+2] = buildingFloors[i].pts[j].y-boundingBox.y;
			nPts += 3;
		
			pts[nPts] = buildingFloors[i].pts[j+1].x-boundingBox.x;
			pts[nPts+1] = i * wallHeight;
			pts[nPts+2] = buildingFloors[i].pts[j+1].y-boundingBox.y;
			nPts += 3;
		
			pts[nPts] = buildingFloors[i].pts[j+1].x-boundingBox.x;
			pts[nPts+1] = (i * wallHeight)+wallHeight;
			pts[nPts+2] = buildingFloors[i].pts[j+1].y-boundingBox.y;
			nPts += 3;
		
			pts[nPts] = buildingFloors[i].pts[j].x-boundingBox.x;
			pts[nPts+1] = (i * wallHeight)+wallHeight;
			pts[nPts+2] = buildingFloors[i].pts[j].y-boundingBox.y;
			nPts += 3;
			
			// texture coords
			
			// get len wall
			float len = sqrt( (buildingFloors[i].pts[j].x-buildingFloors[i].pts[j+1].x)*(buildingFloors[i].pts[j].x-buildingFloors[i].pts[j+1].x) + 
							  (buildingFloors[i].pts[j].y-buildingFloors[i].pts[j+1].y)*(buildingFloors[i].pts[j].y-buildingFloors[i].pts[j+1].y) );
			
			float pct = len / 4.f;
			texPts[nTexPts]	= tx0; texPts[nTexPts+1] = ty0;
			texPts[nTexPts+2] = pct*tx1; texPts[nTexPts+3] = ty0;
			texPts[nTexPts+4] = pct*tx1; texPts[nTexPts+5] = 2*ty1;
			texPts[nTexPts+6] = tx0; texPts[nTexPts+7] = 2*ty1; 
			nTexPts+= 8;
		} 
	}
	
	
	/*for( int i = 0; i < txTotal; i+=8)
	{
	texPts[i]	= tx0; texPts[i+1] = ty0;
	texPts[i+2] = tx1; texPts[i+3] = ty0;
	texPts[i+4] = tx1; texPts[i+5] = ty1;
	texPts[i+6] = tx0; texPts[i+7] = ty1;
	}*/
	
}


void EnsancheModelBuilding::mouseDragged(ofMouseEventArgs& event)
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

void EnsancheModelBuilding::mousePressed(ofMouseEventArgs& event)
{
	if(!bEnabled) return;
	lastMouse.set( event.x, event.y );
	
	int modifier = glutGetModifiers();
	
	if( modifier == GLUT_ACTIVE_SHIFT) editMode = MODEL_EDIT_MODE_SHIFT;
	else editMode = MODEL_EDIT_MODE_NORMAL;
}

