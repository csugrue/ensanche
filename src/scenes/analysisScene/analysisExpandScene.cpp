/*
 *  analysisExpandScene.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/5/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "analysisExpandScene.h"

bool sort_parea_compare(  EnsancheBuilding  a,  EnsancheBuilding  b) {
	
	return (a.buildingPoly.getArea() > b.buildingPoly.getArea());
}


void AnalysisExpandScene::setup()
{
	statusMessage	= "none";
	bPanelOn		= true;
	bSetUserName	= false;
	bEnabled		= false;

	setupControlPanel();
	scaleTool.setup();
	
	currBuilding	= 0;
	prevBuilding	= 0;
	currSide		= 0;
	prevSide		= 0;
	prevFBuilding	= 0;
	
	for( int i = 0; i < MODEL_T_TEXTURES; i++)
	{
		ofImage mtxImg;
		mtxImg.loadImage("textures/wallTexture_"+ofToString(i)+".jpg");
		textureWalls[i].allocate(mtxImg.width,mtxImg.height,GL_RGB,false);
		textureWalls[i].loadData(mtxImg.getPixels(),mtxImg.width,mtxImg.height,GL_RGB);
		textureWalls[i].setTextureWrap(GL_REPEAT, GL_REPEAT);
	}
}

AnalysisExpandScene::~AnalysisExpandScene()
{
}

void AnalysisExpandScene::update()
{
	if(bPanelOn) updateControlPanel();
	
	if( panel.getValueB("SCALE") )
		scaleTool.reEnableLast();
	else
		scaleTool.disableAll(true);
	
}

void AnalysisExpandScene::disable()
{
	bEnabled = false;
}

void AnalysisExpandScene::enable()
{
	bEnabled = true;
}

void AnalysisExpandScene::preDraw()
{
}

void AnalysisExpandScene::draw()
{
	float zoom = panel.getValueF("ZOOM");
	
	ofBackground(255,255,255);
	ofEnableAlphaBlending();
	
	//grid
	ofSetColor(200,200,200,255);
	if(panel.getValueB("GRID")) enDrawGrid(10,10);
	
	ofRectangle boundingBox = barrioOriginal.getGroupBoundingBox();
	ofPoint transPreRotate = ofPoint( ofGetWidth()*.5,ofGetHeight()*.5);
	ofPoint transPstRotate = ofPoint(0,0);//-boundingBox.width,-boundingBox.height,0);
	
	scaleTool.setScale( zoom );
	scaleTool.setOffset(transPreRotate,transPstRotate);
	
	minRectExpander.scaleTool.setScale(zoom);
	minRectExpander.scaleTool.setOffset(transPreRotate,transPstRotate);
	
	// 2d model
	glPushMatrix();
		
		glTranslatef(ofGetWidth()*.5,ofGetHeight()*.5,0);
		
		
		glScalef(zoom,zoom,1);
	
		glPushMatrix();
			glTranslatef(-boundingBox.width*.5,-boundingBox.height*.5,0);
			ofFill();
			ofSetColor(235,235,235,220);
			//barrioOriginal.draw();
			
			ofNoFill();
			ofSetColor(0,0,0,255);
			//barrioOriginal.draw();
		
					
			if( panel.getSelectedPanelName() == "find space" ) 
			{
				// original 
				ofPushStyle();
				
					ofFill();
					ofSetColor(205,205,205,255*minRectExpander.gAlpha);
					minRectExpander.drawExpandAreas();
					
					ofFill();
					ofSetColor(255,255,255,220);
					barrioOriginal.draw();
					
					ofNoFill();
					ofSetColor(80,80,80,255);
					ofEnableSmoothing();
					glLineWidth(2);
					barrioOriginal.draw();
					ofDisableSmoothing();
				
					//glLineWidth(3.0);
					
					if( panel.getValueB("SHOW_INTERSECT") ) minRectExpander.drawIntersections( panel.getValueI("SHOW_WHICH_B") - 1 );
					if( panel.getValueB("SHOW_SHORTEST")  )	minRectExpander.drawShortest( panel.getValueI("SHOW_WHICH_B") - 1  );
					//glLineWidth(1.0);
					
				ofPopStyle();
			
			}else if( panel.getSelectedPanelName() == "find facade sides" )
			{
				for( int i = 0;  i < buildingDataEndPoints.size(); i++)
				{	
					buildingDataEndPoints[i].building.buildingPoly.draw();
					buildingDataEndPoints[i].drawEnds( zoom, &expander.font );
					buildingDataEndPoints[i].drawSideIds();
				}
				sideAssigner.draw();
				
			}else if( panel.getSelectedPanelName() == "find expansion"  )
			{
				minRectExpander.drawExpandAreas();
				minRectExpander.drawMinRectangles();
				ofNoFill();
				expander.polyInitExpand.draw();
				
				ofNoFill();
				for( int i = 0;  i < buildingDataExpanded.size(); i++)
				{	
					buildingDataExpanded[i].building.buildingPoly.draw();
					buildingDataExpanded[i].drawEnds( zoom, &expander.font );
					buildingDataExpanded[i].drawSideIds();
				}
			
			}else if( panel.getSelectedPanelName() == "make new facade" )
			{
				
				for( int i = 0; i < fBuildings.size(); i++)
				{
					fBuildings[i].draw2D(true,false);
				}
				
			}
			
			   
		glPopMatrix();
		
		
		
		scaleTool.draw();
		
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(30,ofGetHeight(),0);
		
		glScalef(zoom,zoom,1);
		
		if( panel.getSelectedPanelName() == "make new facade" )
		{
			int i=panel.getValueI("current_facade_b");
			if( fBuildings.size() > i ) fBuildings[i].drawAllFloors(true);
		}
		
	glPopMatrix();
	
	glPushMatrix();
		
		glTranslatef(ofGetWidth()*.5,ofGetHeight()*.5,0);
	
		glScalef(zoom,zoom,1);
	
		if( panel.getSelectedPanelName() == "make new facade" )
		{
			
			for( int i = 0; i < fBuildings.size(); i++)
			{
				//fBuildings[i].draw2D(true,true);
				
				glColor4f(.85,.85,.85,.8);
				glEnable(GL_DEPTH_TEST);
				fBuildings[i].draw3D2(false);
				glDisable(GL_DEPTH_TEST);
				
			}
						
		}
	glPopMatrix();
	
	// control panel
	if(bPanelOn) panel.draw();
}

void AnalysisExpandScene::keyPressed(int key)
{
	if(key=='k') //TEST FACADE
	{
		facadeMakerBox.getFacadeLine(ofPoint(0,0), ofPoint(18,0), ofPoint(0,1) );
	}
	
	
	
	
}

void AnalysisExpandScene::keyReleased(int key)
{
	
}

void AnalysisExpandScene::mouseMoved(int x, int y)
{
	
}

void AnalysisExpandScene::mouseDragged(int x, int y, int button)
{
	if(bPanelOn) panel.mouseDragged(x,y,button);
	if( panel.isMouseInPanel(x, y)  )
	{
		scaleTool.disableAll(true);
	}
}

void AnalysisExpandScene::mousePressed(int x, int y, int button)
{
	if(bPanelOn) panel.mousePressed(x,y,button);
	if( panel.isMouseInPanel(x, y) )
	{
		scaleTool.disableAll(true);
	}
	
}

void AnalysisExpandScene::mouseReleased(int x, int y, int button)
{
	if(bPanelOn) panel.mouseReleased();
	
}

string AnalysisExpandScene::getStatusMessage()
{
	return statusMessage;
}

void AnalysisExpandScene::setupControlPanel()
{
	
	panel.setup("expand", ofGetWidth()-260, ofGetHeight()-600, 240, 520);
	panel.addPanel("find space",1, false);
	panel.addPanel("find facade sides",1, false);
	panel.addPanel("find expansion",1, false);
	panel.addPanel("make new facade",1, false);

	panel.setWhichPanel("find space");
	panel.addToggle("grid","GRID",false);
	panel.addToggle("scale tool","SCALE",false);
	panel.addSlider("zoom","ZOOM",4.f,1.f,20.f,false);
	panel.addToggle("show intersections", "SHOW_INTERSECT", false);
	panel.addToggle("show shortest dist", "SHOW_SHORTEST", true);
	panel.addToggle("expand", "EXPAND", false);
	
	vector<string> strSelectBs;
	strSelectBs.push_back("show all");
	multiWhichB = panel.addMultiToggle("show which", "SHOW_WHICH_B", 0, strSelectBs);

	panel.setWhichPanel("find facade sides");
	guiChooseBuliding = panel.addSlider("current building","current_building",0,0,0,true);
	guiChooseSide = panel.addSlider("choose side","choose_side",0,0,0,true);
	panel.addSlider("set side id","set_side_id",0,0,3,true);
	//panel.addToggle("remove ends", "remove_ends", false);
	
	panel.setWhichPanel("find expansion");
	guiChooseExpandBuliding = panel.addSlider("current expand","current_expand",0,0,0,true);
	guiChooseExpandSide = panel.addSlider("choose expand side","choose_expand_side",0,0,3,true);
	panel.addToggle("remove ends", "remove_ends", false);
	//panel.addToggle("min rect expand", "min_rect_expand", false);
	panel.addSlider("min expand dist","min_expand_dist",1,0,10,false);
	panel.addSlider("min facade dist","min_facade_dist",1,0,10,false);
	panel.addToggle("min expand", "min_expand", false);
	
	panel.setWhichPanel("make new facade");
	guiChooseFacadeBuilding = panel.addSlider("building","current_facade_b",0,0,0,true);
	guiChooseFacadeFloor	= panel.addSlider("floor","current_facade_flr",0,0,0,true);
	panel.addSlider("side","current_facade_side",0,0,3,true);
	panel.addToggle("create new ripple","create_ripple",false);
	
	// choose type of facade
	// choose amount of ripple variation?

	panel.update();
	
	
}

static float lastTime = 0;

void AnalysisExpandScene::updateControlPanel()
{
	
	float dt = ofGetElapsedTimef()-lastTime;
	lastTime = ofGetElapsedTimef();
		
	panel.update();
	
	if( panel.getValueB("EXPAND") )
	{
		//panel.setValueB("EXPAND",false);
		// NOTE: this should be fixed rate not w/dt and have separate animator
		minRectExpander.expand(1.5*dt);
	}
	
	if(panel.bNewPanelSelected)
	{
		panel.bNewPanelSelected = false;
		
		if(panel.getSelectedPanelName() == "find facade sides" )
		{
				setupEndPointData();
						
		}else if(panel.getSelectedPanelName() == "find expansion" )
		{
				setupExpandData();
		
		}else if( panel.getSelectedPanelName() == "make new facade")
		{
				setUpModels();
				prevFBuilding = 0;
				if( barrioOriginal.buildings.size() > 0)
					guiChooseFacadeFloor->value.setValue(1,0,barrioOriginal.buildings[0].nFloors-1);
		}
		
	}
	
	if(panel.getSelectedPanelName() == "find facade sides" )
	{
		
		currBuilding = panel.getValueI("current_building");
		
		if( currBuilding != prevBuilding)
		{
			// new building so set slider to have num sides of new
			guiChooseSide->value.setValue(0,0,barrioOriginal.buildings[currBuilding].buildingPoly.pts.size()-1);
			
			// if new building is picked, change side to id of side 0 in new
			if(	buildingDataEndPoints.size() > currBuilding && 
				buildingDataEndPoints[currBuilding].sideIds.size() > 0) 
					panel.setValueI("set_side_id",buildingDataEndPoints[currBuilding].sideIds[0]);
			
			prevBuilding = currBuilding;
			currSide = 0; 
			prevSide = 0;
		
		}else{
			
			// if not new building, check if new side selected
			currSide = panel.getValueI("choose_side");
			
			if(currSide != prevSide)
			{
				// if it is a new side, reset slider data
				prevSide= currSide;
				if(buildingDataEndPoints.size() > currBuilding && buildingDataEndPoints[currBuilding].sideIds.size() > currSide) 
					panel.setValueI("set_side_id",buildingDataEndPoints[currBuilding].sideIds[currSide]);
			
			}else{
			
				// if not new side, set selected side id to current side id in slider and recalculate the end points
				if(	buildingDataEndPoints.size() > currBuilding && 
				    buildingDataEndPoints[currBuilding].sideIds.size() > currSide) 
				{
					int newSideValue =  panel.getValueI("set_side_id");
					if( newSideValue != buildingDataEndPoints[currBuilding].sideIds[currSide] )
					{
						buildingDataEndPoints[currBuilding].sideIds[currSide]= newSideValue;
						int i = currBuilding;
						
						expander.findAllEndPoints( buildingDataEndPoints[i].building,buildingDataEndPoints[i].sideIds,&buildingDataEndPoints[i].ends);
						
					} 
				}
			}
		}
		
	}
	
	
	
	if( panel.getValueB("remove_ends") )
	{
		panel.setValueB("remove_ends", false);
		
		int currExpandB = panel.getValueI("current_expand");
		if(buildingDataExpanded.size() > currExpandB) 
		{
			int sideToExpand = panel.getValueI("choose_expand_side");
			expander.removeInBetweenPoints(buildingDataExpanded[currExpandB].building, sideToExpand, &buildingDataExpanded[currExpandB].sideIds,&buildingDataExpanded[currExpandB].ends);
			expander.findAllEndPoints( buildingDataExpanded[currExpandB].building,buildingDataExpanded[currExpandB].sideIds,&buildingDataExpanded[currExpandB].ends);
		}
	}
	

	if( panel.getValueB("min_expand") )
	{
		panel.setValueB("min_expand", false);
		int currExpandB = panel.getValueI("current_expand");
		
		if(buildingDataExpanded.size() > currExpandB) 
		{
			int sideToExpand = panel.getValueI("choose_expand_side");
			expander.explandToMinimum(
									  buildingDataExpanded[currExpandB],sideToExpand, 
									  minRectExpander.expandersOriginal[currExpandB].poly, 
									  minRectExpander.expanders[currExpandB].poly, 
									  panel.getValueF("min_expand_dist"), 
									  panel.getValueF("min_facade_dist"));
		}
	}
	
	if( panel.getSelectedPanelName() == "make new facade" )
	{
		int currB = panel.getValueI("current_facade_b") ;
		if( prevFBuilding != currB)
		{
			prevFBuilding = currB;
			if( barrioOriginal.buildings.size() > currB)
				guiChooseFacadeFloor->value.setValue(1,0,barrioOriginal.buildings[currB].nFloors-1);
			
		}else if( panel.getValueB("create_ripple") ) {
			panel.setValueB("create_ripple",false);
			createFacadeLine( currB, panel.getValueI("current_facade_flr"), panel.getValueI("current_facade_side") );
		
			
		}
		
	}
	
}

void AnalysisExpandScene::findSideIdData()
{
	for( int i = 0; i < barrioOriginal.buildings.size(); i++)
	{
		if(minRectExpander.expanders.size() > i ) 
			sideAssigner.assignSidesQuadrants(&barrioOriginal.buildings[i], minRectExpander.expandersOriginal[i]);
	}	
}

void AnalysisExpandScene::setupExpandData()
{

	cout << "Setup expand data." << endl;
	buildingDataExpanded.clear();
	
	for( int i = 0; i < buildingDataEndPoints.size(); i++)
	{
		buildingDataExpanded.push_back( buildingDataEndPoints[i] );
	}

}

void AnalysisExpandScene::setupEndPointData()
{
	expander.clear();
	buildingDataEndPoints.clear();
	
	for( int i = 0; i < barrioOriginal.buildings.size(); i++)
	{
		// NOTE: need to also change winding of walls!!! 
		barrioOriginal.buildings[i].buildingPoly.setWinding(WIND_CLOCKWISE);
		
		buildingDataEndPoints.push_back( EnsancheExpandData() );
		buildingDataEndPoints[i].setup(barrioOriginal.buildings[i]);
		
		if(barrioOriginal.buildings.size() > i && minRectExpander.expanders.size() > i  ) 
		{
			sideAssigner.clear();
			sideAssigner.assignSidesQuadrants(&barrioOriginal.buildings[i], minRectExpander.expandersOriginal[i], &buildingDataEndPoints[i].sideIds);
			expander.findAllEndPoints(buildingDataEndPoints[i].building, buildingDataEndPoints[i].sideIds,&buildingDataEndPoints[i].ends);
			//for( int j = 0; j < 4; j++)	
			//	expander.expand(barrioOriginal.buildings[i],expandBuildingData[i].building,j,expandBuildingData[i].sideIds,minRectExpander.expanders[i],&expandBuildingData[i].ends);
		}
	}
	
}


void AnalysisExpandScene::setUpModels()
{
	cout << "Setup models " << endl;
	
	for( int i = 0; i < barrioOriginal.buildings.size(); i++)
	{
		//EnsancheModelBuilding model;
		//buildings.push_back(model);
		//buildings[i].setupFromBuilding(barrioOriginal.buildings[i]);
		
		
		fBuildings.push_back(fmodel);
		fBuildings[i].setup();
		fBuildings[i].setupFromBuilding(barrioOriginal.buildings[i]);
		
		fBuildings[ i ].generateModel(EN_FLOOR_HEIGHT);
	}
	
	for( int i = 0; i < buildingDataExpanded.size(); i++)
	{
		setModelInitialExpansion(i);
	}
	
	
	

	
	for( int i = 0; i < fBuildings.size(); i++){
		for( int j = 0; j < MODEL_T_TEXTURES; j++)
			fBuildings[i].setWallTexture(&textureWalls[j],j);
	}
}


void AnalysisExpandScene::setModelInitialExpansion(int index)
{
	/*if(buildings.size() > index && buildingDataExpanded.size() > index)
	{
		cout << "expanded floors " << index << " : " << buildings[index].nFloors << "floors" << endl;
		for( int i = 1; i < buildings[index].nFloors; i++)
		{
			cout << "set expanded floors " << i << endl;
			buildings[index].setFloor( buildingDataExpanded[index].building, i);
		}
	}*/
	
	if(fBuildings.size() > index && buildingDataExpanded.size() > index)
	{
		cout << "f expanded floors " << index << " : " << fBuildings[index].nFloors << "floors" << endl;
		for( int i = 1; i < fBuildings[index].nFloors; i++)
		{
			cout << "f set expanded floors " << i << endl;
			fBuildings[index].setFloor( buildingDataExpanded[index].building, i, buildingDataExpanded[index].sideIds, buildingDataExpanded[index].ends.sideToEndPt);
		}
	}
}


void AnalysisExpandScene::createFacadeLine( int ixBuilding, int ixFloor, int sideToAlter )
{
	ofPoint ptA, ptB;
	
	int st,nxt;
	
	// don't expand gorund floor
	if(ixFloor == 0 ) return;
	
	/*if( buildingDataExpanded.size() > ixBuilding )
	{
		for( int i = 0; i < buildingDataExpanded[ ixBuilding ].sideIds.size(); i++ )
		{
			if( buildingDataExpanded[ ixBuilding ].sideIds[i] == sideToAlter )
			{
				st = i;
				nxt = ( i==buildingDataExpanded[ ixBuilding ].sideIds.size()-1 ) ? 0 : i+1;
				ptA = buildingDataExpanded[ ixBuilding ].building.buildingPoly.pts[i];
				ptB = buildingDataExpanded[ ixBuilding ].building.buildingPoly.pts[nxt];
				break;
			}
			
		}
	}*/
	
	if( fBuildings.size() > ixBuilding && fBuildings[ ixBuilding ].floorData.size() > ixFloor )
	{
		for( int i = 0; i < fBuildings[ ixBuilding ].floorData[ixFloor].sideIds.size(); i++ )
		{
			if( fBuildings[ ixBuilding ].floorData[ixFloor].sideIds[i] == sideToAlter )
			{
				st = i;
				nxt = ( i==fBuildings[ ixBuilding ].floorData[ixFloor].sideIds.size()-1 ) ? 0 : i+1;
				ptA = fBuildings[ ixBuilding ].buildingFloors[ixFloor].pts[i];
				ptB = fBuildings[ ixBuilding ].buildingFloors[ixFloor].pts[nxt];
				break;
			}
			
		}
	}
	
	ofxVec2f pp = ofxVec2f(ptB.x-ptA.x,ptB.y-ptA.y);
	ofPoint ptPP = pp.perpendicular();
	
	vector <ofPoint> fLine;
	cout << "get facade line " << endl;
	fLine = facadeMakerBox.getFacadeLine( ptA,  ptB,  ptPP);
	
	if(fBuildings.size() > ixBuilding)
	{
		//int floorNum, int startPt, int endPt, int sideId );
		
		fBuildings[ ixBuilding ].insertFacadeLine(fLine, ixFloor, st, nxt, sideToAlter);
		//fBuildings[ ixBuilding ].generateModel(EN_FLOOR_HEIGHT);
	}
}

void AnalysisExpandScene::setUserName( string name )
{
	if( name.length() > 0 )
	{
		username = name;
		bSetUserName = true;
	}
}

void AnalysisExpandScene::loadUserFile()
{	
	if( bSetUserName )
	{
		// load model
		string dir = USER_DIRECTORY;
		string filename = dir+username+"/"+username+XML_FILE_MAP;
		
		cout << "---------------------" << endl;
		cout << "Analysis Expand Scene: loading " << filename << endl;
		
		barrioOriginal.loadFromXml(filename);
		cout << "---------------------" << endl;		
	}
	
	
	if( barrioOriginal.buildings.size() >  0 )
	{
	 
		guiChooseBuliding->value.setValue(0,0,barrioOriginal.buildings.size()-1);
		guiChooseSide->value.setValue(0,0,barrioOriginal.buildings[0].buildingPoly.pts.size()-1);
		guiChooseExpandBuliding->value.setValue(0,0,barrioOriginal.buildings.size()-1);
		guiChooseExpandSide->value.setValue(0,0,4);//barrioOriginal.buildings[0].buildingPoly.pts.size()-1);
		guiChooseFacadeBuilding->value.setValue(0,0,barrioOriginal.buildings.size()-1);
		
		// calc area so we can sort them
		for( int i = 0; i < barrioOriginal.buildings.size(); i++)
			barrioOriginal.buildings[i].buildingPoly.setArea();
		 
		std::sort(barrioOriginal.buildings.begin(),barrioOriginal.buildings.end(),sort_parea_compare);
	
		cout << "Analysis: set up expand area finder " << endl;
		minRectExpander.setup(barrioOriginal);
		
		int whichB = panel.getValueI("SHOW_WHICH_B");
		
		vector<string> strSelectBs;
		strSelectBs.push_back("show all");
		for( int i = 0; i < barrioOriginal.buildings.size(); i++)
			strSelectBs.push_back("show "+ofToString(i));
		
		multiWhichB->setup("SHOW_WHICH_B", whichB, strSelectBs);
		multiWhichB->value.setValue(whichB,0,strSelectBs.size()-1);
		
		multiWhichB->setDimensions(180, 17*strSelectBs.size());
		multiWhichB->xmlName = "SHOW_WHICH_B";		
		
	}
	
	cout << "Analysis: user data loaded." << endl;
	
}



void AnalysisExpandScene::saveUserFile()
{
	
	
	//ofxXmlSettings xml;
	//xml.loadFile(filename);
	
	if( bSetUserName )
	{
	
	// load model
	string dir = USER_DIRECTORY;
	string filename = dir+username+"/"+username+XML_FILE_FACADE;
		
	cout << "saving expanded facade data to " << filename << endl;
		
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
			
			xml.addTag("architecture");
			xml.pushTag("architecture");
			
				for( int i = 0; i < fBuildings.size(); i++)
				{
						
					xml.addTag("building");
					
					int nFloors = fBuildings[i].buildingFloors.size();//nFloors;
					xml.setAttribute("building","nFloors",nFloors,i);
					
					cout << "saving building " << i << " with " << nFloors << " floors" << endl;
					
					xml.pushTag("building", i);
						
						for( int f = 0; f < nFloors; f++)
						{
							//buildings[i].addBuildingFloor( barrio->buildings[i] );
							xml.addTag("floor");
							xml.pushTag("floor",f);
							
									int nWalls = fBuildings[i].buildingFloors[f].pts.size()-1;
									
									cout << "floor " << f << " saving " << nWalls << " walls " << endl;
									
									for( int j = 0; j < fBuildings[i].buildingFloors[f].pts.size()-1; j++)
									{
										ofPoint m1,m2;
										m1 = fBuildings[i].buildingFloors[f].pts[j];
										m2 = fBuildings[i].buildingFloors[f].pts[j+1];

										int sideId = 0;
										if(fBuildings[i].floorData.size() > f && fBuildings[i].floorData[f].sideIds.size() > j ) 
											sideId = fBuildings[i].floorData[f].sideIds[j];
										
										xml.addTag("wall");

										xml.setAttribute("wall","uid",getUid(),j);
										xml.setAttribute("wall","side",sideId,j);
										xml.pushTag("wall",j);
											xml.addTag("pt");
											xml.setAttribute("pt","x",m1.x,0);
											xml.setAttribute("pt","y",m1.y,0);
											xml.addTag("pt");
											xml.setAttribute("pt","x",m2.x,1);
											xml.setAttribute("pt","y",m2.y,1);
										xml.popTag();
										
									}
									
									//buildings[nB].addClosingWall();
							xml.popTag(); // floors
							
						}
						
					xml.popTag(); // building
						
				} // end i
					
			xml.popTag(); // arch
			
		xml.popTag(); //exterior
		
	xml.popTag(); //ensanche
	
	xml.saveFile(filename);
	
	} //end if user
	
	
}
