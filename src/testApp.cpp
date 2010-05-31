#include "testApp.h"
#include "uuid.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	bPanelOn = true;
	
	currentScene	= 0;
	numScenes		= 4;
	scenes			= new baseScene*[numScenes];
	
	scenes[0] = new baseScene();
	scenes[1] = new sceneUserInput();
	scenes[2] = new SceneAnalysis();
	scenes[3] = new baseScene();
	
	scenes[0]->myName = "start";		// empty start screen, select to start from scrathc or load
	scenes[1]->myName = "user";			// user input scene, google maps, drawing floor plans etc.
	scenes[2]->myName = "analysis";		// analysis of new space
	scenes[3]->myName = "stats";		// calc and show stats
	
	// setup master control panel to change scenes
	setupControlPanel();
	lastPanelName	= panel.getSelectedPanelName();
	
	// setup scenes
	for( int i = 0; i < numScenes; i++)
		scenes[i]->setup();
	
	// temp set which panel
	panel.setSelectedPanel("user");
	panel.setMinimized(true);
	
	
	// Note: temp movie saving stuff
	movieSaver.setup(640,480, "ensanche2.mov" );
	movieSaver.setCodecType( OF_QT_SAVER_CODEC_QUALITY_NORMAL );
	bRecordingMovie = false;
	bUseRecorder = false;
	bGrabScreen = false;
	screenGrabCounter = 0;
	screenGrabPath = "screenGrabs/"+getTimeString() + "_";
	
	ofSetFrameRate(30);

}
//--------------------------------------------------------------
testApp::~testApp()
{
	for( int i = 0; i < numScenes; i++)
		delete scenes[i];
}

//--------------------------------------------------------------
void testApp::update(){
	
		
	// update master contorls
	updateControlPanel();
	
	// update current scene
	scenes[currentScene]->update();
}


//--------------------------------------------------------------
void testApp::draw(){
	
	
	scenes[currentScene]->draw();
	if(bPanelOn) panel.draw();

	// draw movie area
	ofNoFill();
	if(bUseRecorder){
		if(bRecordingMovie)	ofSetColor(255,0,0,255);
		else				ofSetColor(100,100,100,255);
		
		ofRect(ofGetWidth()/2 - 321, ofGetHeight()/2-241, 642, 482);
	}
	
	if(bRecordingMovie)
	{
		int x = ofGetWidth()/2 - 320;
		int y = ofGetHeight()/2 - 240;
		ofImage img;
		img.grabScreen(x,y,640,480);
		movieSaver.addFrame(img.getPixels());

	}
	
	
	if(bGrabScreen)
	{
		ofImage img;
		img.grabScreen(0,0,ofGetWidth(),ofGetHeight());
		
		string filename = screenGrabPath+ofToString(screenGrabCounter)+".png";
		img.saveImage(filename);
		
		cout << "saving screen grab to " << filename << endl;
		screenGrabCounter++;
		bGrabScreen = false;
		
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	if( key == OF_KEY_F4 )
	{
		cout << "TOGGLE_MOVIE!" << endl;
		if(bRecordingMovie) movieSaver.finishMovie();
		bRecordingMovie = !bRecordingMovie;
	}
	
	if( panel.isAnyTextBoxActive() ){
		if( key == OF_KEY_BACKSPACE) panel.deleteLastChar();
		else{
			panel.addChar(key);
		}
		return;
	}
	
	scenes[currentScene]->keyPressed(key);
	
	if( key == OF_KEY_F3 )
		bUseRecorder = !bUseRecorder;
	
	if( key == OF_KEY_F1) bPanelOn = !bPanelOn;
	
	if( key == OF_KEY_F2) bGrabScreen = true;
	
	// Note: must check any text box active for scenes!!
	// if( key == 'f' ) ofToggleFullscreen();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	scenes[currentScene]->mouseMoved(x,y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	if(bPanelOn) panel.mouseDragged(x,y,button);
	scenes[currentScene]->mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(bPanelOn) panel.mousePressed(x,y,button);
	scenes[currentScene]->mousePressed(x,y,button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(bPanelOn) panel.mouseReleased();
	scenes[currentScene]->mouseReleased(x,y,button);
	
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){

}
//--------------------------------------------------------------
void testApp::setupControlPanel(){

	panel.setup("main app", 10, 10, 230, 75);

	for( int i = 0; i < numScenes; i++)
		panel.addPanel(scenes[i]->myName, 1, false);
	
	//panel.addTextInput("name", "NAME", "chris", 200, 1);
}
//--------------------------------------------------------------
void testApp::updateControlPanel(){
	
	panel.update();
	
	bool bChangedScene = false;
	if( panel.getSelectedPanelName() != lastPanelName )
	{
		bChangedScene = true;
		lastPanelName	= panel.getSelectedPanelName();
		for( int i = 0; i < numScenes; i++)
		{
			// keep username updated from sceneinput
			if( scenes[i]->myName != "user" ) scenes[i]->setUserName(scenes[1]->username);
			
			if( scenes[i]->myName == lastPanelName )
			{
				// set new scene
				currentScene = i;
				
				// if this is first change, execute start scene for any init functions
				cout << "Changed scene: " << bChangedScene << endl;
				if(bChangedScene) scenes[i]->startScene();
			}
			
		}
	}
	
	
	for( int i = 0; i < numScenes; i++)
	{
		// keep username updated from sceneinput
		if( scenes[i]->myName != "user" ) scenes[i]->setUserName(scenes[1]->username);
	}
	
}
