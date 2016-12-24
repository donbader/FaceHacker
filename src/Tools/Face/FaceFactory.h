#pragma once

#include <math.h>

// dlib
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>


// eos
#include <eos/core/Landmark.hpp>
#include <eos/core/LandmarkMapper.hpp>
#include <eos/fitting/nonlinear_camera_estimation.hpp>
#include <eos/fitting/linear_shape_fitting.hpp>
#include <eos/render/utils.hpp>
#include <eos/render/texture_extraction.hpp>
#include "eos/render/render.hpp"
#include "eos/fitting/fitting.hpp"
#include "eos/fitting/contour_correspondence.hpp"

//
#include "../GLSL/FaceProgram.h"
#include "../Debugger.h"
#include "Face.h"


#define FRAME_DETECT_FACTOR 2
#define GEN_MESH_EVERY_TIME true
#define DRAW_FACE_BY_LINE false
#define interpolate(a, b, mu) ((a)*(1-(mu)) + (b)*(mu))





using namespace std;
using namespace dlib;
using namespace eos;
using cv::Vec2f;
using cv::Vec4f;
using cv::Mat;
using eos::core::Landmark;
using eos::core::LandmarkCollection;

class Face
{
	Mat image;
	LandmarkCollection<Vec2f> landmarks;
	// mesh;

	// //
	// model_points;
	// vertex_indices;
	// image_points;

	// // coefficients
	// shape_coeffs;
	// blendshape_coeffs;
}



class FaceFactory{
public:
	FaceFactory();
	~FaceFactory(){}

	void detectLandmarks(cv::Mat);
	// void drawLandmarks(){}
	// void generateMesh(){}



private:
	/* tools */
	dlib_to_eos_landmarks();




	/* dlib */
	//for detecting enclosing box
	frontal_face_detector detector;
	//for detecting 68 landmarks in enclosing box
	shape_predictor pose_model;

};



