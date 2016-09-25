#include "main.h"
using namespace glm;


int main( int argc, char ** argv )
{
    Window window(960, 540, "Face Hacker");
    window.setBackground(0.5,0.5,0.5);

    // get


    GLSLProgram obj_program("dataset/screen.vs","dataset/screen.fs");
    FaceProgram face_program("dataset/obj.vs","dataset/obj.fs");
    Face face("dataset/shape_predictor_68_face_landmarks.dat",
                "dataset/eos/share/ibug2did.txt",
                "dataset/eos/share/model_contours.json",
                "dataset/eos/share/sfm_shape_3448.bin",
                "dataset/eos/share/expression_blendshapes_3448.bin");


    obj_program.camera().setPosition(0,0,320);
    face_program.camera().setPosition(0,0,320);
    GLfloat vertices[] = {
        -320, -180, 0,
        320, -180, 0,
        320, 180, 0,
        -320, 180, 0,
    };

    GLfloat texcoords[] = {
        0, 1,
        1, 1,
        1, 0,
        0, 0,
    };
    GLfloat normals[] = {
    	0,0,1,
    	0,0,1,
    	0,0,1,
    	0,0,1
    };

    GLuint indice[] = {
        0, 1, 2,
        2, 3, 0,
    };



    cv::Mat white(1, 1, CV_8UC3, cv::Scalar(255, 255, 255));
    int screen_index = obj_program.addObj(	vertices, 12,
										texcoords, 8,
										normals,  12,
										indice,    6,
										white
                                    );
    int anchor = obj_program.addObj(    vertices, 12,
                                        texcoords, 8,
                                        normals,  12,
                                        indice,    6,
                                        white
                                    );

    // obj_program.object(screen_index)->translate_Model(0,0,1000);
    obj_program.object(anchor)->scale_Model(1.f/640,1.f/360,1);

    cv::Mat frame;
    cv::VideoCapture cap;
    cap.open(0);

    //game loop
    GLint k=100;
    do {
        if (!cap.read(frame))break;


        //face operation
        // if(argc == 3){
        //     face.setCoef(std::stoi(argv[1]));
        //     face.setRot(std::stof(argv[2])*(k++));
        // }

        _DEBUG_TIMER_INIT();
        _PRINT_TIME("face.detect()",
        face.detect(frame, false, true);
        );

        _PRINT_TIME("face.objectOperation()",
        face.objectOperation(&face_program, true);
        );
        //set camera frame
        obj_program.object(screen_index)->setTexture(frame);
        obj_program.render();


        window.render();

    }
    while (!window.shouldClose());

    return 0;
}

