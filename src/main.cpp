#include "main.h"
using namespace glm;

std::mutex gMutex;
void test(std::vector<cv::Mat>& vec,std::vector<cv::Mat>& vec2,Face & face,cv::VideoCapture & cap);
void test2(std::vector<cv::Mat>& vec2,Window & window);


Window window(1920, 540, "Face Hacker");
int main( int argc, char ** argv )
{	
    Window window(1920, 540, "Face Hacker");
    window.setBackground(0.5,0.5,0.5);

    // get


    GLSLProgram vid_program("dataset/screen.vs","dataset/screen.fs");
    GLSLProgram cam_program("dataset/screen.vs","dataset/screen.fs");
    FaceProgram face_program("dataset/obj.vs","dataset/obj.fs");
    Face face("dataset/shape_predictor_68_face_landmarks.dat",
                "dataset/eos/share/ibug2did.txt",
                "dataset/eos/share/model_contours.json",
                "dataset/eos/share/sfm_shape_3448.bin",
                "dataset/eos/share/expression_blendshapes_3448.bin");
    Face face2("dataset/shape_predictor_68_face_landmarks.dat",
                "dataset/eos/share/ibug2did.txt",
                "dataset/eos/share/model_contours.json",
                "dataset/eos/share/sfm_shape_3448.bin",
                "dataset/eos/share/expression_blendshapes_3448.bin");


    vid_program.camera().setPosition(0,0,320);
    cam_program.camera().setPosition(0,0,320);
    face_program.camera().setPosition(0,0,320);
    GLfloat vertices[] = {
        -320, -180, 0,
        0, -180, 0,
        0, 180, 0,
        -320, 180, 0,
    };

GLfloat vertices2[] = {
        320, -180, 0,
        0, -180, 0,
        0, 180, 0,
        320, 180, 0,
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
    int screen_index = vid_program.addObj(	vertices, 12,
										texcoords, 8,
										normals,  12,
										indice,    6,
										white
                                    );
    int cam_screen_index = cam_program.addObj(  vertices2, 12,
                                        texcoords, 8,
                                        normals,  12,
                                        indice,    6,
                                        white
                                    );

    // vid_program.object(screen_index)->translate_Model(0,0,1000);

    cv::Mat frame;
    cv::Mat frame2;
    cv::Mat dectect_frame;
    cv::Mat dectect_frame2;
    cv::VideoCapture cap;
    cv::VideoCapture cam_cap;
    // cap.open(0);
    cap.open("dataset/test.mp4");
    cam_cap.open(0);
    
    //game loop
    GLint k=100;
    // int tmp=-1;
    std::vector<cv::Mat> vec;
    std::vector<cv::Mat> vec2;
    bool a;
    bool b;


// #pragma omp parallel sections
//         {
//             #pragma omp section
//             {
//                 do{
//                     // if(vec.size() >= 50 ) { std::cout<<"vec.size:"<<vec.size()<<endl; continue; }
//                     tmp++;
//                     if(tmp%10!=0){continue;}
//                     a=cap.read(frame);
//                     b=cam_cap.read(frame2);
//                     cout<<"test"<<endl;
//                     if(!a || !b){break;}   
//                     #pragma omp critical
//                     {
//                         vec.push(frame);
//                         vec2.push(frame2);
//                     }
//                 }
//                 while(!window.shouldClose());
//             }
//              #pragma omp section
//             {
//                 bool open=false;
//                 do{
//                     cout<<"omp2"<<endl;
//                     #pragma omp critical
//                     {
//                     if(!vec.empty() && !vec.empty()){
//                         open=true;
//                         dectect_frame=vec.front();    
//                         dectect_frame2=vec2.front();        
//                         vec.pop();
//                         vec2.pop();
//                         }
//                     }
//                     if(!open){continue;}
//                     open=false;
                    // _DEBUG_TIMER_INIT();
                    // _PRINT_TIME("face.detect()",
                    // face.detect(dectect_frame, false, true);
                    // );

                    // _PRINT_TIME("face.objectOperation()",
                    // face.objectOperation(&face_program, true);
                    // );
//                     // set video frame
//                     vid_program.object(screen_index)->setTexture(dectect_frame);
//                     vid_program.render();
//                     //set camera frame
//                     cam_program.object(screen_index)->setTexture(dectect_frame2);
//                     cam_program.render();

//                     window.render();
//                 }while(!window.shouldClose());
//             }
//         }


    // int tmp=-1;
    // #pragma omp parallel sections
    // {
    //     #pragma omp section
    //     {
    //         bool open=false;
    //         do{ //video+face mesh
    //             tmp++;
    //             if(tmp%10!=0){continue;}
    //         #pragma omp critical (vecUpdate)
    //                 {
    //                     cout<<"omp start"<<endl;
    //                     if(vec.size()<500){
    //                     open=true;
    //                     a=cap.read(frame);
    //                     vec.push_back(frame.clone());
    //                     vec2.push_back(frame.clone());
    //                     cout<<"vec.size():"<<vec.size()<<endl;
    //                     }
    //                 }
    //                 cout<<"omp end"<<endl;
    //                 if(!open){continue;}
    //                 open=false;
    //                 face.detect(frame, false, true);
                    
    //         }while(a);
    //     }
    //     // #pragma omp section
    //     // {
    //     //     bool open2=false;
    //     //     do{
    //     //         #pragma omp critical (vecUpdate)
    //     //             {
    //     //                 cout<<"i come"<<endl;
    //     //                 if(!vec2.empty()){
    //     //                     open2=true;
    //     //                     /*臉部模型建構*/
    //     //                     vec2.erase(vec2.begin(),vec2.begin()+1);
    //     //                 }
    //     //             }
    //     //             cout<<"i'm out"<<endl;
    //     //             if(!open2){continue;}
    //     //             open2=false;
    //     //     }while(!window.shouldClose());
    //     // }
    //     #pragma omp section
    //     {
    //         bool open3=false;
    //         do{
    //                 #pragma omp critical (vecUpdate)
    //                 {
    //                     cout<<"hihi"<<endl;
    //                     if(!vec.empty()){
    //                     b=cam_cap.read(frame2);
    //                     open3=true;
    //                     dectect_frame=vec.front();        
    //                     vec.erase(vec.begin(),vec.begin()+1);
    //                     cout<<"---------------vec.size():"<<vec.size()<<endl;
    //                     }
    //                 }
    //                 cout<<"byby"<<endl;
    //                 if(!open3){continue;}
    //                 open3=false;
    //                 face2.detect(frame2, false, true);
    //                 // set video frame
    //                 vid_program.object(screen_index)->setTexture(dectect_frame);
    //                 vid_program.render();
    //                 //set camera frame
    //                 cam_program.object(screen_index)->setTexture(frame2);
    //                 cam_program.render();
    //                 cout<<"render"<<endl;
    //                 window.render();
    //                 cout<<"render end"<<endl;
    //         }while(!window.shouldClose());
    //     }
    // }

    thread mThread1( test , ref(vec) , ref(vec2),ref(face),ref(cap));
    thread mThread2( test2 , ref(vec2),ref(window));

    bool open3=false;
            do{
                    cout<<"vec:"<<vec.size()<<endl;
                    gMutex.lock();
                        // cout<<"hihi"<<endl;
                        if(!vec.empty()){
                        b=cam_cap.read(frame2);
                        open3=true;
                        dectect_frame=vec.front();        
                        vec.erase(vec.begin(),vec.begin()+1);
                        cout<<"---------------vec.size():"<<vec.size()<<endl;
                        }
                    gMutex.unlock();
                    if(!open3){continue;}
                    open3=false;
                    face2.detect(frame2, false, true);
                    // set video frame
                    vid_program.object(screen_index)->setTexture(dectect_frame);
                    vid_program.render();
                    //set camera frame
                    cam_program.object(screen_index)->setTexture(frame2);
                    cam_program.render();
                    cout<<"render"<<endl;
                    window.render();
                    cout<<"render end"<<endl;
            }while(!window.shouldClose());

    return 0;
}

void test(std::vector<cv::Mat>& vec,std::vector<cv::Mat>& vec2,Face & face,cv::VideoCapture & cap){
    bool open=false;
    int tmp=-1;
    cv::Mat frame;
    bool a;
            do{ //video+face mesh
                tmp++;
                cout<<"fun vec:"<<vec.size()<<endl;
                if(tmp%10!=0){continue;}
            gMutex.lock();
                        if(vec.size()<500){
                        open=true;
                        a=cap.read(frame);
                        vec.push_back(frame.clone());
                        vec2.push_back(frame.clone());
                        }
            gMutex.unlock();
                    if(!open){continue;}
                    open=false;
                    face.detect(frame, false, true);
                    ///ddd
            }while(a);
            return;
}

void test2(std::vector<cv::Mat>& vec2,Window & window){
    bool open2=false;
            do{
                gMutex.lock();
                        if(!vec2.empty()){
                            open2=true;
                            /*臉部模型建構*/
                            vec2.erase(vec2.begin(),vec2.begin()+1);
                        }
                gMutex.unlock();
                    if(!open2){continue;}
                    open2=false;
            }while(!window.shouldClose());
            return;
}