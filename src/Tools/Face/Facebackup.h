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





class PcaCoefficientMerging
{
public:
	/**
	 * @brief Merges the given new PCA coefficients with all previously processed coefficients.
	 *
	 * @param[in] coefficients The new coefficients to add.
	 * @return The merged coefficients of all images processed so far.
	 */
	std::vector<float> add_and_merge(const std::vector<float>& coefficients)
	{
		if (merged_shape_coefficients.empty())
		{
			merged_shape_coefficients = cv::Mat::zeros(coefficients.size(), 1, CV_32FC1);
		}
		assert(coefficients.size() == merged_shape_coefficients.rows);

		cv::Mat test(coefficients);
		merged_shape_coefficients = (merged_shape_coefficients * num_processed_frames + test) / (num_processed_frames + 1.0f);
		++num_processed_frames;
		return std::vector<float>(merged_shape_coefficients.begin<float>(), merged_shape_coefficients.end<float>());
	};

private:
	int num_processed_frames = 0;
	cv::Mat merged_shape_coefficients;
};


class Face {
public:
	//constructor & destructor
	Face(
		string shapePredictFile,
		string mappingsFile,
		string contourFile,
		string morphableModelFile,
		string BlendShapeFile
	);

	~Face();

	//
	void detect(cv::Mat newFrame, bool draw_rects=false, bool draw_landmarks=false);
	void objectOperation(FaceProgram* program, bool doTransform = true);

	//converter

	static cv::Rect toCvRect(dlib::rectangle drect);
	template<class T>std::tuple<std::vector<Vec2f>, std::vector<Vec4f>, std::vector<int>> get_corresponding_pointset(const T& landmarks, const eos::core::LandmarkMapper& landmark_mapper, const eos::morphablemodel::MorphableModel& morphable_model);
	template<class T>std::vector<T> concat(const std::vector<T>& vec_a, const std::vector<T>& vec_b);
	LandmarkCollection<Vec2f> dlib_to_eos_landmarks(full_object_detection dlib_landmarks);



	//getter
	int objectIndex(cv::Mat frame);
	render::Mesh mesh() {return _mesh;}
	cv::Mat frame() {return _frame;}
	cv::Mat isomap() {return _isomap;}
	bool boxes_Detected() {return ((_enclosing_boxes.size() == 0) ? false : true);}
	bool landmarks_Detected() {return ((_current_landmarks.size() == 0) ? false : true);}
	bool hasMesh(){return ((_mesh.vertices.size() == 0 ) ? false : true);}


	//setter
	void setFrame(cv::Mat frame) { _frame = frame;}
	void setEnclosingBoxes(std::vector<dlib::rectangle> dlibFaceRects);
	void setLandmarks(LandmarkCollection<Vec2f> eos_landmarks);
	void setMesh(render::Mesh mesh);
	void setRenderingParams(fitting::RenderingParameters rendering_params);
	void setRot(GLfloat r){_rot = r;};
	void setCoef(GLuint i){_coef = i;}

	//detector
	std::vector<dlib::rectangle> detectEnclosingBoxes();
	full_object_detection detectLandmarks();

	//generator
	fitting::RenderingParameters genRenderingParams();
	render::Mesh genMesh_1();
	render::Mesh genMesh_2();
	render::Mesh genMesh_3();
	render::Mesh genMesh_4();
	glm::vec3 genDelta_39();
	GLfloat genRatio_39_42();
	cv::Mat to_matrix(const std::vector<eos::morphablemodel::Blendshape>& blendshapes);
	cv::Mat genIsomap();



	// glm::vec3 rotateVec3(glm::vec3 p, GLfloat radx, GLfloat rady, GLfloat, radz);
	Vec2f toWorldVec(Vec2f matPos);



	//painter
	void drawObjects(cv::Scalar color);
	void drawLandmarks(cv::Scalar color);
	void drawLine(int i, int j, cv::Scalar color);
	void drawSingleLine(int i, int j, cv::Scalar color);



private:

	cv::Mat _frame;
	cv::Mat _faceROI;

	//eos
	LandmarkCollection<Vec2f> _current_landmarks;

	core::LandmarkMapper _landmark_mapper;
	fitting::RenderingParameters _rendering_params;
	fitting::ModelContour _model_contour;
	fitting::ContourLandmarks _ibug_contour;
	cv::Mat _isomap;
	cv::Mat _affine_from_ortho;
	morphablemodel::MorphableModel _morphable_model;
	std::vector<morphablemodel::Blendshape> _blend_shapes;
	PcaCoefficientMerging _pca_shape_merging;
	render::Mesh _mesh;

    std::vector<Vec4f> _model_points; // the points in the 3D shape model
    std::vector<int> _vertex_indices; // their vertex indices
    std::vector<Vec2f> _image_points; // the corresponding 2D landmark points
	float _head_angles[3] = {0};

	//dlib
	frontal_face_detector _detector;
	shape_predictor _pose_model;
	std::vector<dlib::rectangle> _enclosing_boxes;

	//painter
	std::vector<cv::Rect> _painting_objects;

	//program
	int _indexInProgram = -1;
	GLSLVertexArrayObject* _objInProgram;
	GLuint _coef = 0;
	GLfloat _rot = 0.f;
};
