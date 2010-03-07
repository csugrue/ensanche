/*
 *  analysisExpandScene.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/5/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "analysisExpandScene.h"


void AnalysisExpandScene::setup()
{
	statusMessage	= "none";
	bPanelOn		= false;
	bSetUserName	= false;
	
	modelRoom.setup();
	modelRoom.enable();
	
	setupControlPanel();
	
}

AnalysisExpandScene::~AnalysisExpandScene()
{
}

void AnalysisExpandScene::update()
{
	if(bPanelOn) updateControlPanel();
}


void AnalysisExpandScene::preDraw()
{
}

void AnalysisExpandScene::draw()
{
	glPushMatrix();
		
		glTranslatef(10,400,0);
		glScalef(4,4,1);
		
		barrioOriginal.draw();
		
	
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(ofGetWidth()/2.f,ofGetHeight()/2.f,0);
		glScalef(4,4,4);
		modelRoom.draw();
	glPopMatrix();
	
	if(bPanelOn) panel.draw();
}

void AnalysisExpandScene::keyPressed(int key)
{
	
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
}

void AnalysisExpandScene::mousePressed(int x, int y, int button)
{
	if(bPanelOn) panel.mousePressed(x,y,button);
	
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
	
//	panel.setup("expand", 250, 10, 530, 270);
	//panel.update();
	
	
}

void AnalysisExpandScene::updateControlPanel()
{
	
	panel.update();
	
	
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
		
		if( barrioOriginal.buildings.size() > 0 )
			modelRoom.setFromBuilding( barrioOriginal.buildings[0], 4.f);
	}
	
}
