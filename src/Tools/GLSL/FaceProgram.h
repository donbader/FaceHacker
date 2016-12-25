#pragma once

#include "GLSLProgram.h"

#include <eos/render/utils.hpp>



using eos::render::Mesh;
using std::vector;

class FaceProgram : public GLSLProgram {
public:
	FaceProgram(const string& vsFile, const string& fsFile, const string& gsFile = "none");
	~FaceProgram(){}

	int addObj(Mesh mesh);
	void render();
	void translate_Bone(int index, GLfloat x, GLfloat y, GLfloat z);
	void scale_Bone(int index, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	void setRenderMode(int m){render_mode=m;}
	//generator
	vector<cv::Vec2f> genBoneIndexArray(int meshSize);
	vector<cv::Vec2f> genBoneWeights(int meshSize);
private:
	vector<cv::Vec2f> bone_index_array;
	vector<cv::Vec2f> bone_weights;
	int render_mode = GL_TRIANGLES;
};
