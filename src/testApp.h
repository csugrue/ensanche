#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxControlPanel.h"

#include "sceneUserInput.h"
#include "sceneAnalysis.h"
#include "polyEditable.h"
#include "ensancheConstants.h"

// Note: prob remove this..
#include "ofxQtVideoSaver.h"

class testApp : public ofBaseApp{

	public:

	~testApp();
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void resized(int w, int h);
		
		void setupControlPanel();
		void updateControlPanel();
	
		//---- main controls
		ofxControlPanel	panel;
		bool bPanelOn;
	
		//---- scenes
		baseScene	**scenes;
		int currentScene;
		int numScenes;
		string lastPanelName;
	
		//---- app settings
	
		//---- mode
		string mode;
	
		//---- error / status report
		
	
		//---- saving to movie file
		ofxQtVideoSaver	movieSaver;
		bool bRecordingMovie;
		bool bUseRecorder;
		bool bGrabScreen;
		int screenGrabCounter;
		string screenGrabPath;

};

#endif
