#include "ofApp.h"
#include "mpu_util.h"

const  float q30 = 1073741824.0f;

std::vector<dmp_data> dmp;
std::vector<dmp_data>::iterator  it_cur;

std::vector<ofVec3f> gravities;




ofRectangle viewport;

void ofApp::UpdateData()
{
	gravities.clear();

	std::vector<dmp_data>::iterator  it;
	for (it = dmp.begin(); it != dmp.end(); ++it)
	{
		for (int i = 0; i < 3; i++)
			cout << it->accel[i] << " ";
		for (int i = 0; i < 4; i++)
			cout << it->quat[i] << " ";
		cout << endl;

		// build gravities
		ofVec3f grav;
		Quaternion g = quat_from_dmpquat(it._Ptr);
		g.dmpGetGravity(&grav);
		gravities.push_back(grav);
	}

	it_cur = dmp.begin();
}
void ofApp::reset(const char *fname)
{
	ifstream in;
	string s;
	std::vector<string> lines;

	dmp.clear();

	in.open(fname, ios::in);
	if (in.is_open())
	{
		printf("Reading file\n");
		while (std::getline(in, s))
		{

			printf("%s\n", s.c_str());
			lines.push_back(s);

		}
	}

	cout << "Lines readed : " << lines.size() << endl;
	cout << "decoding data" << endl;

	std::vector<string>::iterator st;
	for (st = lines.begin(); st != lines.end(); ++st)
	{
		dmp_data data;
		istringstream s(*st);

		s >> data.it_freq >> data.accel[0] >> data.accel[1] >> data.accel[2] >> data.quat[0] >> data.quat[1] >> data.quat[2] >> data.quat[3];
		dmp.push_back(data);
	}
	this->fname = fname;
	UpdateData();
}

//--------------------------------------------------------------
void ofApp::setup(){
	
	reset("accel_4G_200hz.txt");


	// view initialization
		
	//cam.setupPerspective();
	//cam.enableOrtho();
	/*
	cam.enableMouseMiddleButton();
	cam.enableMouseInput();
	cam.setTarget(ofVec3f(0, 0, 0));
	//cam.setTranslationKey('z');
	*/
	viewport = ofGetCurrentViewport();

	//ofSetBackgroundAuto(true);
	//ofSetBackgroundColor(0, 0, 0, 255);
	

}

Quaternion curquat;

//--------------------------------------------------------------
void ofApp::update(){
	
	

	
	curquat = quat_from_dmpquat((dmp_data *) it_cur._Ptr);
	//cout << curquat.w << " " << curquat.x << " " << curquat.y << " " << curquat.z << endl;

	++it_cur;
	if (it_cur == dmp.end())
		it_cur = dmp.begin();
}

//--------------------------------------------------------------
void ofApp::draw(){
	const float falsezoom = 1;
	stringstream msg;
	std::vector<ofVec3f>::iterator  itq;// = gravities.begin();
	std::vector<ofVec3f>::iterator  itq1;// = gravities.begin() + 1;

	//cam.setupPerspective();

	ofBackgroundGradient(ofColor(128), ofColor(0));
	ofSetColor(0, 0, 0);
	ofFill();

	
	//camera.begin();
	
	cam.begin(viewport);
	
	ofPushMatrix();
	{
		ofScale(400, 400, 400);

		

		ofDrawAxis(falsezoom / 3);
		//ofDrawAxis(25);
		//ofDrawGridPlane(falsezoom/10, 15, true);
		ofDrawGrid(falsezoom / 3, 4, true, true, false, true);

		
		// draw axes id

		ofPushMatrix();
		{
			ofTranslate(2, 0, 0);
			
			
			

			
			ofSetColor(0, 0, 255);
			ofDrawBitmapString("X", falsezoom / 2, 0.1, 0.1);
			ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(1, 0, 0), 0.03);

			
			ofSetColor(255, 0, 0);
			ofDrawBitmapString("Y", 0.1, falsezoom / 2, 0.1);
			ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0), 0.03);

			ofSetColor(0, 255, 0);
			ofDrawBitmapString("Z", 0.1, 0.1, falsezoom / 2);
			ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(0, 0, 1), 0.03);
		}
		ofPopMatrix();

		ofVec3f grav;

		curquat.dmpGetGravity(&grav, &curquat);
		ofSetColor(255, 255, 255);
		ofDrawArrow(ofVec3f(0, 0, 0), grav * falsezoom, 0.005);
		ofDrawArrow(ofVec3f(0, grav.y * falsezoom, grav.z * falsezoom), grav * falsezoom, 0.005);

		
		itq = gravities.begin();
		for (auto it_dmp = dmp.begin (); it_dmp != dmp.end () && itq != gravities.end (); ++it_dmp, ++itq)
		{
			ofVec3f acc = ofVec3f(it_dmp->accel[0] * 4. / 32768., it_dmp->accel[1] * 4. / 32768., it_dmp->accel[2] * 4. / 32768.);
			//acc.normalize();
			ofVec3f g = *itq;

			//g.normalize();
			

			ofPushMatrix();
			{
				
				ofTranslate(*itq * falsezoom);
				ofSetColor(0, 255, 0);
				ofDrawLine(0, 0, 0, acc.x * 1, acc.y * 1, acc.z * 1);
				acc -= g;
				ofSetColor(0, 255, 255);
				ofDrawLine(0, 0, 0, acc.x * 1, acc.y * 1, acc.z * 1);
			}
			ofPopMatrix();
		}
		ofSetColor(0, 128, 128);
		/*
		for (int i = 0; i < gravities.size() - 1; i++)
		{
			if (gravities.at(i).x > 0)
				ofSetColor(128, 128, 0, 128);
			else
				ofSetColor(128, 32, 0, 128);
			ofDrawLine(gravities.at(i) * falsezoom , gravities.at(i + 1) * falsezoom);

		}
		*/
		
		for (itq = gravities.begin(), itq1 = gravities.begin() + 1; itq != gravities.end() && itq1 != gravities.end(); ++itq, ++itq1)
		{
			if (itq->x > 0)
				ofSetColor(128, 128, 0, 128);
			else
				ofSetColor(128, 32, 0, 128);
			ofDrawLine(*itq * falsezoom, *itq1 * falsezoom);


		}
		/*
		for (int i = 0; i < gravities.size(); i++)
		{
			if (gravities.at(i).x > 0)
				ofSetColor(128, 128, 128, 128);
			else
				ofSetColor(32, 32, 32, 128);
			ofDrawLine(gravities.at(i) * falsezoom, ofVec3f (0, gravities.at(i).y * falsezoom, gravities.at(i).z * falsezoom));

		}
		*/


		for (itq = gravities.begin(); itq != gravities.end(); ++itq)
		{
			if (itq->x > 0)
				ofSetColor(255, 255, 0);
			else
				ofSetColor(255, 32, 0);
			ofDrawSphere(*itq * falsezoom, 0.01);


		}
	}
	ofPopMatrix();
	//camera.end();
	cam.end();
	ofSetColor(0, 255, 0);
	msg << (float)cam.getGlobalPosition().x << " " << (float)cam.getGlobalPosition().y << " " << (float)cam.getGlobalPosition().z << " " << cam.getPosition().x << " " << cam.getPosition().y << " " << cam.getPosition().z;
	ofDrawBitmapString(msg.str(), 400, 1 * 12);
	msg.str("");
	msg << "fps : " << ofGetFrameRate();
	ofDrawBitmapString(msg.str(), 400, 2 * 12);
	ofDrawBitmapString(this->fname, 400, 3 * 12);
	msg.str("");
	msg << "points : " << dmp.size();
	ofDrawBitmapString(msg.str (), 400, 4*12);

	ofNoFill();
	ofPushMatrix();

	
	ofTranslate(110, 100);

	ofSetColor(0, 0, 255);
	ofDrawLine(ofVec2f(-100, 0), ofVec2f(100, 0));
	ofDrawLine(ofVec2f(0, -100), ofVec2f(0, +100));
	ofDrawCircle(ofVec2f(0, 0), 50);

	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Y", ofVec2f(5, -90));
	ofDrawBitmapString("X", ofVec2f(90, 10));
	//ofDrawCircle(ofVec2f(100, 100), 100);

	
	
	for (itq = gravities.begin(), itq1 = gravities.begin() + 1; itq != gravities.end() && itq1 != gravities.end(); ++itq, ++itq1)
	{
		ofVec2f pn = ofVec2f(itq->x, -itq->y) * 100;
		ofVec2f pn1 = ofVec2f(itq1->x, -itq1->y) * 100;
		
		if (itq->x > 0)
			ofSetColor(255, 255, 0,128);
		else
			ofSetColor(255, 0, 0,128);

		ofDrawCircle(pn, 2);

		ofSetColor(255, 255, 255,128);
		ofDrawLine(pn, pn1);
		
		
	}
	ofTranslate(0, 250);
	ofSetColor(0, 0, 255);
	ofDrawLine(ofVec2f(-100, 0), ofVec2f(100, 0));
	ofDrawLine(ofVec2f(0, -100), ofVec2f(0, +100));
	ofDrawCircle(ofVec2f(0, 0), 50);

	ofSetColor(255, 255, 255);
	ofDrawBitmapString("X", ofVec2f(5, -90));
	ofDrawBitmapString("Z", ofVec2f(90, 10));
	
	for (itq = gravities.begin(), itq1 = gravities.begin() + 1; itq != gravities.end() && itq1 != gravities.end(); ++itq, ++itq1)
	{
		ofVec2f pn = ofVec2f(itq->z, -itq->x) * 100;
		ofVec2f pn1 = ofVec2f(itq1->z, -itq1->x) * 100;

		if (itq->x > 0)
			ofSetColor(255, 255, 0, 128);
		else
			ofSetColor(255, 0, 0, 128);

		ofDrawCircle(pn, 2);

		ofSetColor(255, 255, 255, 128);
		ofDrawLine(pn, pn1);


	}

	ofTranslate(0, 250);
	ofSetColor(0, 0, 255);
	ofDrawLine(ofVec2f(-100, 0), ofVec2f(100, 0));
	ofDrawLine(ofVec2f(0, -100), ofVec2f(0, +100));
	ofDrawCircle(ofVec2f(0, 0), 50);

	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Y", ofVec2f(5, -90));
	ofDrawBitmapString("Z", ofVec2f(90, 10));

	for (itq = gravities.begin(), itq1 = gravities.begin() + 1; itq != gravities.end() && itq1 != gravities.end(); ++itq, ++itq1)
	{
		ofVec2f pn = ofVec2f(itq->z, -itq->y) * 100;
		ofVec2f pn1 = ofVec2f(itq1->z, -itq1->y) * 100;

		if (itq->x > 0)
			ofSetColor(255, 255, 0,128);
		else
			ofSetColor(255, 0, 0, 128);

		ofDrawCircle(pn, 2);

		ofSetColor(255, 255, 255, 128);
		ofDrawLine(pn, pn1);


	}
	ofPopMatrix();


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'r')
	{
		cam.setPosition(1, 1, 1000);
		cam.lookAt(ofVec3f(0, 0, 0));
	}
	if (key == 'f')
	{
		ofFileDialogResult res = ofSystemLoadDialog("", false, "*.txt");
		if (res.bSuccess == true)
		{
			
			
			reset(res.getPath().c_str());
			UpdateData();
		}
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'r')
	{
		cam.setPosition(1, 1, 500);
		cam.lookAt(ofVec3f(0, 0, 0));
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	//viewport = ofGetCurrentViewport();
	viewport = ofRectangle(0, 0, w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

