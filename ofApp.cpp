#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);

	for (int x = -300; x <= 300; x += 100) {

		for (int y = -300; y <= 300; y += 100) {

			this->location_list.push_back(glm::vec2(x, y));
			this->noise_seed_list.push_back(ofRandom(1000));
		}
	}

	this->deg_list = {10, 15, 18, 30, 36, 72, 90, 120, 180};
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	int anime_rate = 90;
	auto param = ofGetFrameNum() % anime_rate;

	int radius = 15;

	if (param == 0) {

		for (int i = 0; i < this->location_list.size(); i++) {
			
			this->noise_seed_list[i] = ofRandom(1000); 
		}
	}

	for (int i = 0; i < this->location_list.size(); i++) {

		ofPushMatrix();
		ofTranslate(this->location_list[i]);

		int deg_span = this->deg_list[(int)ofMap(ofNoise(this->noise_seed_list[i]), 0, 1, 1, this->deg_list.size())];
		for (int deg_start = 0; deg_start < 360; deg_start += deg_span) {

			auto source_location = glm::vec2(radius * cos(deg_start * DEG_TO_RAD), radius * sin(deg_start * DEG_TO_RAD));
			auto add_radius = radius * 1.5;
			auto anime_param = 0.f;
			if (param > anime_rate * 0.4 && param <= anime_rate * 0.5) {

				anime_param = ofMap(param, anime_rate * 0.4, anime_rate * 0.5, 0, 1);
			}
			else if (param > anime_rate * 0.5 && param <= anime_rate * 0.9) {

				anime_param = 1;
			}
			else if (param > anime_rate * 0.9 && param <= anime_rate) {

				anime_param = ofMap(param, anime_rate * 0.9, anime_rate, 1, 0);
			}

			auto noise_location = glm::vec2(anime_param * add_radius * cos(deg_start * DEG_TO_RAD), anime_param * add_radius * sin(deg_start * DEG_TO_RAD));

			vector<glm::vec2> out, in;
			for (int deg = deg_start - deg_span * 0.5; deg <= deg_start + deg_span * 0.5; deg++) {

				in.push_back(noise_location + glm::vec2((radius - 3) * cos(deg * DEG_TO_RAD), (radius - 3) * sin(deg * DEG_TO_RAD)));
				out.push_back(noise_location + glm::vec2((radius + 3) * cos(deg * DEG_TO_RAD), (radius + 3) * sin(deg * DEG_TO_RAD)));
			}

			if (param < anime_rate * 0.4) {

				ofSetColor(239);
			}
			else if (param < anime_rate * 0.5) {
				
				ofSetColor(ofMap(param, anime_rate * 0.4, anime_rate * 0.5, 239, 39));
			}
			else if (param < anime_rate * 0.9) {

				ofSetColor(39);
			}
			else {

				ofSetColor(ofMap(param, anime_rate * 0.9, anime_rate, 39, 239));
				
			}
			ofDrawLine(glm::vec2(), source_location + noise_location);

			ofSetColor(39);
			ofBeginShape();
			ofVertices(in);
			reverse(out.begin(), out.end());
			ofVertices(out);
			ofEndShape(true);
		}

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}