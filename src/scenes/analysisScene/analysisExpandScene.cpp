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
	
}

AnalysisExpandScene::~AnalysisExpandScene()
{
}

void AnalysisExpandScene::update()
{
	if(bPanelOn) updateControlPanel();
	
	/*
	if( panel.getSelectedPanelName() == "add scale" )
		scaleTool.reEnableLast();
	else
		scaleTool.disableAll(true);
		*/
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
	ofBackground(255,255,255);
	
	//grid
	ofSetColor(200,200,200,255);
	if(panel.getValueB("GRID")) enDrawGrid(10,10);
	
	ofRectangle boundingBox = barrioOriginal.getGroupBoundingBox();
	scaleTool.setScale( panel.getValueF("ZOOM"));
	ofPoint transPreRotate = ofPoint( ofGetWidth()*.5,ofGetHeight()*.5);
	ofPoint transPstRotate = ofPoint(0,0);//-boundingBox.width,-boundingBox.height,0);
	
	scaleTool.setOffset(transPreRotate,transPstRotate);

	// 2d model
	glPushMatrix();
		
		glTranslatef(ofGetWidth()*.5,ofGetHeight()*.5,0);
		
		float zoom = panel.getValueF("ZOOM");
		glScalef(zoom,zoom,1);
	
		glPushMatrix();
			glTranslatef(-boundingBox.width*.5,-boundingBox.height*.5,0);
			ofFill();
			ofSetColor(235,235,235,220);
			barrioOriginal.draw();
			
			ofNoFill();
			ofSetColor(0,0,0,255);
			barrioOriginal.draw();
		
		
			ofNoFill();
			ofSetColor(255,0,0,200);
			//minRectExpander.draw();
			
			if( panel.getSelectedPanelName() == "find space" ) 
			{
			   glLineWidth(3.0);
			   minRectExpander.drawExpandAreas();
			   if( panel.getValueB("SHOW_INTERSECT") ) minRectExpander.drawIntersections( panel.getValueI("SHOW_WHICH_B") - 1 );
			   if( panel.getValueB("SHOW_SHORTEST")  )	minRectExpander.drawShortest( panel.getValueI("SHOW_WHICH_B") - 1  );
			   glLineWidth(1.0);
			
			}else if( panel.getSelectedPanelName() == "find facade sides" ){
			   sideAssigner.draw();
			}else if( panel.getSelectedPanelName() == "make new facade" )
			{
				expander.draw(zoom);
			}
			   
		glPopMatrix();
		
		facadeMakerBox.draw();
		
		scaleTool.draw();
		
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
	
	
	if(key=='K')
	{
		if(barrioOriginal.buildings.size() > 0 ) sideAssigner.assignSides(&barrioOriginal.buildings[0]);
		if(barrioOriginal.buildings.size() > 0 && minRectExpander.expanders.size() > 0 ) 
			sideAssigner.assignSidesQuadrants(&barrioOriginal.buildings[0], minRectExpander.expanders[0]);
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

	panel.update();
	
	
}

void AnalysisExpandScene::updateControlPanel()
{
	
		
	panel.update();
	
	if( panel.getValueB("EXPAND") )
	{
		panel.setValueB("EXPAND",false);
		minRectExpander.expand(.1);
	}
	
	if(panel.bNewPanelSelected)
	{
		panel.bNewPanelSelected = false;
		
		// FIND SIDES HERE
		// if we switch to side panel, find sides
		if(panel.getSelectedPanelName() == "find facade sides" )
		{
			for( int i = 0; i < barrioOriginal.buildings.size(); i++)
			{
				if(minRectExpander.expanders.size() > i ) 
					sideAssigner.assignSidesQuadrants(&barrioOriginal.buildings[i], minRectExpander.expanders[i]);
			}	
		}
		
		else if(panel.getSelectedPanelName() == "make new facade" )
		{
			expander.clear();
			for( int i = 0; i < barrioOriginal.buildings.size(); i++)
			{
				EnsancheBuilding dstBuilding;
				if(barrioOriginal.buildings.size() > i && minRectExpander.expanders.size() > i  ) 
				{
					barrioOriginal.buildings[i].buildingPoly.setWinding(WIND_CLOCKWISE);
					sideAssigner.assignSidesQuadrants(&barrioOriginal.buildings[i], minRectExpander.expanders[i]);
					for( int j = 0; j < 4; j++)	
						expander.expand(barrioOriginal.buildings[i],dstBuilding,j,sideAssigner.sideIds,minRectExpander.expanders[i]);
				}
				
			}
		}
		
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
	 
	 // calc area so we can sort them
	 for( int i = 0; i < barrioOriginal.buildings.size(); i++)
		 barrioOriginal.buildings[i].buildingPoly.setArea();
		 
		std::sort(barrioOriginal.buildings.begin(),barrioOriginal.buildings.end(),sort_parea_compare);
	
		minRectExpander.setup(barrioOriginal);
		/*
		for( int i = 0; i < barrioOriginal.buildings.size(); i++)
		 dilator.copyBuildingToImage(barrioOriginal.buildings[i], i);
		
		dilator.doAnalysis();
		*/
	
		//panel.removeToggle();
		
		int whichB = panel.getValueI("SHOW_WHICH_B");
		
		vector<string> strSelectBs;
		strSelectBs.push_back("show all");
		for( int i = 0; i < barrioOriginal.buildings.size(); i++)
			strSelectBs.push_back("show "+ofToString(i));
		
		multiWhichB->setup("SHOW_WHICH_B", whichB, strSelectBs);
		multiWhichB->value.setValue(whichB,0,strSelectBs.size()-1);
		
		//multiWhichB->setup("show which", whichB, strSelectBs);
		multiWhichB->setDimensions(180, 17*strSelectBs.size());
		multiWhichB->xmlName = "SHOW_WHICH_B";
		//panel.addMultiToggle("show which", "SHOW_WHICH_B", 0, strSelectBs);
		
		
	}
	
}
