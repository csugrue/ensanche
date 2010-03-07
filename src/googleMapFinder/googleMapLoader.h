/*
 *  googleMapMaker.h
 *  ensanche_2.0
 *
 *  Created by Chris on 1/18/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ofMain.h"
#include "googleQueryMaker.h"
#include "SimpleWebScraper.h"
#include "ofMemoryImage.h"
#include "ofxOpenCv.h"

/* This is theorder  we load the tiles (and so on to outer frame)
 -----------
 | 5 | 1 | 6 |
 | 3 | 0 | 4 |
 | 7 | 2 | 8 |
 -----------
 */


class googleMapLoader{
	
	public:
	
	googleMapLoader();
	~googleMapLoader(){}
	
	void setup();
	void update();
	void beginDownloadingMap(string enteredAddress);
	void draw();
	int getNumLoaded(){ return nLoaded;}
	void loadSavedTiles( string path );
	
	// loaded tiles
	ofImage			tiles[GOOGLE_MAP_T_TILES];
	
	private:
	
	void getTile( int x, int y );
	
	// threaded loading
	SimpleWebScraper sws;
	bool            received;
	string			resp;
	ofMemoryImage	memImg;
	ofMemoryImage	threadedMemImage;
	StreamEventArgs * args;
	int				nLoaded;
	bool			bDownloading;
	
	ofxCvColorImage	cvColorImage;
	
	// query maker
	googleQueryMaker	googleQuery;
	
	
	// callback events ----------------------------------------------------------
	void onTextReceived(const void* pSender, string& response) {
		cout << "text received :" << endl;
		resp = response;
	}
	
	void onUnthreadedStreamReceived(const void* pSender, StreamEventArgs & args_) {
		cout << "unthreaded stream received. " << endl;
		memImg.loadFromData(args_.buff,args_.bytesToRead);
	}
	
	void onThreadedStreamReceived(const void* pSender, StreamEventArgs & args_) {
		
		cout << "threaded stream received. " << endl;
		args = &args_;
		threadedMemImage.loadFromData(args_.buff,args_.bytesToRead);
		received = true;
	}
	
};