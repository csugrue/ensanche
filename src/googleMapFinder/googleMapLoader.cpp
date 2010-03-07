/*
 *  googleMapLoader.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 1/18/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "googleMapLoader.h"

// this descibes the position offsets for the tiles
// so tile[0] is center with offset 0,0 in x and y
int xtiles[] = {0,0,0,-1,1,-1,1,-1,1,-2,-1,0,1,2,-2,-1,0,1,2,-2,-2,-2,2,2,2};
int ytiles[] = {0,1,-1,0,0,1,1,-1,-1,2,2,2,2,2,-2,-2,-2,-2,-2,1,0,-1,1,0,-1};


googleMapLoader::googleMapLoader()
{
}


void googleMapLoader::setup()
{
	// googleQuery.setup("28 Calle de las deliciase, madrid, spain");
	
	sws.textReady				+= Delegate<googleMapLoader, string>(this, &googleMapLoader::onTextReceived);
	sws.unthreadedStreamReady	+= Delegate<googleMapLoader, StreamEventArgs>(this, &googleMapLoader::onUnthreadedStreamReceived);
	sws.threadedStreamReady		+= Delegate<googleMapLoader, StreamEventArgs>(this, &googleMapLoader::onThreadedStreamReceived);
	
	threadedMemImage.setUseTexture(false); // we can't use a texture with this one
	received		= false;
	nLoaded			= 0;
	bDownloading	= false;
	
	for( int i = 0; i < GOOGLE_MAP_T_TILES; i++)
	{
		tiles[i].allocate(GOOGLE_MAP_TILE_SIZE,GOOGLE_MAP_TILE_SIZE-GOOGLE_MAP_OFFSET,OF_IMAGE_COLOR);
	}
	
	cvColorImage.allocate(GOOGLE_MAP_TILE_SIZE,GOOGLE_MAP_TILE_SIZE);
}

void  googleMapLoader::update()
{
	// if we are downloading, and the last image has been received and num loaded is not == total, then load next map tile
	if( bDownloading && received && nLoaded < GOOGLE_MAP_T_TILES)
	{
		
		// save last loaded
		cvColorImage.setFromPixels(threadedMemImage.getPixels(), GOOGLE_MAP_TILE_SIZE, GOOGLE_MAP_TILE_SIZE);
		cvColorImage.mirror(1,0);
		tiles[nLoaded].setFromPixels(cvColorImage.getPixels(), GOOGLE_MAP_TILE_SIZE, GOOGLE_MAP_TILE_SIZE-GOOGLE_MAP_OFFSET, OF_IMAGE_COLOR);
		tiles[nLoaded].saveImage("tiles/tile"+ofToString(nLoaded)+".jpg");
		
		// reset to load again
		nLoaded++;										// increment total downloaded
		received = false;								// reset received flag
		
		if(nLoaded < GOOGLE_MAP_T_TILES) getTile( xtiles[ nLoaded ], ytiles[ nLoaded ] );  // start get the next tile
		else 
			bDownloading = false;
	
	}
	
}

void  googleMapLoader::beginDownloadingMap(string enteredAddress)
{
	// reset
	received = false;
	nLoaded = 0;
	
	// create first query string and set up the query maker
	googleQuery.setup(enteredAddress);
	
	if( googleQuery.status == "error" ){
		cout << "error" << endl;
		return;
	}
	
	// set to downloading
	bDownloading = true;
				
	// get first tile
	getTile( xtiles[ nLoaded ], ytiles[ nLoaded ] );
}

void googleMapLoader::getTile( int x, int y)
{
	string url = googleQuery.getQueryForTile( x,y );
	if( url != "error" )
		sws.getStreamThreaded(url);
}

void googleMapLoader::draw()
{
	glPushMatrix();
		//glTranslatef(-GOOGLE_MAP_TILE_SIZE,-(GOOGLE_MAP_TILE_SIZE-GOOGLE_MAP_OFFSET),0);
		for( int i = 0; i < nLoaded; i++)
		{
			tiles[i].draw( GOOGLE_MAP_TILE_SIZE * xtiles[i], (GOOGLE_MAP_TILE_SIZE-GOOGLE_MAP_OFFSET)*-ytiles[i]);
		}
	
		for( int i = 0; i < nLoaded; i++)
		{
			//ofNoFill();
			//ofRect(GOOGLE_MAP_TILE_SIZE * xtiles[i], (GOOGLE_MAP_TILE_SIZE-GOOGLE_MAP_OFFSET)*-ytiles[i],GOOGLE_MAP_TILE_SIZE,GOOGLE_MAP_TILE_SIZE-GOOGLE_MAP_OFFSET);
		}
	glPopMatrix();
}

void googleMapLoader::loadSavedTiles( string path )
{
	nLoaded = 0;
	for( int i = 0; i < GOOGLE_MAP_T_TILES; i++)
	{
		tiles[i].loadImage(path+"tile_"+ofToString(i) +".jpg");
		nLoaded++;
	}
	bDownloading = false;

}

