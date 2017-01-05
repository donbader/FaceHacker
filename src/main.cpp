#include "main.h"
using namespace glm;

void runMenu();

int main(int argc, char ** argv)
{

    Window window(960, 540, "Face Hacker");
    window.setBackground(0.5, 0.5, 0.5);

    auto app = Gtk::Application::create(argc, argv);
    ControlMenu Controlwindow;
    Controlwindow.show();


    // get

    //Shows the window and returns when it is closed.

    GLSLProgram obj_program("dataset/screen.vs", "dataset/screen.fs");
    FaceProgram face_program("dataset/obj.vs", "dataset/obj.fs");
    Face face("dataset/shape_predictor_68_face_landmarks.dat",
              "dataset/eos/share/ibug2did.txt",
              "dataset/eos/share/model_contours.json",
              "dataset/eos/share/sfm_shape_3448.bin",
              "dataset/eos/share/expression_blendshapes_3448.bin");


    obj_program.camera().setPosition(0, 0, 320);
    face_program.camera().setPosition(0, 0, 320);
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
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1
    };

    GLuint indice[] = {
        0, 1, 2,
        2, 3, 0,
    };



    cv::Mat white(1, 1, CV_8UC3, cv::Scalar(255, 255, 255));
    int screen_index = obj_program.addObj(  vertices, 12,
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
    obj_program.object(anchor)->scale_Model(1.f / 640, 1.f / 360, 1);

    cv::Mat frame;
    cv::VideoCapture cap;
    cap.open(0);
    // cap.open("dataset/test.mp4");

    //game loop
    GLint k = 100;
    face_program.setUniformFloat("lightPower", 100000);

    // thread mthread(runMenu);
    do {
        if (!cap.read(frame))break;
        gtk_main_iteration_do(false);
        cv::Mat dst;
        cv::flip(frame, dst, 1);

        // _DEBUG_TIMER_INIT();
        // _PRINT_TIME("face.detect()",
        // face.detect(frame, false, 1 == 0 ? false : true , 1 == 1 ? false : true);
        face.detect(dst, false, Controlwindow.Landmark_Mode == 0 ? false : true , Controlwindow.Landmark_Mode == 1 ? false : true);
        // );


        // face.mosaics(face.enclosing_box());
        // if(face.landmarks_Detected())face.hentai();

        // _PRINT_TIME("face.objectOperation()",
        switch(Controlwindow.AR_Mode){
            case 1:
                face.objectOperation(&face_program, cv::imread("dataset/v.png") , true);
                break;
            case 2:
                face.objectOperation(&face_program, cv::imread("dataset/無臉男.png") , true);
                break;
            case 3:
                face.objectOperation(&face_program, cv::imread("dataset/spider.png") , true);
                break;
            case 4:
                face.mosaics(face.enclosing_box());
                break;
            case 5:
                if(face.landmarks_Detected())face.hentai();
                break;
        }

        switch(Controlwindow.Mesh_Mode){
            case 1:
                face_program.setUniformInt("mode", 0);
                face_program.setRenderMode(GL_POINTS);
                face.objectOperation(&face_program, cv::Mat(1, 1, CV_8UC3, cv::Scalar( Controlwindow.getColor().b,  Controlwindow.getColor().g,  Controlwindow.getColor().r)) , true);
                break;
            case 2:
                face_program.setUniformInt("mode",2);
                face_program.setRenderMode(GL_LINES);
                face.objectOperation(&face_program, cv::Mat(1, 1, CV_8UC3, cv::Scalar( Controlwindow.getColor().b,  Controlwindow.getColor().g,  Controlwindow.getColor().r)) , true);
                break;
            case 3:
                face_program.setUniformInt("mode",2);
                face_program.setRenderMode(GL_TRIANGLES);
                face.objectOperation(&face_program, cv::Mat(1, 1, CV_8UC3, cv::Scalar( Controlwindow.getColor().b,  Controlwindow.getColor().g,  Controlwindow.getColor().r)) , true);
                break;
        }
        float df_value = Controlwindow.getDiffuseFactor();
        float sf_value = Controlwindow.getSpecularFactor();
        vec3 df = vec3(df_value,df_value,df_value);
        vec3 sf = vec3(sf_value,sf_value,sf_value);
        face_program.setUniformVec3("materialDiffuseFactor", df);
        face_program.setUniformVec3("materialSpecularFactor",sf);
        // );
        // if(face.hasMesh())
        // face.object()->setTexture();
        //set camera frame
        obj_program.object(screen_index)->setTexture(dst);
        obj_program.render();


        window.render();

    }
    while (!window.shouldClose());





    return 0;
}


void runMenu(){
    // Gtk::Main::run(window);
     gtk_main_iteration();

}
