#include "FaceFactory.h"

FaceFactory::FaceFactory(){
	// landmark detecting tool
	detector = get_frontal_face_detector();
	deserialize("dataset/shape_predictor_68_face_landmarks.dat") >> pose_model;
}

void FaceFactory::detectLandmarks(Mat image){
	
}

