#include "userInteriorScene.h"


void userInteriorScene::setup()
{
	statusMessage	= "none";
	bPanelOn		= false;
	bFloorPlanLoaded = false;
	bSetUserName	 = false;
	username		= "";
	fpFileName = "";
	
	// set up structures for input
	fpLimits.setup();
	fpLimits.setMode( POLY_EDIT_MODE_ADD_PTS );
	
	pilars.setup();
	pilars.setWidth(4);
	pilars.setFillColor(150,150,150,200);

	
	windows.setup();
	windows.setWidth(6);
	windows.setFillColor(255,255,255,200);
	
	rmLabels.setup();
	
	scaleTool.setup();
	
	setupControlPanel();
	
	// do this after setup control panel so labels set
	rmLabels.setLabel( room_labels[0], 0);
	
	lastSaveTime = ofGetElapsedTimef();
	
	// NOTE: temporary
	enFloorPlan.setup();
}

userInteriorScene::~userInteriorScene()
{
}

void userInteriorScene::resetScene()
{
	bFloorPlanLoaded	= false;
	bPanelOn			= false;
	statusMessage		= "none";
	
	// NOTE: must clear structures
	
}

void userInteriorScene::update()
{
	if(bPanelOn) updateControlPanel();
	
	//--- disable or enable floor plan poly
	if( panel.getSelectedPanelName() == "adjust scan" )		fpPoly.enable();
	else													fpPoly.disable();
	
	//--- disable or enable the various floor plan structures depening on the current selection
	if( panel.getSelectedPanelName() == "add structure" ){
		
		if( panel.getValueI("piso_structure") == 0 ) fpLimits.enable();
		else fpLimits.disable();
		
		if( panel.getValueI("piso_structure") == 1 ) pilars.reEnableLast();
		else pilars.disableAll(true);
		
		if( panel.getValueI("piso_structure") == 2 ) windows.reEnableLast();
		else windows.disableAll(true);
		
	}
	else
	{
		fpLimits.disable();
		pilars.disableAll(true);
		windows.disableAll(true);
	}
	
	if( panel.getSelectedPanelName() == "add layout data" )
	{
		rmLabels.reEnableLast();
	}else{
		rmLabels.disableAll(true);
	}
	   
	if( panel.getSelectedPanelName() == "add scale" )
		scaleTool.reEnableLast();
	else
		scaleTool.disableAll(true);
	
	
	float dt = ofGetElapsedTimef() - lastSaveTime;
	if( dt > AUTO_SAVE_TIME){
		//saveUserFile();
		lastSaveTime = ofGetElapsedTimef();
	}
	
	//NOTE: temporary
	//enFloorPlan.addWalls(fpLimits,panel.getValueF("pixel_per_meter"));
}

void userInteriorScene::preDraw()
{
}

void userInteriorScene::draw()
{
	ofBackground(255,255,255);
	ofEnableAlphaBlending();
	
	//--- calc translations
	ofPoint transPreRotate = ofPoint( ofGetWidth()*.5+fpPoly.getPt(0).x,ofGetHeight()*.5 + fpPoly.getPt(0).y);
	ofPoint transPstRotate = ofPoint( -imgFloorPlan.width*.5,-imgFloorPlan.height*.5 );
	
	float scale = panel.getValueF("ZOOM");
	
	fpLimits.setOffset(transPreRotate,transPstRotate);
	fpLimits.setGRotation(fpPoly.rotation);
	fpLimits.setScale(scale);
	
	pilars.setOffset(transPreRotate,transPstRotate);
	pilars.setGRotation(fpPoly.rotation);
	pilars.setScale(scale);
	
	windows.setOffset(transPreRotate,transPstRotate);
	windows.setGRotation(fpPoly.rotation);
	windows.setScale(scale);
	
	rmLabels.setOffset(transPreRotate,transPstRotate);
	rmLabels.setGRotation(fpPoly.rotation);
	rmLabels.setScale(scale);
	
	scaleTool.setOffset(transPreRotate,transPstRotate);
	scaleTool.setGRotation(fpPoly.rotation);
	scaleTool.setScale(scale);
	
	glPushMatrix();
		
		glTranslatef(transPreRotate.x,transPreRotate.y,0);
		glRotatef(fpPoly.rotation,0,0,1);
		
		glTranslatef(transPstRotate.x,transPstRotate.y,0);
		glScalef(scale,scale,1);
	
		float shade = 255 * panel.getValueF("SHADE");
		ofSetColor(255,255,255,shade);
		imgFloorPlan.draw(0,0);
		
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(transPreRotate.x,transPreRotate.y,0);
		glRotatef(fpPoly.rotation,0,0,1);
		glTranslatef(transPstRotate.x,transPstRotate.y,0);
	
		glScalef(scale,scale,1);
	
		ofNoFill();
		ofSetColor(255,0,0);
		
		fpLimits.draw();
		
		pilars.draw();
	
		//NOTE: move to own func??
		calcWindowsToWalls();
		for( int i = 0; i < windows.polyFWs.size(); i++)
		{
			ofPoint a = windows.polyFWs[i]->getCentroid();
			ofPoint b = closeWindowWall[i].closePt;
			ofLine(a.x,a.y,b.x,b.y);
			
		}
		
		windows.draw();
		
		rmLabels.draw();
	
		scaleTool.draw();
	
	
	glPopMatrix();
	
	enFloorPlan.draw();
	
	if( panel.getValueB("GRID") )
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(200,200,200,100);
		
		enDrawGrid(10,10);
		
		ofPopStyle();
	}
	
	
	if(bPanelOn) panel.draw();
}

void userInteriorScene::keyPressed(int key)
{
	if( panel.isAnyTextBoxActive() ){
		if( key == OF_KEY_BACKSPACE) panel.deleteLastChar();
		else{
			panel.addChar(key);
		}
		return;
	}
	
	if( panel.getSelectedPanelName() == "add scale" )
	{
		float scaleVal = panel.getValueF("pixel_per_meter");
		if( key == '+' )
		{
			panel.setValueF("pixel_per_meter",scaleVal+.0001);
		}else if( key == '-' )
		{
			panel.setValueF("pixel_per_meter",scaleVal-.0001);
		}
	}
	
}

void userInteriorScene::keyReleased(int key)
{

}

void userInteriorScene::mouseMoved(int x, int y)
{

}

void userInteriorScene::mouseDragged(int x, int y, int button)
{
	if(bPanelOn) panel.mouseDragged(x,y,button);
	
	if( panel.isMouseInPanel( x, y) ){
		fpPoly.disable();
		fpLimits.disable();
		pilars.disableAll(true);
		windows.disableAll(true);
		rmLabels.disableAll(true);
		scaleTool.disableAll(true);
		
		if( panel.getSelectedPanelName() == "add structure" && panel.getValueI("piso_structure") == 1 ) 
			pilars.setWidth( panel.getValueF("pilar_width") );

	}

}

void userInteriorScene::mousePressed(int x, int y, int button)
{
	
	if(bPanelOn) panel.mousePressed(x,y,button);
	
	if( panel.isMouseInPanel( x, y) ){
		fpPoly.disable();
		fpLimits.disable();
		pilars.disableAll(true);
		windows.disableAll(true);
		rmLabels.disableAll(true);
		scaleTool.disableAll(true);
		
		// if select in control panel, change label
		if( panel.getSelectedPanelName() == "add layout data" )
		{
			int id = panel.getValueI("labels");
			rmLabels.setLabel( room_labels[id], id);
		}
		
	}else{
		
		if( panel.getSelectedPanelName() == "add structure"  && panel.getValueI("piso_structure") == 1 ) 
			panel.setValueF("pilar_width", pilars.cFWidth);
		
		if( panel.getSelectedPanelName() == "add layout data" )
		{
			//int id = rmLabels.getSelectedId();
			//panel.setValueI("labels",id);
		}
	}
	
	
}

void userInteriorScene::mouseReleased(int x, int y, int button)
{
	if(bPanelOn) panel.mouseReleased();

}

string userInteriorScene::getStatusMessage()
{
	return statusMessage;
}

void userInteriorScene::setupControlPanel()
{
	
	panel.setup("interior", ofGetWidth()-260, ofGetHeight()-450, 240, 370);
	panel.addPanel("adjust scan",1, false);
	panel.addPanel("add structure",1,false);
	panel.addPanel("add layout data",1,false);
	panel.addPanel("add scale",1,false);
	
	panel.setWhichPanel("adjust scan");
	panel.addSlider("zoom","ZOOM",1,1,10,false);
	panel.addSlider("shade","SHADE",1,0,1,false);
	panel.addToggle("reset rotation", "RESET_ROTATION", false);
	panel.addToggle("grid", "GRID", true);
	
	panel.setWhichPanel("add structure");
	vector<string> piso_labels;
	piso_labels.push_back("limits");
	piso_labels.push_back("pilars");
	piso_labels.push_back("windows");
	panel.addMultiToggle("piso structure","piso_structure", 0, piso_labels);
	panel.addSlider("pilar width","pilar_width",4,1,100,false);
	panel.addToggle("toggle square", "MAKE_SQUARE", false);
	panel.addToggle("set straight", "SET_STRAIGHT", false);
	panel.addToggle("align windows to walls", "ALIGN_WINDOWS", false);
	
	panel.setWhichPanel("add layout data");
	room_labels.push_back( subsChars("salón") );
	room_labels.push_back("cocina");
	room_labels.push_back("comedor");
	room_labels.push_back( subsChars("baño") );
	room_labels.push_back("dormitorio");
	room_labels.push_back("terraza");
	room_labels.push_back("entrada");
	room_labels.push_back("agua");
	
	panel.addMultiToggle("labels","labels", 0, room_labels);
	
	panel.setWhichPanel("add scale");
	panel.addSlider("pixel per meter","pixel_per_meter",.1,0,1,false);

	panel.update();
	
	
}

void userInteriorScene::updateControlPanel()
{

	panel.update();
	
	if( panel.getSelectedPanelName() == "add structure" )
	{
		if( panel.getValueB("MAKE_SQUARE") && panel.getValueI("piso_structure") == 1 )
		{
			panel.setValueB("MAKE_SQUARE",false);
			pilars.toggleSquare();
		}
		
		if( panel.getValueB("SET_STRAIGHT")  )
		{
			panel.setValueB("SET_STRAIGHT",false);
			if(panel.getValueI("piso_structure") == 1) pilars.setStraight();
			else if(panel.getValueI("piso_structure") == 2) windows.setStraight();
		} 
		
		if( panel.getValueB("ALIGN_WINDOWS") && panel.getValueI("piso_structure") == 2 )
		{
			panel.setValueB("ALIGN_WINDOWS",false);
			alignWindowsToWalls();
		}
		
	}
	
	if( panel.getSelectedPanelName() == "adjust scan" )
	{
		if( panel.getValueB("RESET_ROTATION") )
		{
			panel.setValueB("RESET_ROTATION",false);
			fpPoly.setGRotation(0);
		}
	}
	
	if(  panel.getSelectedPanelName() == "add scale" )
	{
		scaleTool.setPixelPerMeter( panel.getValueF("pixel_per_meter") );
	}
	
}


void userInteriorScene::loadFloorPlanImage( string filename )
{
	if( imgFloorPlan.loadImage(filename) )
	{
		fpPoly.setup();
		fpPoly.createFromRect( ofRectangle(0,0,imgFloorPlan.width,imgFloorPlan.height) );
		fpPoly.setMode(POLY_EDIT_MODE_MOVE_ALL);
		fpPoly.bUseKeyPress = false;
		fpPoly.bUseRotation = true;
		fpPoly.enable();
		
		bPanelOn = true;
		bFloorPlanLoaded = true;
		
		fpFileName = filename;
	}
}

void userInteriorScene::calcWindowsToWalls()
{
	
	if( fpLimits.pts.size() <= 0 ) return;
	if( windows.polyFWs.size() <= 0 ) return;
	
	closeWindowWall.clear();
	
	vector<int> wall_orientations;
	
	// get bounding of limits
	ofRectangle bb = fpLimits.getBoundingBox();
	float pplen = bb.width * 2.f;					// use to calc perpendiculars of windows
		
	// make a poly with last point included
	polySimple poly;
	for( int j = 0; j < fpLimits.pts.size(); j++)
	{
		poly.pushVertex(fpLimits.pts[j]);
	}
	poly.pushVertex(fpLimits.pts[0]);
	
	// get orientations of each wall setting if it is more horizontal or vertical
	for( int j = 0; j < poly.pts.size()-1; j++)
	{
		float	xDiff = fabs(poly.pts[j].x-poly.pts[j+1].x);
		float	yDiff = fabs(poly.pts[j].y-poly.pts[j+1].y);
		
		if (xDiff < yDiff)	wall_orientations.push_back(WALL_ORIENT_V);
		else				wall_orientations.push_back(WALL_ORIENT_H);

	}
	
	windowToWall tempWtW;
	tempWtW.closePt.set(0,0);
	tempWtW.wallindex = 0;
	tempWtW.closeDist = 0;
	
	for( int i = 0; i < windows.polyFWs.size(); i++)
	{
		
		closeWindowWall.push_back(tempWtW);// for debugging
		int tWtW = closeWindowWall.size()-1;
		
		// skip if not closed window
		if( windows.polyFWs[i]->pts.size() < 2 ) continue;
		
		// calulate perpendiculars for window
		windows.polyFWs[i]->setPerpendiculars(pplen);
		
		// vars for calclating closest wall
		int		closestWall = -1;
		float	closestDist =  0;
		ofPoint closePt = ofPoint(0,0);
		
		// determine orientation so we skip mismatch later
		float	xDiff = fabs(windows.polyFWs[i]->pts[0].x-windows.polyFWs[i]->pts[1].x);
		float	yDiff = fabs(windows.polyFWs[i]->pts[0].y-windows.polyFWs[i]->pts[1].y);
		int		orient = (xDiff < yDiff) ? WALL_ORIENT_V : WALL_ORIENT_H;
		
		ofPoint c = windows.polyFWs[i]->getCentroid();
		
		// loop thru all walls -- size()-1 because looking at pairs of points
		for( int j = 0; j < poly.pts.size()-1; j++)
		{
			
			// skip if orientations are opposite
			if( wall_orientations[j] != orient) continue;
			
			ofPoint iPt = ofPoint(0,0);
			
			// check intersection of perpendicular
			if( intersectionTwoLines( 
									 windows.polyFWs[i]->ppA, windows.polyFWs[i]->ppB, 
									 poly.pts[j], poly.pts[j+1], &iPt ) )
			{
				float distSq = ( (c.x-iPt.x)*(c.x-iPt.x) + (c.y-iPt.y)*(c.y-iPt.y) );
				
				if( closestWall == -1 || distSq < closestDist )
				{
					closestWall = j;
					closestDist = distSq;
					closePt.set(iPt.x,iPt.y);
					
				}
			}
			
			
		}
		
		closeWindowWall[ tWtW ].closeDist = closestDist;
		closeWindowWall[ tWtW ].wallindex = closestWall;
		closeWindowWall[ tWtW ].closePt.set(closePt.x,closePt.y);
	}
		
		
}

void userInteriorScene::alignWindowsToWalls()
{
	
	if( closeWindowWall.size() < windows.polyFWs.size() ) return;
	
	// get bounding of limits
	ofRectangle bb = fpLimits.getBoundingBox();
	float pplen = bb.width * 2.f;					// use to calc perpendiculars of windows
	
	// make a poly with last point included
	polySimple poly;
	for( int j = 0; j < fpLimits.pts.size(); j++)
	{
		poly.pushVertex(fpLimits.pts[j]);
	}
	poly.pushVertex(fpLimits.pts[0]);
	
		
	for( int i = 0; i < windows.polyFWs.size(); i++)
	{
		
		int wIndex = closeWindowWall[ i ].wallindex;
		float closeDist = closeWindowWall[ i ].closeDist;
		ofPoint closePt = closeWindowWall[ i ].closePt;
		
		if( wIndex != -1 && closeDist > 0.001 )
		{
			// calc slope of line
			ofxVec2f wallSlope = ofxVec2f(poly.pts[wIndex].x,poly.pts[wIndex].y)-ofxVec2f(poly.pts[wIndex+1].x,poly.pts[wIndex+1].y);
			wallSlope = wallSlope.getNormalized();
			
			// calc length
			ofxVec2f diffVec = ofxVec2f(windows.polyFWs[i]->pts[0].x,windows.polyFWs[i]->pts[0].y)-ofxVec2f(windows.polyFWs[i]->pts[1].x,windows.polyFWs[i]->pts[1].y);
			float hlen = .5 * diffVec.length();
			
			// set pts to positions on the wall
			windows.polyFWs[i]->pts[0].x = closePt.x - hlen * wallSlope.x;
			windows.polyFWs[i]->pts[0].y = closePt.y - hlen * wallSlope.y;
			windows.polyFWs[i]->pts[1].x = closePt.x + hlen * wallSlope.x;
			windows.polyFWs[i]->pts[1].y = closePt.y + hlen * wallSlope.y;
			
		}
		
	}
}


void userInteriorScene::setUserName(string name)
{
	if( name.length() > 0 )
	{
		username = name;
		bSetUserName = true;
	}
}

void userInteriorScene::disable()
{
	fpPoly.disable();
	fpLimits.disable();
	pilars.disableAll(true);
	windows.disableAll(true);
	rmLabels.disableAll(true);
	scaleTool.disableAll(true);
}

void userInteriorScene::loadUserFile()
{
	if( bSetUserName )
	{
		
		ofxXmlSettings xml;
		string myDir = USER_DIRECTORY + username + "/";
		string filename = myDir + username + XML_FILE_INT;
		xml.loadFile(filename);
		
		cout << "loading " << filename << endl;
		
		ofPoint boundingMin = ofPoint(0,0);
		
		if( xml.pushTag("ensanche") )
		{
			//--- get floorplan image and pos,rot etc
			if( xml.pushTag("floorplan") )
			{
				if( xml.pushTag("representation") )
				{
					fpFileName = xml.getValue("scanFilename",fpFileName);
					loadFloorPlanImage(fpFileName);
					
					panel.setValueF( "ZOOM", xml.getValue("zoom", 1.f) );
					
					float x	= xml.getValue("position:x",0.f);
					float y	= xml.getValue("position:y",0.f);
					fpPoly.createFromRect( ofRectangle(x,y,imgFloorPlan.width,imgFloorPlan.height) );
					fpPoly.rotation = xml.getValue("rotation", 0.f);
					
					panel.setValueF("pixel_per_meter", xml.getValue("scale",.1f) );
					
					boundingMin.x = xml.getValue("boundingBox:x", 0.f );
					boundingMin.y = xml.getValue("boundingBox:y", 0.f );
					
					bool bClosed = xml.getAttribute("fpLimits","bClosed",false,0);
					
					if( xml.pushTag("fpLimits") )
					{
						fpLimits.clear();
						int npts = xml.getNumTags("pt");
						
						for( int i = 0; i < npts; i++)
						{
							float x= xml.getAttribute("pt","x",0.f,i);
							float y= xml.getAttribute("pt","y",0.f,i);
							fpLimits.addPoint( ofPoint(x,y) );
						}
						
						if( npts > 0  && bClosed)
						{
							fpLimits.addPoint(fpLimits.pts[0]);
						}
						
						xml.popTag();
					}
					
					//pilars
					pilars.clear();
					int nPilars = xml.getNumTags("pilar");
					ofPoint m[2] = ofPoint(0,0);
					
					for( int i = 0; i < nPilars; i++)
					{
						float w = xml.getAttribute("pilar","width",4.f,i);
						xml.pushTag("pilar",i);
							
							m[0].x = xml.getAttribute("pt","x",0.f,0);
							m[0].y = xml.getAttribute("pt","y",0.f,0);
							m[1].x = xml.getAttribute("pt","x",0.f,1);
							m[1].y = xml.getAttribute("pt","y",0.f,1);
							
							pilars.addPoly(m[0],m[1],w);
						
						xml.popTag();
					}
					
					// windows
					windows.clear();
					int nWindows = xml.getNumTags("window");
					ofPoint wPt = ofPoint(0,0);
					
										
					for( int i = 0; i < nWindows; i++)
					{
						xml.pushTag("window",i);
						
						float w = xml.getAttribute("window","width",4.f,i);
						
						m[0].x = xml.getAttribute("pt","x",0.f,0);
						m[0].y = xml.getAttribute("pt","y",0.f,0);
						m[1].x = xml.getAttribute("pt","x",0.f,1);
						m[1].y = xml.getAttribute("pt","y",0.f,1);
												
						windows.addPoly(m[0],m[1],w);
						
						xml.popTag();
					}
					
					
					// labels
					
					
					xml.popTag(); //rep
				}
					
				xml.popTag(); //floorplan
			}
			
			xml.popTag(); //ensanche
		}
		
	}
}

void userInteriorScene::saveUserFile()
{
	//return;
	cout << "save file " << endl;
	
	if( bSetUserName )
	{
		string myDir = USER_DIRECTORY + username + "/";
		cout << "Saving interior data to: " << myDir << endl;
	
		EnsancheFloorPlan	enTempFloorPlan;
		enTempFloorPlan.addWalls(fpLimits, panel.getValueF("pixel_per_meter") );
		enTempFloorPlan.windows.clear();
		for(int i =0; i < windows.polyFWs.size(); i++)
			enTempFloorPlan.addWindow(windows.polyFWs[i], closeWindowWall[i],  panel.getValueF("pixel_per_meter")  );
		enTempFloorPlan.clearPilars();
		for(int i =0; i < pilars.polyFWs.size(); i++)
			enTempFloorPlan.addPilar(pilars.polyFWs[i], panel.getValueF("pixel_per_meter")  );
		
		ofxXmlSettings xml;
		
		xml.addTag("ensanche");
		xml.pushTag("ensanche");
		
			// record user
			xml.addTag("client");
			xml.pushTag("client");
				xml.setValue("username", username);
			xml.popTag();
		
			xml.addTag("floorplan");
			xml.pushTag("floorplan");
			
				// floorplan name, pos, rotation, translation, zoom
			
				xml.addTag("representation");
				xml.pushTag("representation");
		
					xml.setValue("zoom",panel.getValueF("ZOOM") );
					xml.setValue("rotation",fpLimits.gRotation );
					
					xml.setValue("position:x",fpPoly.getPt(0).x);
					xml.setValue("position:y",fpPoly.getPt(0).y);
		
					xml.setValue("scanFilename",fpFileName);
					xml.setValue("scale", panel.getValueF("pixel_per_meter") );
					
					ofRectangle boundingBox = fpLimits.getBoundingBox();
					xml.setValue("boundingBox:x", boundingBox.x );
					xml.setValue("boundingBox:y", boundingBox.y );
					xml.setValue("boundingBox:width", boundingBox.width );
					xml.setValue("boundingBox:height", boundingBox.height );
		
					xml.addTag("fpLimits");
					xml.setAttribute("fpLimits","bClosed",fpLimits.bClosed,0);
					xml.pushTag("fpLimits");
					for( int i = 0; i < fpLimits.pts.size(); i++)
					{
						xml.addTag("pt");
						xml.setAttribute("pt","x",(float)fpLimits.pts[i].x,i);
						xml.setAttribute("pt","y",(float)fpLimits.pts[i].y,i);
					}
					
					xml.popTag();
		
					// pilars
					for(int i =0; i < pilars.polyFWs.size(); i++)
					{
						xml.addTag("pilar");
						xml.setAttribute("pilar","width",pilars.polyFWs[i]->width,i);
						
						xml.pushTag("pilar",i);
						for(int j = 0; j < pilars.polyFWs[i]->pts.size(); j++)
						{
							xml.addTag("pt");
							xml.setAttribute("pt","x",pilars.polyFWs[i]->pts[j].x,j);
							xml.setAttribute("pt","y",pilars.polyFWs[i]->pts[j].y,j);
						}
						xml.popTag();
					}
					
					// windows
					for(int i =0; i < windows.polyFWs.size(); i++)
					{
						xml.addTag("window");
						xml.pushTag("window",i);
						for(int j = 0; j < windows.polyFWs[i]->pts.size(); j++)
						{
							xml.addTag("pt");
							xml.setAttribute("pt","x",windows.polyFWs[i]->pts[j].x,j);
							xml.setAttribute("pt","y",windows.polyFWs[i]->pts[j].y,j);
						}
						xml.popTag();
					}
			
					//room labels 
					for(int i =0; i < rmLabels.labels.size(); i++)
					{
						
						xml.addTag("label");
						if(rmLabels.labels[i]->pts.size()>0)
						{	
							xml.setAttribute("label","x",rmLabels.labels[i]->pts[0].x,i);
							xml.setAttribute("label","y",rmLabels.labels[i]->pts[0].y,i);
							xml.setAttribute("label","labelID",rmLabels.labels[i]->labelID,i);
						}
					}
		
		
		
				xml.popTag();
			
		
			// architectural elements
		
			xml.addTag("architecture");
			xml.pushTag("architecture");
				
			xml.addTag("room");
			xml.setAttribute("room","uid",getUid(),0);
			
			xml.pushTag("room");
			
			// each wall
			for( int i = 0; i < enTempFloorPlan.walls.size(); i++)
			{
				xml.addTag("wall");
				xml.setAttribute("wall","uid",(enTempFloorPlan.walls[i].uid),i);
				
				xml.pushTag("wall", i);
					
					xml.addTag("pt");
					xml.setAttribute("pt","x",(float)enTempFloorPlan.walls[i].pts[0].x,0);
					xml.setAttribute("pt","y",(float)enTempFloorPlan.walls[i].pts[0].y,0);
				
					xml.addTag("pt");
					xml.addAttribute("pt","x",enTempFloorPlan.walls[i].pts[1].x,1);
					xml.addAttribute("pt","y",enTempFloorPlan.walls[i].pts[1].y,1);
					
					// check for windows
					int nW = 0;
					for( int j = 0; j < closeWindowWall.size(); j++)
					{
						if( closeWindowWall[j].wallindex == i && enTempFloorPlan.windows.size() > j)
						{
							xml.addTag("window");
							xml.setAttribute("window","uid",enTempFloorPlan.windows[j].uid,nW);
							xml.pushTag("window",nW);
								xml.addTag("pt");
									xml.setAttribute("pt","x",enTempFloorPlan.windows[j].pts[0].x,0);
									xml.setAttribute("pt","y",enTempFloorPlan.windows[j].pts[0].y,0);
								xml.addTag("pt");
									xml.setAttribute("pt","x",enTempFloorPlan.windows[j].pts[1].x,1);
									xml.setAttribute("pt","y",enTempFloorPlan.windows[j].pts[1].y,1);
							xml.popTag();
							nW++;
						}
					}
					
				xml.popTag(); // wall
				
			}
			
		
			// pilars
			int nP = 0;
			for( int i = 0; i < pilars.polyFWs.size(); i++)
			{
				if( enTempFloorPlan.pilars.size() > i)
				{
					xml.addTag("pilar");
						xml.setAttribute("pilar","uid",enTempFloorPlan.pilars[i].uid,nP);
						xml.setAttribute("pilar","width",enTempFloorPlan.pilars[i].width,nP);
					xml.pushTag("pilar",nP);
					xml.addTag("pt");
						xml.setAttribute("pt","x",enTempFloorPlan.pilars[i].pts[0].x,0);
						xml.setAttribute("pt","y",enTempFloorPlan.pilars[i].pts[0].y,0);
					xml.addTag("pt");
						xml.setAttribute("pt","x",enTempFloorPlan.pilars[i].pts[1].x,1);
						xml.setAttribute("pt","y",enTempFloorPlan.pilars[i].pts[1].y,1);
					xml.popTag();
					nP++;
				}
			}
		
			xml.popTag(); // room
			
			xml.popTag(); // architecture
			
			// pilars
			// labels
			xml.popTag(); // floorplan
		
		xml.popTag(); // esanche
		
		string filename = myDir + username + XML_FILE_INT;
		xml.saveFile(filename);
	}
}


