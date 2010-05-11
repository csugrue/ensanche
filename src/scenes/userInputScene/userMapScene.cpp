#include "userMapScene.h"

void userMapScene::setup()
{
	statusMessage	= "none";
	bPanelOn		= false;
	bUsingGoogleMap = false;
	bSavedTiles		= false;
	bSetUserName	= false;
	
	setupControlPanel();
	
	myMap.loadImage("images/map.jpg");
	mapPoly.createFromRect( ofRectangle(0,0,myMap.width,myMap.height) );
	
	// applies to map
	rotation=0;
	
	// setup for google map, mapPoly is used to move the map on screen
	googleLoader.setup();
	nTilesLoaded = 0;
	mapPoly.setMode( POLY_EDIT_MODE_MOVE_ALL);
	mapPoly.bUseKeyPress = false;
	mapPoly.setup();
	mapPoly.bUseRotation = true;
	
	// set up with one building to start but no points
	buildingGroup.setup();
	
	userdir = "none";
	username = "none";
	lastSelectedBuilding = -1;
}

userMapScene::~userMapScene()
{
}

void userMapScene::disable()
{
	mapPoly.disable();
	buildingGroup.disableAll(true);
	
}


void userMapScene::update()
{
	// update controls
	if(bPanelOn) updateControlPanel();
	
	
	// update goolge map loader
	googleLoader.update();
	
	// updates for different modes
	if( panel.getSelectedPanelName() == "adjust map" ){
		mapPoly.enable();
	}else 
		mapPoly.disable();
	
	
	// be sure to save our tiles after all have been loaded
	if( !bSavedTiles && googleLoader.getNumLoaded() == GOOGLE_MAP_T_TILES )
	{
		saveTiles(userdir);
	}
	
}

void userMapScene::findMap( string address)
{
	if( address.size() <= 0 ) return;
	
	bSavedTiles = false;
	googleLoader.beginDownloadingMap(address);
	bUsingGoogleMap = true;
	nTilesLoaded = 0;
	mapPoly.createFromRect(ofRectangle(0,0,GOOGLE_MAP_TILE_SIZE*GOOGLE_MAP_N_TILES_X,(GOOGLE_MAP_TILE_SIZE-GOOGLE_MAP_OFFSET)*GOOGLE_MAP_N_TILES_Y));

}



void userMapScene::preDraw()
{
}

void userMapScene::draw()
{
	ofBackground(0,0,0);
	
	ofEnableAlphaBlending();
	
	float mapW, mapH;
	
	//--- get map dimensions depending on what is used (maybe move from here)
	if(bUsingGoogleMap) 
	{
		mapW = GOOGLE_MAP_TILE_SIZE;//GOOGLE_MAP_TILE_SIZE*GOOGLE_MAP_N_TILES_X;
		mapH = (GOOGLE_MAP_TILE_SIZE-GOOGLE_MAP_OFFSET);//(GOOGLE_MAP_TILE_SIZE-GOOGLE_MAP_OFFSET)*GOOGLE_MAP_N_TILES_Y;
	}
	else
	{
		mapW = myMap.width;
		mapH = myMap.height;
	}
	
	//--- get current zomo level
	float scale = panel.getValueF("ZOOM");
	
	//--- calc translations
	ofPoint transPreRotate = ofPoint( ofGetWidth()*.5+mapPoly.getPt(0).x,ofGetHeight()*.5 + mapPoly.getPt(0).y);
	ofPoint transPstRotate = ofPoint( -mapW*.5,-mapH*.5 );
	
	buildingGroup.setOffset( transPreRotate, transPstRotate);
	buildingGroup.setGRotation( mapPoly.rotation);
	buildingGroup.setScale(scale);
	
	
	//--- draw google map
	glPushMatrix();
		
		glTranslatef(transPreRotate.x,transPreRotate.y,0);
		glRotatef(mapPoly.rotation,0,0,1);
		glTranslatef(transPstRotate.x,transPstRotate.y,0);
		
		glScalef(scale,scale,1);
		
		float shade = 255 * panel.getValueF("SHADE");
		ofSetColor(shade,shade,shade);
		
		if(bUsingGoogleMap)		googleLoader.draw();
		else					myMap.draw(0,0);
	
		
	glPopMatrix();
	
	// mapPoly.draw(); // debug only
	
	//--- draw grid
	if( panel.getValueB("grid") )
	{
		ofPushStyle();
			ofEnableAlphaBlending();
			ofSetColor(200,200,200,100);
			enDrawGrid(10,10);
		ofPopStyle();
	}
	
	//--- draw buildings
	
	glPushMatrix();
		glTranslatef(transPreRotate.x,transPreRotate.y,0);
		glRotatef(mapPoly.rotation,0,0,1);
		glTranslatef(transPstRotate.x,transPstRotate.y,0);
	
		glScalef(scale,scale,1);
		
		buildingGroup.draw();
		
		//ofSetColor(255,0,0);
		//ofCircle(mpt.x, mpt.y, 10);
	
	glPopMatrix();
	
	//--- draw controls
	if(bPanelOn) panel.draw();
	
	

}

void userMapScene::keyPressed(int key)
{
	if( key == OF_KEY_F1) bPanelOn = !bPanelOn;
	
	if( panel.isAnyTextBoxActive() ){
		if( key == OF_KEY_BACKSPACE) panel.deleteLastChar();
		else{
			panel.addChar(key);
		}
		return;
	}
	
	
}

void userMapScene::keyReleased(int key)
{

}

void userMapScene::mouseMoved(int x, int y)
{
	
	if( panel.getSelectedPanelName() != "edit buildings")
	{
		buildingGroup.disableAll(true);
	}
	else if( bPanelOn && panel.isMouseInPanel(x, y) ){
		buildingGroup.disableAll(true);
	}
	else if( panel.getSelectedPanelName() == "edit buildings" ){
		buildingGroup.reEnableLast();
	}
	
	mouse.x = x;
	mouse.y = y;
}

void userMapScene::mouseDragged(int x, int y, int button)
{
	
	
	if(bPanelOn) panel.mouseDragged(x,y,button);
	
	// disable mappoly if we are in control panel
	if( panel.bMouseInPanel ) mapPoly.disable();
	
	
	
}

void userMapScene::mousePressed(int x, int y, int button)
{
	if(bPanelOn) panel.mousePressed(x,y,button);
	
	
}

void userMapScene::mouseReleased(int x, int y, int button)
{
	if(bPanelOn) panel.mouseReleased();
	lastSelectedBuilding = buildingGroup.selectedId;
}

string userMapScene::getStatusMessage()
{
	return statusMessage;
}

void userMapScene::setupControlPanel()
{
	
	panel.setup("user map", ofGetWidth()-260, ofGetHeight()-450, 240, 370);
	panel.addPanel("adjust map", 1, false);
	panel.addPanel("edit buildings", 1, false);
	
	panel.setWhichPanel("adjust map");
	panel.addToggle("grid", "grid", false);
	panel.addToggle("reset map", "reset_map", false);
	panel.addToggle("save tiles", "save_tiles", false);
	panel.addSlider("zoom","ZOOM",1,.25,2,false);
	panel.addSlider("shade","SHADE",1,0,1,false);
	
	panel.setWhichPanel("edit buildings");
	/*vector<string> buildingOptions;
	buildingOptions.push_back("add points");
	buildingOptions.push_back("edit points");
	buildingOptions.push_back("move building");
	buildingOptions.push_back("rotate");
	
	panel.addMultiToggle("buildings", "BUILDINGS", 0, buildingOptions);*/
	panel.addSlider("num floors","NUM_FLOORS",6,1,20,true);
	panel.addToggle("draw data", "DRAW_DATA", true);
	panel.addToggle("set as my building", "ME", false);
	//slider_building_chooser = panel.addSlider("building to edit","EDIT_THIS_BUILDING",0,0,1,true);
	
	panel.update();
	
	
}

void userMapScene::updateControlPanel()
{
	panel.update();
	
	
	 if( panel.getValueB("reset_map") )
	 {
		 panel.setValueB("reset_map", false);
		 mapPoly.rotation = 0;
		 mapPoly.moveAllPointsBy(ofPoint(0-mapPoly.getPt(0).x,0-mapPoly.getPt(0).y));
	 }
	
	if( panel.getValueB("save_tiles") )
	{
		panel.setValueB("save_tiles", false);
		saveTiles(userdir);
		saveUserFile();//MapData();

	}
	
	
	if(panel.getSelectedPanelName() == "edit buildings")
	{
		buildingGroup.bDrawData = panel.getValueB("DRAW_DATA");
		
		if( panel.getValueB("ME") )
		{
			panel.setValueB("ME",false);
			buildingGroup.setIsMe();
		}
		
		/*switch( panel.getValueI("BUILDINGS") )
		{
			case 0: buildings[bdOn]->setMode( POLY_EDIT_MODE_ADD_PTS ); break;
			case 1: buildings[bdOn]->setMode( POLY_EDIT_MODE_MOVE_PTS ); break;
			case 2: buildings[bdOn]->setMode( POLY_EDIT_MODE_MOVE_ALL ); break;
			case 3: buildings[bdOn]->setMode( POLY_EDIT_MODE_ROTATE ); break;
		}*/
		
				
		if( buildingGroup.buildings.size() >  buildingGroup.selectedId )
		{
			if( lastSelectedBuilding == buildingGroup.selectedId ) 
				buildingGroup.buildings[buildingGroup.selectedId]->nFloors = panel.getValueI("NUM_FLOORS");
			else
				panel.setValueI("NUM_FLOORS",buildingGroup.buildings[buildingGroup.selectedId]->nFloors);
		}
		
	}
	

}


void userMapScene::saveTiles(string dir)
{
	if(userdir == "none" ) return;
	for( int i = 0; i < googleLoader.getNumLoaded(); i++)
	{
		if( bSetUserName) dir = userdir;
		else dir = "tiles";
		cout << "saving tile " << dir << "tile_" << ofToString(i) << ".jpg" << endl;
		googleLoader.tiles[i].saveImage( dir+"tile_"+ofToString(i)+".jpg");
	}
	
	bSavedTiles = true;
}

void userMapScene::saveUserFile()
{
	if( bSetUserName )
	{
		
	ofxXmlSettings xml;
	xml.addTag("ensanche");
	xml.pushTag("ensanche");
		
		// record user
		xml.addTag("client");
		xml.pushTag("client");
			xml.setValue("username", username);
		xml.popTag();
		
		xml.addTag("exterior");
		xml.pushTag("exterior");
			
			xml.addTag("representation");
			xml.pushTag("representation");
				// map
				xml.addTag("map");
				xml.pushTag("map");
					xml.setValue("position:x",mapPoly.getPt(0).x);
					xml.setValue("position:y",mapPoly.getPt(0).y);
					xml.setValue("rotation", mapPoly.rotation);
				xml.popTag();
				
				// buildings
				xml.addTag("barrio");
				xml.pushTag("barrio");
					for( int i = 0; i < buildingGroup.buildings.size(); i++)
					{
						xml.addTag("building");
						xml.setAttribute("building","bClosed",buildingGroup.buildings[i]->bClosed,i);
						xml.setAttribute("building","type",buildingGroup.buildings[i]->type,i);
						xml.setAttribute("building","floors",buildingGroup.buildings[i]->nFloors,i);
						xml.setAttribute("building","uid",buildingGroup.buildings[i]->uid,i);
						xml.pushTag("building",i);
						for(int j = 0; j < buildingGroup.buildings[i]->pts.size(); j++)
						{
							xml.addTag("pt");
							xml.setAttribute("pt","x",buildingGroup.buildings[i]->pts[j].x,j);
							xml.setAttribute("pt","y",buildingGroup.buildings[i]->pts[j].y,j);
						}
						xml.popTag(); //building
					}
				xml.popTag(); //barrio
		
		
			xml.popTag(); // representatio
			
			//------------------ set architecture part
			xml.addTag("architecture");
			xml.pushTag("architecture");
			int tSaved = 0;
			
			EnsancheBarrio barrio;
			barrio.addBuilding(&buildingGroup, 1.f/GOOGLE_MAP_METERS_TO_PIXEL);
		
			for( int i = 0; i < barrio.buildings.size(); i++)
			{
				// only if closed
				if( buildingGroup.buildings[i]->bClosed )
				{
					// make translated, scaled version
					xml.addTag("building");
					xml.addAttribute("building","nFloors",barrio.buildings[i].nFloors,tSaved);
					xml.setAttribute("building","uid",buildingGroup.buildings[i]->uid,tSaved);
					
					xml.pushTag("building",tSaved);
					
						
							xml.addTag("floor");
							xml.pushTag("floor",0);
							for(int j = 0; j < barrio.buildings[i].walls.size(); j++)
							{
								xml.addTag("wall");
								xml.setAttribute("wall","uid",barrio.buildings[i].walls[j].uid, j);
								xml.pushTag("wall",j);
									xml.addTag("pt");
									xml.setAttribute("pt","x",barrio.buildings[i].walls[j].pts[0].x, 0);
									xml.setAttribute("pt","y",barrio.buildings[i].walls[j].pts[0].y, 0);
									xml.addTag("pt");
									xml.setAttribute("pt","x",barrio.buildings[i].walls[j].pts[1].x, 1);
									xml.setAttribute("pt","y",barrio.buildings[i].walls[j].pts[1].y, 1);
								xml.popTag();
							}
							xml.popTag();
						
					xml.popTag();
					tSaved++;
				}
			}
			
			xml.popTag(); // architecture
			
		xml.popTag(); //exterior
	
	xml.popTag(); //ensanche
	
	if(userdir == "none" || username == "none") 
		return;
	
	string file = USER_DIRECTORY+username+"/"+username+XML_FILE_MAP;
	xml.saveFile(file);
	
	string tDir = USER_DIRECTORY+username+"/";
		saveTiles(tDir);
	}
	
}

void userMapScene::loadTileFromFolder( string path)
{
	googleLoader.loadSavedTiles(path);
	bUsingGoogleMap = true;
}

void userMapScene::setUserName(string name)
{
	if( name.length() > 0 )
	{
		username = name;
		bSetUserName = true;
	}
}


void userMapScene::loadUserFile()
{
	cout << "load user file " << endl;
	
	if( bSetUserName )
	{
		string file = USER_DIRECTORY+username+"/"+username+XML_FILE_MAP;
		loadTileFromFolder(USER_DIRECTORY+username+"/");
		
		ofxXmlSettings xml;
		
		cout << file << endl;
		xml.loadFile(file);
		
		if( xml.pushTag("ensanche") )
		{
			if( xml.pushTag("exterior") )
			{
				
				//------------------ set drawing part
				if( xml.pushTag("representation") )
				{
					if( xml.pushTag("map") )
					{
						float x	= xml.getValue("position:x",0.f);
						float y	= xml.getValue("position:y",0.f);
						mapPoly.clear();
						mapPoly.createFromRect( ofRectangle(x,y,GOOGLE_MAP_TILE_SIZE*GOOGLE_MAP_N_TILES_X,(GOOGLE_MAP_TILE_SIZE-GOOGLE_MAP_OFFSET)*GOOGLE_MAP_N_TILES_Y));
						mapPoly.rotation = xml.getValue("rotation", 0.f);
						
						xml.popTag(); // map
					}
					
					xml.setValue("scale", GOOGLE_MAP_METERS_TO_PIXEL);
					
					//
					if(xml.pushTag("barrio") )
					{
						int nBdgs = xml.getNumTags("building");
						
						for( int i = 0; i < buildingGroup.buildings.size(); i++)
							delete buildingGroup.buildings[i];
						
						buildingGroup.buildings.clear();
						
						for( int i = 0; i < nBdgs; i++)
						{
							buildingGroup.addBuilding();
							bool bClose = xml.getAttribute("building","bClosed",false,i);
							buildingGroup.buildings[i]->type	= xml.getAttribute("building","type",EN_BUILDING_NEIGHBOR,i);
							buildingGroup.buildings[i]->nFloors = xml.getAttribute("building","floors",6,i);
							buildingGroup.buildings[i]->uid = xml.getAttribute("building","uid",getUid(),i);
							
							xml.pushTag("building",i);
							
								int npts = xml.getNumTags("pt");
								for( int j = 0; j < npts; j++)
								{
									float x = xml.getAttribute("pt","x",0.f,j);
									float y = xml.getAttribute("pt","y",0.f,j);
									buildingGroup.buildings[i]->addPoint( ofPoint(x,y) );
								}
							
								if(bClose && npts > 0 ) buildingGroup.buildings[i]->addPoint( buildingGroup.buildings[i]->pts[0] ); 
							
							xml.popTag();
						}
						
						xml.popTag(); // barrio
					}
					
					xml.popTag(); // rep
				}
				

				
				
				xml.popTag(); // ext
			}
			xml.popTag(); // ensanche
		}
		
	}
}
