#include "Face.h"

//constructor & destructor
Face::Face(
    string shapePredictFile,
    string mappingsFile,
    string contourFile,
    string morphableModelFile,
    string BlendShapeFile
)
{
	/* init dlib */
	//for detect enclosing box
	_detector = get_frontal_face_detector();
	//for detect 68 landmarks in enclosing box
	deserialize(shapePredictFile) >> _pose_model;

	/* init eos */
	//for mapping 2D landmarks points to 3D mesh vertices
	_landmark_mapper = core::LandmarkMapper(mappingsFile);
	_ibug_contour =	fitting::ContourLandmarks::load(mappingsFile);
	_model_contour = fitting::ModelContour::load(contourFile);
	_morphable_model = morphablemodel::load_model(morphableModelFile);
	_blend_shapes = morphablemodel::load_blendshapes(BlendShapeFile);


}

Face::~Face()
{

}

//
void Face::detect(cv::Mat newFrame, bool draw_enclosing_boxes, bool draw_landmarks)
{
	_frame = newFrame;

	//timer start
	_DEBUG_TIMER_INIT();

	// detect & set enclosing boxes
	setEnclosingBoxes(detectEnclosingBoxes());
	_DEBUG_TIME("detectEnclosingBoxes()");

	// draw
	if (draw_enclosing_boxes && boxes_Detected()) {
		_painting_objects.clear();
		_painting_objects.push_back(toCvRect(_enclosing_boxes[0]));
		drawObjects(cv::Scalar(0, 255, 255));
	}

	// detect & set 68 landmarks
	_DEBUG_TIMER_ZERO();
	setLandmarks(dlib_to_eos_landmarks(detectLandmarks()));
	_DEBUG_TIME("detectLandmarks()");

	// draw
	if (draw_landmarks && landmarks_Detected()) {
		_painting_objects.clear();
		for (int i = 0; i < _current_landmarks.size(); ++i) {
			_painting_objects.push_back( {(int)(_current_landmarks[i].coordinates[0]), (int)(_current_landmarks[i].coordinates[1]), 2, 2} );
		}

		if (DRAW_FACE_BY_LINE)
			drawLandmarks(cv::Scalar(255, 255, 0));
		else
			drawObjects(cv::Scalar(255, 255, 0));
	}
}

void Face::objectOperation(FaceProgram* program, bool doTransform)
{
	_DEBUG_MSG("-----------------------------------------------------------------");
	_DEBUG_TIMER_INIT();
	setRenderingParams(genRenderingParams());
	_DEBUG_TIME("genRenderingParams()");



	if (GEN_MESH_EVERY_TIME) {
		setMesh(genMesh_3());
		if (hasMesh()) {
			_indexInProgram = program->addObj(_mesh);
			_objInProgram = program->object(_indexInProgram);

			if(doTransform){
				//move to anchor
				GLfloat anchor_x = (_mesh.vertices[181][0] + _mesh.vertices[614][0]) / 2;
				GLfloat anchor_y = (_mesh.vertices[181][1] + _mesh.vertices[614][1]) / 2;
				GLfloat anchor_z = (_mesh.vertices[181][2] + _mesh.vertices[614][2]) / 2;
				_objInProgram->translate_Model(-anchor_x, -anchor_y, -anchor_z);



				//scale
				GLfloat ratio = genRatio_39_42() * (220.f / (320.f - _mesh.vertices[181][2]));
				_objInProgram->scale_Model(ratio*1.2, ratio, ratio*1.2);


				// rotate
				_objInProgram->rotate_Model(_head_angles[0], _head_angles[1], _head_angles[2]);

				// //move to the right position
				glm::vec3 delta = genDelta_39();
				_objInProgram->translate_Model(delta.x, delta.y, 100 );
				program->camera().setPosition(delta.x, delta.y, 320);
			}
			else{
				_objInProgram->translate_Model(0, 0, 100);
			}

			// _objInProgram->setTexture(genIsomap());
			//render
			program->render();
			program->deleteObj(_indexInProgram);
		}
	}
	else {
		if (!landmarks_Detected()) {
			program->deleteObj(_indexInProgram);
			_indexInProgram = -1;
			return;
		}

		if (!hasMesh()) {
			setMesh(genMesh_2());
			_indexInProgram = program->addObj(_mesh);
			_objInProgram = program->object(_indexInProgram);
			_objInProgram->translate_Model(0, 0, 95);
		}

		//rotate
		// _objInProgram->rotate_Model(_angles_delta[0],1,0,0);
		// _objInProgram->rotate_Model(_angles_delta[1],0,1,0);
		// _objInProgram->rotate_Model(_angles_delta[2],0,0,1);

		//render
		program->render();
	}
	//set deformation
}


//setter
void Face::setEnclosingBoxes(std::vector<dlib::rectangle> boxes)
{
	_enclosing_boxes = boxes;
}

void Face::setLandmarks(LandmarkCollection<cv::Vec2f> eos_landmarks)
{

	if (!boxes_Detected()) {
		_mesh.vertices.clear();
		_current_landmarks = eos_landmarks;
	}
	else {
		if (_current_landmarks.size() > 0) {
			for (int i = 0; i < eos_landmarks.size(); ++i) {
				_current_landmarks[i].coordinates[0] = interpolate(
				        _current_landmarks[i].coordinates[0],
				        eos_landmarks[i].coordinates[0],
				        0.75
				                                       );
				_current_landmarks[i].coordinates[1] = interpolate(
				        _current_landmarks[i].coordinates[1],
				        eos_landmarks[i].coordinates[1],
				        0.75
				                                       );
			}
		}
		else {
			_current_landmarks = eos_landmarks;
		}
	}

}
void Face::setRenderingParams(fitting::RenderingParameters rendering_params)
{
	_rendering_params = rendering_params;
	_head_angles[0] = rendering_params.r_x;
	_head_angles[1] = rendering_params.r_y;
	_head_angles[2] = rendering_params.r_z;

	_affine_from_ortho = get_3x4_affine_camera_matrix(_rendering_params, _frame.cols, _frame.rows);
}
void Face::setMesh(render::Mesh mesh) {
	if (_mesh.vertices.size() > 0) {
		//interpolation
		for (int i = 0; i < mesh.vertices.size(); ++i) {
			_mesh.vertices[i][0] = interpolate(_mesh.vertices[i][0], mesh.vertices[i][0], 0.6);
			_mesh.vertices[i][1] = interpolate(_mesh.vertices[i][1], mesh.vertices[i][1], 0.6);
			_mesh.vertices[i][2] = interpolate(_mesh.vertices[i][2], mesh.vertices[i][2], 0.6);
		}
	}
	else {
		_mesh = mesh;
	}
}



//generator
std::vector<dlib::rectangle> Face::detectEnclosingBoxes()
{
	cv::Mat tmp(_frame);
	// resize for speed
	cv::resize(_frame, tmp, cv::Size(_frame.cols / FRAME_DETECT_FACTOR, _frame.rows / FRAME_DETECT_FACTOR));
	cv_image<bgr_pixel> cimg(tmp);
	std::vector<dlib::rectangle> enclosing_boxes = _detector(cimg);
	// reduce to origin enclosing box
	for (int i = 0; i < enclosing_boxes.size(); ++i) {
		enclosing_boxes[i].left() *= FRAME_DETECT_FACTOR;
		enclosing_boxes[i].right() *= FRAME_DETECT_FACTOR;
		enclosing_boxes[i].top() *= FRAME_DETECT_FACTOR;
		enclosing_boxes[i].bottom() *= FRAME_DETECT_FACTOR;
	}
	return enclosing_boxes;
}

full_object_detection Face::detectLandmarks()
{
	// Find the Landmarks of each Enclosing Box
	full_object_detection landmarks;
	if (boxes_Detected()) {
		cv_image<bgr_pixel> cimg(_frame);
		landmarks = _pose_model(cimg, _enclosing_boxes[0]);
	}
	return landmarks;
}

fitting::RenderingParameters Face::genRenderingParams() {
	fitting::RenderingParameters rendering_params;
	if (landmarks_Detected()) {
		std::tie(_image_points, _model_points, _vertex_indices) = get_corresponding_pointset(_current_landmarks, _landmark_mapper, _morphable_model);
		// Estimate the camera (pose) from the 2D - 3D point correspondences
		rendering_params = fitting::estimate_orthographic_camera(_image_points, _model_points, _frame.cols, _frame.rows);
		// add contour
		// Given the estimated pose, find 2D-3D contour correspondences:
		auto view_model = fitting::get_4x4_modelview_matrix(rendering_params);
		auto ortho_projection = glm::ortho(rendering_params.frustum.l, rendering_params.frustum.r, rendering_params.frustum.b, rendering_params.frustum.t);
		glm::vec4 viewport(0, _frame.rows, _frame.cols, -_frame.rows); // flips y, origin top-left, like in OpenCV
		// These are the additional contour correspondences we're going to find and then use:
		std::vector<cv::Vec2f> image_points_contour;
		std::vector<cv::Vec4f> model_points_contour;
		std::vector<int> vertex_indices_contour;
		// For each 2D contour landmark, get the corresponding 3D vertex point and vertex id:

		std::tie(image_points_contour, model_points_contour, vertex_indices_contour) = fitting::get_contour_correspondences(_current_landmarks, _ibug_contour, _model_contour, glm::degrees(rendering_params.r_y), _morphable_model, view_model, ortho_projection, viewport);
		// Add the contour correspondences to the set of landmarks that we use for the fitting:
		_image_points = concat(_image_points, image_points_contour);
		_model_points = concat(_model_points, model_points_contour);
		_vertex_indices = concat(_vertex_indices, vertex_indices_contour);
		//re- estimate
		rendering_params = fitting::estimate_orthographic_camera(_image_points, _model_points, _frame.cols, _frame.rows);


	}


	return rendering_params;

}




render::Mesh Face::genMesh_1() {
	render::Mesh mesh;
	if (boxes_Detected() && landmarks_Detected()) {
		// Estimate the shape coefficients by fitting the shape to the landmarks:
		std::vector<float> shape_coeffs = fitting::fit_shape_to_landmarks_linear(_morphable_model, _affine_from_ortho, _image_points, _vertex_indices);

		// Obtain the full mesh with the estimated coefficients:
		mesh = _morphable_model.draw_sample(shape_coeffs, std::vector<float>());
	}

	return mesh;
}
render::Mesh Face::genMesh_2() {
	render::Mesh mesh;
	if (boxes_Detected() && landmarks_Detected()) {
		_DEBUG_TIMER_INIT();
		// Fit the PCA shape model and expression blendshapes:
		std::vector<float> shape_coeffs;
		// shape_coeffs.reserve(63);
		std::vector<float> blendshape_coeffs;
		// blendshape_coeffs.reserve(6);
		//return morphing要用的矩陣
		cv::Mat shape_instance = fitting::fit_shape_model(_affine_from_ortho, _morphable_model, _blend_shapes, _image_points, _vertex_indices, 10.0f, shape_coeffs, blendshape_coeffs);
		_DEBUG_TIME("fit_shape_model()");


		// mesh = morphablemodel::detail::sample_to_mesh(shape_instance, _morphable_model.get_color_model().get_mean(), _morphable_model.get_shape_model().get_triangle_list(), _morphable_model.get_color_model().get_triangle_list(), _morphable_model.get_texture_coordinates());
		shape_coeffs = _pca_shape_merging.add_and_merge(shape_coeffs);
		auto merged_shape = _morphable_model.get_shape_model().draw_sample(shape_coeffs) + to_matrix(_blend_shapes) * Mat(blendshape_coeffs);
		mesh = morphablemodel::detail::sample_to_mesh(merged_shape, _morphable_model.get_color_model().get_mean(), _morphable_model.get_shape_model().get_triangle_list(), _morphable_model.get_color_model().get_triangle_list(), _morphable_model.get_texture_coordinates());


		_DEBUG_TIME("sample_to_mesh()");

	}
	return mesh;
}

render::Mesh Face::genMesh_3() {
	render::Mesh mesh;
	if (boxes_Detected() && landmarks_Detected()) {
		// Estimate the shape coefficients by fitting the shape to the landmarks:
		Mat blendshapes_as_basis(_blend_shapes[0].deformation.rows, _blend_shapes.size(), CV_32FC1); // assert blendshapes.size() > 0 and all of them have same number of rows, and 1 col
		for (int i = 0; i < _blend_shapes.size(); ++i)
		{
			_blend_shapes[i].deformation.copyTo(blendshapes_as_basis.col(i));
		}
		std::vector<float> shape_coeffs, last_shape_coeffs;
		std::vector<float> blendshape_coeffs, last_blendshape_coeffs;
		shape_coeffs.resize(_morphable_model.get_shape_model().get_num_principal_components());
		blendshape_coeffs.resize(_blend_shapes.size());
		cv::Mat combined_shape;

		do{
			last_shape_coeffs = shape_coeffs;
			last_blendshape_coeffs = blendshape_coeffs;

			Mat mean_plus_blendshapes = _morphable_model.get_shape_model().get_mean() + blendshapes_as_basis * Mat(last_blendshape_coeffs);
			shape_coeffs = fitting::fit_shape_to_landmarks_linear(_morphable_model, _affine_from_ortho, _image_points, _vertex_indices, mean_plus_blendshapes, 3.f);

			Mat pca_model_shape = _morphable_model.get_shape_model().draw_sample(shape_coeffs);
			blendshape_coeffs = fitting::fit_blendshapes_to_landmarks_linear(_blend_shapes, pca_model_shape, _affine_from_ortho, _image_points, _vertex_indices, 0.f);

			combined_shape = pca_model_shape + blendshapes_as_basis * Mat(blendshape_coeffs);
		}while(std::abs(cv::norm(shape_coeffs) - cv::norm(last_shape_coeffs)) >= 0.5 || std::abs(cv::norm(blendshape_coeffs) - cv::norm(last_blendshape_coeffs)) >= 0.5);

		// Obtain the full mesh with the estimated coefficients:
		mesh = morphablemodel::detail::sample_to_mesh(combined_shape, _morphable_model.get_color_model().get_mean(), _morphable_model.get_shape_model().get_triangle_list(), _morphable_model.get_color_model().get_triangle_list(), _morphable_model.get_texture_coordinates());

	}

	return mesh;
}


render::Mesh Face::genMesh_4() {
	render::Mesh mesh;
	if (boxes_Detected() && landmarks_Detected()) {
		// Estimate the shape coefficients by fitting the shape to the landmarks:
		Mat blendshapes_as_basis(_blend_shapes[0].deformation.rows, _blend_shapes.size(), CV_32FC1); // assert blendshapes.size() > 0 and all of them have same number of rows, and 1 col
		for (int i = 0; i < _blend_shapes.size(); ++i)
		{
			_blend_shapes[i].deformation.copyTo(blendshapes_as_basis.col(i));
		}
		std::vector<float> shape_coeffs, last_shape_coeffs;
		std::vector<float> blendshape_coeffs, last_blendshape_coeffs;
		shape_coeffs.resize(_morphable_model.get_shape_model().get_num_principal_components());
		blendshape_coeffs.resize(_blend_shapes.size());
		cv::Mat combined_shape;

		do{
			last_blendshape_coeffs = blendshape_coeffs;

			Mat mean_plus_blendshapes = _morphable_model.get_shape_model().get_mean() + blendshapes_as_basis * Mat(last_blendshape_coeffs);

			blendshape_coeffs = fitting::fit_blendshapes_to_landmarks_linear(_blend_shapes, mean_plus_blendshapes, _affine_from_ortho, _image_points, _vertex_indices, 0.f);

			combined_shape = mean_plus_blendshapes + blendshapes_as_basis * Mat(blendshape_coeffs);
		}while(std::abs(cv::norm(blendshape_coeffs) - cv::norm(last_blendshape_coeffs)) >= 0.5);


		// Obtain the full mesh with the estimated coefficients:
		mesh = morphablemodel::detail::sample_to_mesh(combined_shape, _morphable_model.get_color_model().get_mean(), _morphable_model.get_shape_model().get_triangle_list(), _morphable_model.get_color_model().get_triangle_list(), _morphable_model.get_texture_coordinates());

	}

	return mesh;
}

glm::vec3 Face::genDelta_39() {
	glm::vec3 p;
	GLfloat x = (_current_landmarks[39].coordinates[0] + _current_landmarks[42].coordinates[0]) / 2;
	GLfloat y = (_current_landmarks[39].coordinates[1] + _current_landmarks[42].coordinates[1]) / 2;
	p = glm::vec3(x * 640 / _frame.cols - 320, 180 - y * 360 / _frame.rows, 0);
	return p;
}

GLfloat Face::genRatio_39_42() {
	glm::vec3 p1, p2;
	glm::vec4 p3;
	p1 = glm::vec3(_current_landmarks[39].coordinates[0] * 640 / _frame.cols - 320, 180 - _current_landmarks[39].coordinates[1] * 360 / _frame.rows, 0);
	p2 = glm::vec3(_current_landmarks[42].coordinates[0] * 640 / _frame.cols - 320, 180 - _current_landmarks[42].coordinates[1] * 360 / _frame.rows, 0);
	p3 = glm::vec4(_mesh.vertices[614][0] - _mesh.vertices[181][0],
	               _mesh.vertices[614][1] - _mesh.vertices[181][1],
	               _mesh.vertices[614][2] - _mesh.vertices[181][2], 1);

	glm::mat4 r_matrix = GLSLVertexArrayObject::rotateMatrix(_head_angles[0], _head_angles[1], _head_angles[2]);
	p3 = r_matrix * p3;
	return pow( (pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)) / (pow(p3.x, 2) + pow(p3.y, 2)), 0.5);
}


cv::Mat Face::genIsomap()
{
	cv::Mat isomap = render::extract_texture(_mesh, _affine_from_ortho, _frame);
	return isomap;
}



//converter


cv::Rect Face::toCvRect(dlib::rectangle drect) {
	return cv::Rect( drect.left(), drect.top(), drect.width(), drect.height());
}

template<class T>
std::tuple<std::vector<Vec2f>, std::vector<Vec4f>, std::vector<int>>
        Face::get_corresponding_pointset(
            const T& landmarks,
            const eos::core::LandmarkMapper& landmark_mapper,
            const eos::morphablemodel::MorphableModel& morphable_model
        )
{
	using cv::Mat;
	using std::vector;
	using cv::Vec2f;
	using cv::Vec4f;

	// These will be the final 2D and 3D points used for the fitting:
	vector<Vec2f> image_points; // the corresponding 2D landmark points
	vector<Vec4f> model_points; // the points in the 3D shape model
	vector<int> vertex_indices; // their vertex indices

	// Sub-select all the landmarks which we have a mapping for (i.e. that are defined in the 3DMM):
	for (int i = 0; i < landmarks.size(); ++i) {
		auto converted_name = landmark_mapper.convert(landmarks[i].name);
		if (!converted_name) { // no mapping defined for the current landmark
			continue;
		}
		int vertex_idx = std::stoi(converted_name.get());
		Vec4f vertex = morphable_model.get_shape_model().get_mean_at_point(vertex_idx);
		model_points.emplace_back(vertex);
		vertex_indices.emplace_back(vertex_idx);
		image_points.emplace_back(landmarks[i].coordinates);
	}

	return std::make_tuple(image_points, model_points, vertex_indices);
};

template <class T>
std::vector<T> Face::concat(const std::vector<T>& vec_a, const std::vector<T>& vec_b)
{
	std::vector<T> concatenated_vec;
	concatenated_vec.reserve(vec_a.size() + vec_b.size());
	concatenated_vec.insert(std::end(concatenated_vec), std::begin(vec_a), std::end(vec_a));
	concatenated_vec.insert(std::end(concatenated_vec), std::begin(vec_b), std::end(vec_b));
	return concatenated_vec;
};


cv::Vec2f Face::toWorldVec(cv::Vec2f matPos) {
	return cv::Vec2f(	matPos[0] * 640 / _frame.cols - 320,
	                    180 - matPos[1] * 360 / _frame.rows);
}


//painter
void Face::drawObjects(cv::Scalar color) {
	cv::Scalar tmp;
	for (int i = 0; i < _painting_objects.size(); ++i) {
		if (i == 39 || i == 42) {
			tmp = cv::Scalar(125, 120, 255);
		}
		else
			tmp = color;
		cv::rectangle(_frame,
		              cvPoint(_painting_objects[i].x, _painting_objects[i].y ),
		              cvPoint(_painting_objects[i].x + _painting_objects[i].width, _painting_objects[i].y  + _painting_objects[i].height),
		              tmp);
	}
}

void Face::drawLandmarks(cv::Scalar color) {

	for (int j = 0; j < _painting_objects.size() / 68; ++j) {
		// 17 for ear-chin-ear
		drawLine(j * 68 + 0, j * 68 + 16, color);
		//4 for top of nose
		drawLine(j * 68 + 27, j * 68 + 30, color);

		//5 for bottom of nose
		drawLine(j * 68 + 30, j * 68 + 35, color);
		drawSingleLine(j * 68 + 30, j * 68 + 35, color);
		//5 for left eyebrow
		drawLine(j * 68 + 17, j * 68 + 21, color);
		//5 for right eyebrow
		drawLine(j * 68 + 22, j * 68 + 26, color);


		//5 for left eye
		drawLine(j * 68 + 36, j * 68 + 41, color);
		drawSingleLine(j * 68 + 36, j * 68 + 41, color);
		//5 for right eye
		drawLine(j * 68 + 42, j * 68 + 47, color);
		drawSingleLine(j * 68 + 42, j * 68 + 47, color);
		//11 for Lips outer part
		drawLine(j * 68 + 48, j * 68 + 59, color);
		drawSingleLine(j * 68 + 48, j * 68 + 59, color);

		//11 for Lips inner part
		drawLine(j * 68 + 60, j * 68 + 67, color);
		drawSingleLine(j * 68 + 60, j * 68 + 67, color);
	}
}

void Face::drawLine(int i, int j, cv::Scalar color) {
	for (int k = i + 1; k <= j; ++k)
		cv::line(_frame, cvPoint(_painting_objects[k - 1].x, _painting_objects[k - 1].y), cvPoint(_painting_objects[k].x, _painting_objects[k].y), color, 1, 16);
}

void Face::drawSingleLine(int i, int j, cv::Scalar color) {
	cv::line(_frame, cvPoint(_painting_objects[i].x, _painting_objects[i].y), cvPoint(_painting_objects[j].x, _painting_objects[j].y), color, 1, 16);
}


cv::Mat Face::to_matrix(const std::vector<eos::morphablemodel::Blendshape>& blendshapes)
{
	cv::Mat blendshapes_as_basis(blendshapes[0].deformation.rows, blendshapes.size(), CV_32FC1); // assert blendshapes.size() > 0 and all of them have same number of rows, and 1 col
	for (int i = 0; i < blendshapes.size(); ++i)
	{
		blendshapes[i].deformation.copyTo(blendshapes_as_basis.col(i));
	}
	return blendshapes_as_basis;
};

LandmarkCollection<cv::Vec2f> Face::dlib_to_eos_landmarks(full_object_detection dlib_landmarks) {
	LandmarkCollection<cv::Vec2f> eos_landmarks;
	for (int i = 0; i < dlib_landmarks.num_parts(); ++i) {
		eos_landmarks.emplace_back(Landmark<cv::Vec2f> {std::to_string(i + 1), cv::Vec2f{(float)dlib_landmarks.part(i).x(), (float)dlib_landmarks.part(i).y()}});
	}
	return eos_landmarks;
}


// LandmarkCollection<cv::Vec2f> Face::getToType() {
// 	LandmarkCollection<cv::Vec2f> landmarks;
// 	landmarks.reserve(68);
// 	int ibugId = 1;
// 	for (int i = 0; i < _landmarks.num_parts(); ++i) {
// 		Landmark<cv::Vec2f> landmark;
// 		landmark.name = std::to_string(ibugId);
// 		landmark.coordinates[0] = _landmarks.part(i).x();
// 		landmark.coordinates[1] = _landmarks.part(i).y();
// 		landmarks.emplace_back(landmark);
// 		++ibugId;
// 	}
// 	return landmarks;
// }

// LandmarkCollection<cv::Vec2f> Face::rcr_to_eos_landmark_collection(const LandmarkCollection<cv::Vec2f>& landmark_collection) {
// 	eos::core::LandmarkCollection<cv::Vec2f> eos_landmark_collection;
// 	std::transform(begin(landmark_collection), end(landmark_collection), std::back_inserter(eos_landmark_collection), [](auto && lm) { return eos::core::Landmark<cv::Vec2f> { lm.name, lm.coordinates }; });
// 	return eos_landmark_collection;
// }

// fitting::ContourLandmarks Face::getToType2() {
// 	fitting::ContourLandmarks contour;
// 	for (int i = 1; i <= 8; ++i) {
// 		contour.right_contour.push_back(std::to_string(i));
// 		contour.left_contour.push_back(std::to_string(i + 9));
// 	}
// 	return contour;
// }

// fitting::ModelContour Face::getToType3() {
// 	fitting::ModelContour contour;
// 	contour.right_contour.push_back(380);
// 	contour.right_contour.push_back(373);
// 	contour.right_contour.push_back(356);
// 	contour.right_contour.push_back(358);
// 	contour.right_contour.push_back(359);
// 	contour.right_contour.push_back(360);
// 	contour.right_contour.push_back(365);
// 	contour.right_contour.push_back(363);
// 	contour.right_contour.push_back(364);
// 	contour.right_contour.push_back(388);
// 	contour.right_contour.push_back(391);
// 	contour.right_contour.push_back(392);
// 	contour.right_contour.push_back(393);
// 	contour.right_contour.push_back(11   );
// 	contour.right_contour.push_back(21   );
// 	contour.right_contour.push_back(25   );
// 	contour.right_contour.push_back(22   );
// 	contour.left_contour.push_back(795);
// 	contour.left_contour.push_back(790);
// 	contour.left_contour.push_back(773);
// 	contour.left_contour.push_back(775);
// 	contour.left_contour.push_back(776);
// 	contour.left_contour.push_back(777);
// 	contour.left_contour.push_back(782);
// 	contour.left_contour.push_back(780);
// 	contour.left_contour.push_back(781);
// 	contour.left_contour.push_back(802);
// 	contour.left_contour.push_back(805);
// 	contour.left_contour.push_back(806);
// 	contour.left_contour.push_back(807);
// 	contour.left_contour.push_back(454);
// 	contour.left_contour.push_back(464);
// 	contour.left_contour.push_back(466);
// 	contour.left_contour.push_back(465);
// 	return contour;
// }

// std::vector<cv::Vec4f> Face::concat(const std::vector<cv::Vec4f>& vec_a, const std::vector<cv::Vec4f>& vec_b) {
// 	std::vector<cv::Vec4f> concatenated_vec;
// 	concatenated_vec.reserve(vec_a.size() + vec_b.size());
// 	concatenated_vec.insert(std::end(concatenated_vec), std::begin(vec_a), std::end(vec_a));
// 	concatenated_vec.insert(std::end(concatenated_vec), std::begin(vec_b), std::end(vec_b));
// 	return concatenated_vec;
// }

// std::vector<int> Face::concat2(const std::vector<int>& vec_a, const std::vector<int>& vec_b) {
// 	std::vector<int> concatenated_vec;
// 	concatenated_vec.reserve(vec_a.size() + vec_b.size());
// 	concatenated_vec.insert(std::end(concatenated_vec), std::begin(vec_a), std::end(vec_a));
// 	concatenated_vec.insert(std::end(concatenated_vec), std::begin(vec_b), std::end(vec_b));
// 	return concatenated_vec;
// }

// std::vector<cv::Vec2f> Face::concat3(const std::vector<cv::Vec2f>& vec_a, const std::vector<cv::Vec2f>& vec_b) {
// 	std::vector<cv::Vec2f> concatenated_vec;
// 	concatenated_vec.reserve(vec_a.size() + vec_b.size());
// 	concatenated_vec.insert(std::end(concatenated_vec), std::begin(vec_a), std::end(vec_a));
// 	concatenated_vec.insert(std::end(concatenated_vec), std::begin(vec_b), std::end(vec_b));
// 	return concatenated_vec;
// }