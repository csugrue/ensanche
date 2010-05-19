/*
 *  ensancheModelBuilding.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/8/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheModelBuildingAdv.h"


EnsancheModelBuildingAdv::EnsancheModelBuildingAdv()
{
	
	offSet.set(0,0,0);
	center.set(0,0,0);
	rotation.set(0,0,0);
	lastMouse.set(0,0);
	
	nPts		= 0;
	nFloorPts	= 0;
	nTexPts		= 0;
	scale		= 1;
	nFloors		= 0;
	
	editMode = MODEL_EDIT_MODE_NORMAL;
	
	bSetWallTexture = false;
	memset(bSetWallTextures,0,MODEL_T_TEXTURES*sizeof(bool));
	
	memset(nptsv,0,MODEL_T_TEXTURES*sizeof(int));
	memset(ntexptsv,0,MODEL_T_TEXTURES*sizeof(int));
}

EnsancheModelBuildingAdv::~EnsancheModelBuildingAdv()
{
	if( nPts > 0 )		delete [] pts;
	if(nFloorPts > 0)	delete [] floorPts;
	if(nTexPts > 0)		delete [] texPts;
	
	ofRemoveListener(ofEvents.mousePressed, this, &EnsancheModelBuildingAdv::mousePressed);
	ofRemoveListener(ofEvents.mouseDragged, this, &EnsancheModelBuildingAdv::mouseDragged);
	
	buildingFloors.clear();
	
	//!! NOTE check if this is ok for delete other arrays!!
	for( int i = 0; i < MODEL_T_TEXTURES; i++)
	{
		if(nptsv[i] > 0) delete [] ptsv[i];
		if(ntexptsv[i] > 0) delete [] texptsv[i];
	}
}

EnsancheModelBuildingAdv::EnsancheModelBuildingAdv(const EnsancheModelBuildingAdv & mom )
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
		memcpy(texPts,mom.texPts,mom.nTexPts*sizeof(float) );
	}
	
	// copy all data
	wallTexIds.assign(mom.wallTexIds.begin(),mom.wallTexIds.end());
	
	// NOTE: not sure if this is going to make problemss.... check it...
	/*for( int i = 0; i < MODEL_T_TEXTURES; i++)
	{
	
		if( mom.nptsv[i] > 0 )
		{
			if(nptsv[i] > 0 ) delete [] ptsv[i];
			nptsv[i] = mom.nptsv[i];
			ptsv[i] = new float[ nptsv[i] ];
			memcpy( ptsv[i],mom.ptsv[i], mom.nptsv[i]*sizeof(float) );
		}
		
		if( mom.ntexptsv[i] > 0 )
		{
			if(ntexptsv[i] > 0 ) delete [] texptsv[i];
			ntexptsv[i] = mom.ntexptsv[i];
			texptsv[i] = new float[ ntexptsv[i] ];
			memcpy( texptsv[i],mom.texptsv[i], mom.ntexptsv[i]*sizeof(float) );
		}
	}*/
	
	// NOTE: does not keep texture
	bSetWallTexture = false;	
}

void EnsancheModelBuildingAdv::setup()
{
	ofAddListener(ofEvents.mousePressed, this, &EnsancheModelBuildingAdv::mousePressed);
	ofAddListener(ofEvents.mouseDragged, this, &EnsancheModelBuildingAdv::mouseDragged);

}

void EnsancheModelBuildingAdv::clear()
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
	
	memset(nptsv,0,MODEL_T_TEXTURES*sizeof(int));
	memset(ntexptsv,0,MODEL_T_TEXTURES*sizeof(int));
}

void EnsancheModelBuildingAdv::setWallTexture(ofTexture * tex, int type)
{
	textureWall = tex;
	bSetWallTexture = true;
	
	if( type < MODEL_T_TEXTURES)
	{
	textureWalls[type] = tex;
	bSetWallTextures[type] = true;
	}
}

void EnsancheModelBuildingAdv::draw(int drawMode, bool bDrawWOffset)
{
	if( drawMode == MODEL_DRAW_MODE_3D ) draw3D( bDrawWOffset );
	else								 draw2D( bDrawWOffset );
}

void EnsancheModelBuildingAdv::draw2D(bool bDrawWOffset)
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

void EnsancheModelBuildingAdv::draw3D(bool bDrawWOffset)
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
	
	// for each texture type
	for( int i = 0; i < MODEL_T_TEXTURES; i++)
	{
		// get nDrawPts
		nDrawPts = nptsv[i] / 3.f;
		
		// apply texture
		if(bSetWallTextures[i]){
			
			//textureWall->setTextureWrap(GL_REPEAT, GL_REPEAT);
			glEnable(textureWalls[i]->texData.textureTarget);
			
			// bind the texture
			glBindTexture( textureWalls[i]->texData.textureTarget, (GLuint)textureWalls[i]->texData.textureID );
			
			glTexParameterf(textureWalls[i]->texData.textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(textureWalls[i]->texData.textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
			
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			
			glTexCoordPointer(2, GL_FLOAT, 0, texptsv[i]);
			
		}
		
		
		// draw pts
		glEnableClientState(GL_VERTEX_ARRAY);
		
		glVertexPointer(3, GL_FLOAT, 0, ptsv[i]);
		
		glDrawArrays(GL_QUADS, 0, nDrawPts);
		
		if(bSetWallTextures[i])  glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		
		glDisableClientState(GL_VERTEX_ARRAY);
		
		if(bSetWallTextures[i]) glDisable(textureWalls[i]->texData.textureTarget);
	}
	
	/*
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
	
	glVertexPointer(3, GL_FLOAT, 0, pts);
	
	glDrawArrays(GL_QUADS, 0, nDrawPts);
	
	if(bSetWallTexture)  glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glDisableClientState(GL_VERTEX_ARRAY);
	
	if(bSetWallTexture) glDisable(textureWall->texData.textureTarget);
*/
	glPopMatrix();
}
/*
 void EnsancheModelRoom::setFromPolygon( polyEditable poly, float wallHeigh, bool bOffset  )
 {
 
 }
 */
void EnsancheModelBuildingAdv::setFloor(EnsancheBuilding buildingFloor, int floorNum )
{
	if( buildingFloors.size() > floorNum)
	{
		buildingFloors[ floorNum ].clear();
		buildingFloors[ floorNum ] = buildingFloor.buildingPoly;
	}
	
}

void EnsancheModelBuildingAdv::setupFromBuilding(EnsancheBuilding building)
{
	cout << "set up model with " << building.nFloors << " floors " << endl;
	for( int i = 0; i < building.nFloors; i++)
		addBuildingFloor( building);//.buildingPoly );
}

void EnsancheModelBuildingAdv::addBuildingFloor( EnsancheBuilding & building, bool bOffset )
{
	
	if( nFloors == 0 ){
	 ofPoint c = building.buildingPoly.getCentroid();
	 center.set(c.x,0,c.y);
	}
	
	nFloors++;
	polySimple poly;
	buildingFloors.push_back(building.buildingPoly);
	
	// store ids of each wall for each floor for texturing later
	wallTextureIds wIds;
	wallTexIds.push_back(wIds);
		
	int bdOn =  buildingFloors.size()-1;
	
	buildingFloors[ bdOn ].bDrawnWithPoints = false;
	
	for(int i = 0; i < building.buildingPoly.pts.size(); i++)
	{
		buildingFloors[ bdOn ].pushVertex(building.buildingPoly.pts[i]);
		
	}
	
	if(buildingFloors[ bdOn ].pts.size() > 0 ) buildingFloors[ bdOn ].pushVertex(buildingFloors[ bdOn ].pts[0]);
	
	ofRectangle boundingBox = building.buildingPoly.getBoundingBox();
	offSet.set(boundingBox.x,boundingBox.y);
	
	if(bOffset) 
	{
		for(int i = 0; i < buildingFloors[ bdOn ].pts.size(); i++)
		{
			buildingFloors[ bdOn ].pts[i].x -= boundingBox.x;
			buildingFloors[ bdOn ].pts[i].y -= boundingBox.y;
		}
	}
	
	
	// get type from each wall
	for( int i = 0; i <  buildingFloors[ bdOn ].pts.size(); i++)
		wallTexIds[wallTexIds.size()-1].tId.push_back(i%MODEL_T_TEXTURES);//building.walls[i].type);
	
}

void EnsancheModelBuildingAdv::generateModel( float wallHeight )
{
	
	// y inverted
	if(wallHeight > 0 ) wallHeight *= -1;
	
	// set the center y position of model
	center.y = (buildingFloors.size()*wallHeight ) * .5;
	
	// memset the totals
	memset(nptsv,0,MODEL_T_TEXTURES*sizeof(int));
	memset(ntexptsv,0,MODEL_T_TEXTURES*sizeof(int));
	
	// calculate totatls for each type of texture
	for( int i = 0; i < wallTexIds.size(); i++)
	{
		for( int j = 0; j < wallTexIds[i].tId.size(); j++)
		{
			int id = wallTexIds[i].tId[j];
			if( id >= MODEL_T_TEXTURES ) id = 0;
			nptsv[id]++;
			ntexptsv[id]++;
		}
	}
	
	// multiply so includes memory for 4 corners of quad and 3 coordinates, 2 coord for texture
	for( int i = 0; i < MODEL_T_TEXTURES; i++)
	{
		nptsv[i] = (nptsv[i]*4)*3;
		ntexptsv[i] = (ntexptsv[i]*4)*2;
	}
	
	// allocate memory for pts for textures
	for( int i = 0; i < MODEL_T_TEXTURES; i++)
	{
		ptsv[i]		= new float[ nptsv[i]	];
		texptsv[i]	= new float[ ntexptsv[i] ];
	}
	
	// reset totals 
	memset(nptsv,0,MODEL_T_TEXTURES*sizeof(int));
	memset(ntexptsv,0,MODEL_T_TEXTURES*sizeof(int)); 
	
	// leave, maybe need offset later??
	ofRectangle boundingBox = ofRectangle(0,0,0,0);
	
	// create pts
	for( int i = 0; i < buildingFloors.size(); i++)
	{
		for( int j = 0; j < buildingFloors[i].pts.size()-1; j++)
		{
			int id = wallTexIds[i].tId[j];
			if( id >= MODEL_T_TEXTURES || id < 0 ) id = 0;
			
			cout << "i: " << i << " j: " << j << "id " << id << " walltype " << wallTexIds[i].tId[j] << endl;
			
			ptsv[id][ nptsv[id] ]	= buildingFloors[i].pts[j].x-boundingBox.x;
			ptsv[id][ nptsv[id]+1]	= i * wallHeight;
			ptsv[id][ nptsv[id]+2]	= buildingFloors[i].pts[j].y-boundingBox.y;
			nptsv[id] += 3;
			
			ptsv[id][ nptsv[id]]	= buildingFloors[i].pts[j+1].x-boundingBox.x;
			ptsv[id][ nptsv[id]+1]	= i * wallHeight;
			ptsv[id][ nptsv[id]+2]	= buildingFloors[i].pts[j+1].y-boundingBox.y;
			nptsv[id] += 3;
			
			ptsv[id][ nptsv[id]]	= buildingFloors[i].pts[j+1].x-boundingBox.x;
			ptsv[id][ nptsv[id]+1]	= (i * wallHeight)+wallHeight;
			ptsv[id][ nptsv[id]+2]	= buildingFloors[i].pts[j+1].y-boundingBox.y;
			nptsv[id] += 3;
			
			ptsv[id][ nptsv[id]]	= buildingFloors[i].pts[j].x-boundingBox.x;
			ptsv[id][ nptsv[id]+1]	= (i * wallHeight)+wallHeight;
			ptsv[id][ nptsv[id]+2]	= buildingFloors[i].pts[j].y-boundingBox.y;
			nptsv[id] += 3;
		}
	}
	
	float tx0 = 0;
	float ty0 = 0;
	float tx1 = (bSetWallTexture) ? 2.5*textureWall->texData.tex_t : 1;
	float ty1 = (bSetWallTexture) ? textureWall->texData.tex_u : 1;
	
	
	// create texture pts
	for( int i = 0; i < buildingFloors.size(); i++)
	{
		for( int j = 0; j < buildingFloors[i].pts.size()-1; j++)
		{
			
			int id = wallTexIds[i].tId[j];
			if( id >= MODEL_T_TEXTURES || id < 0 ) id = 0;
			
			// get len wall
			float len = sqrt( (buildingFloors[i].pts[j].x-buildingFloors[i].pts[j+1].x)*(buildingFloors[i].pts[j].x-buildingFloors[i].pts[j+1].x) + 
					 (buildingFloors[i].pts[j].y-buildingFloors[i].pts[j+1].y)*(buildingFloors[i].pts[j].y-buildingFloors[i].pts[j+1].y) );
	
			float pct = len / 4.f;
			texptsv[id][ ntexptsv[id] ]	= tx0;		texptsv[id][ ntexptsv[id]+1] = ty0;
			texptsv[id][ ntexptsv[id]+2] = pct*tx1;	texptsv[id][ ntexptsv[id]+3] = ty0;
			texptsv[id][ ntexptsv[id]+4] = pct*tx1;	texptsv[id][ ntexptsv[id]+5] = 2*ty1;
			texptsv[id][ ntexptsv[id]+6] = tx0;		texptsv[id][ ntexptsv[id]+7] = 2*ty1; 
			ntexptsv[id]+= 8;
			
		}
	}
	
	
}


void EnsancheModelBuildingAdv::mouseDragged(ofMouseEventArgs& event)
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

void EnsancheModelBuildingAdv::mousePressed(ofMouseEventArgs& event)
{
	if(!bEnabled) return;
	lastMouse.set( event.x, event.y );
	
	int modifier = glutGetModifiers();
	
	if( modifier == GLUT_ACTIVE_SHIFT) editMode = MODEL_EDIT_MODE_SHIFT;
	else editMode = MODEL_EDIT_MODE_NORMAL;
}

