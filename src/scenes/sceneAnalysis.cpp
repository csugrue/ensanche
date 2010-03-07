/*
 *  sceneAnalysis.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/5/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "sceneAnalysis.h"

void SceneAnalysis::setup()
{
	statusMessage	= "none";
	bPanelOn		= true;
	
	setupControlPanel();
	
	// set up sub scenes
	analysisScenes[ ANALYSIS_SUB_EXPAND ] = new AnalysisExpandScene();
	
	for( int i = 0; i < ANALYSIS_NUM_SUB_SCENES; i++)
		analysisScenes[i]->setup();
	
	currentSubScene = -1;
	lastPanelOn = panel.getSelectedPanelName();
	
}

void SceneAnalysis::startScene()
{
	// load current user data in case there were any changes
	cout << "start analysis scene " << endl;
	analysisScenes[ANALYSIS_SUB_EXPAND]->loadUserFile();
	
}

void SceneAnalysis::setUserName(string name)
{
	baseScene::setUserName(name);
	for( int i = 0; i < ANALYSIS_NUM_SUB_SCENES; i++)
		analysisScenes[i]->setUserName(name); 
}

void SceneAnalysis::update()
{
	if(bPanelOn) updateControlPanel();
	if( currentSubScene >= 0 ) analysisScenes[currentSubScene]->update();
	
	currentSubScene = getSubSceneId( panel.getSelectedPanelName() );
	
}

void SceneAnalysis::resetScene()
{
}

void SceneAnalysis::preDraw()
{
}

void SceneAnalysis::draw()
{
	ofBackground(200,200,200);
	if( currentSubScene >= 0 ) analysisScenes[currentSubScene]->draw();
	if(bPanelOn) panel.draw();
}

void SceneAnalysis::keyPressed(int key)
{
	if( panel.isAnyTextBoxActive() ){
		if( key == OF_KEY_BACKSPACE) panel.deleteLastChar();
		else{
			panel.addChar(key);
		}
		return;
	}
	
	if( currentSubScene >= 0 ) analysisScenes[currentSubScene]->keyPressed(key);
	if( currentSubScene >= 0 &&  !analysisScenes[currentSubScene]->panel.isAnyTextBoxActive() ){
		;
	}
}

void SceneAnalysis::keyReleased(int key)
{
	if( currentSubScene >= 0 ) analysisScenes[currentSubScene]->keyReleased(key);
}

void SceneAnalysis::mouseMoved(int x, int y)
{
	if(bPanelOn && panel.isMouseInPanel(x,y) )
	{
		for( int i = 0; i < ANALYSIS_NUM_SUB_SCENES; i++)
			analysisScenes[i]->disable();
	}else{
		
		if( currentSubScene >= 0 ) analysisScenes[currentSubScene]->mouseMoved(x,y);
	}
}

void SceneAnalysis::mouseDragged(int x, int y, int button)
{
	if(bPanelOn) panel.mouseDragged(x,y,button);
	
	if(bPanelOn && panel.isMouseInPanel(x,y) )
	{
		for( int i = 0; i < ANALYSIS_NUM_SUB_SCENES; i++)
			analysisScenes[i]->disable();
	}else{
		
		if( currentSubScene >= 0 ) analysisScenes[currentSubScene]->mouseDragged(x,y,button);
	}
}

void SceneAnalysis::mousePressed(int x, int y, int button)
{
	
	if(bPanelOn) panel.mousePressed(x,y,button);
	
	if(bPanelOn && panel.isMouseInPanel(x,y) )
	{
		for( int i = 0; i < ANALYSIS_NUM_SUB_SCENES; i++)
			analysisScenes[i]->disable();
	}
	
	
	if( currentSubScene >= 0 ) analysisScenes[currentSubScene]->mousePressed(x,y,button);
	
}

void SceneAnalysis::mouseReleased(int x, int y, int button)
{
	if(bPanelOn) panel.mouseReleased();
	if( currentSubScene >= 0 ) analysisScenes[currentSubScene]->mouseReleased(x,y,button);

}

string	SceneAnalysis::getStatusMessage()
{
}

void SceneAnalysis::setupControlPanel()
{
	panel.setup("analysis", 250, 10, 530, 400);
	
	panel.addPanel("expand", 1, false);
	panel.addPanel("3d model", 1, false);
	
	panel.update();
}

void SceneAnalysis::updateControlPanel()
{
	panel.update();
}


int  SceneAnalysis::getSubSceneId( string panelName)
{
	if(			panelName == "expand")	return ANALYSIS_SUB_EXPAND;
	//else if (	panelName == "3d model")	return USER_SUB_INT;
	else		return -1;
	
}

