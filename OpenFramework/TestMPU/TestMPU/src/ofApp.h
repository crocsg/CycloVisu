#pragma once

#include "ofMain.h"
#include "mathhelper.h"



class ofApp : public ofBaseApp{

	public:
		void UpdateData();
		void reset(const char * fname);
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		std::string fname;

		ofEasyCam cam;
		ofLight light;

};
