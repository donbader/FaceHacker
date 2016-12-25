#include "FaceProgram.h"

FaceProgram::FaceProgram(
    const string& vsFile,
    const string& fsFile,
    const string& gsFile
    )
    : GLSLProgram(vsFile,fsFile)
{
    bone_index_array = genBoneIndexArray(3448);
    bone_weights = genBoneWeights(3448);
}


int FaceProgram::addObj(Mesh mesh){
    GLSLVertexArrayObject* vao = new GLSLVertexArrayObject();
    GLSLVertexBufferObject* pos = new GLSLVertexBufferObject(mesh.vertices, GL_ARRAY_BUFFER);
    GLSLVertexBufferObject* tcd = new GLSLVertexBufferObject(mesh.texcoords, GL_ARRAY_BUFFER);
    GLSLVertexBufferObject* bon = new GLSLVertexBufferObject(bone_index_array ,GL_ARRAY_BUFFER);
    GLSLVertexBufferObject* wei = new GLSLVertexBufferObject(bone_weights, GL_ARRAY_BUFFER);
    GLSLVertexBufferObject* idc = new GLSLVertexBufferObject(mesh.tvi, GL_ELEMENT_ARRAY_BUFFER);
    GLfloat* normals = computeVertexNormals(    pos->data(),
                                                pos->size(),
                                                idc->elementData(),
                                                idc->size()
                                                );

    GLSLVertexBufferObject* nmls = new GLSLVertexBufferObject(normals, pos->size(), 3);
    delete[] normals;

    vao->setBuffer(vao->VBO_POS, pos);
    vao->setBuffer(vao->VBO_TEXCOORDS, tcd);
    vao->setBuffer(vao->VBO_NORMALS, nmls);
    vao->setBuffer(vao->VBO_BONE_INDEX, bon);
    vao->setBuffer(vao->VBO_BONE_WEIGHT, wei);
    vao->setBuffer(vao->VBO_INDICE, idc);


    // cv::Mat white(1,1,CV_8UC3,cv::Scalar(0,0,0));
    // vao->setTexture(white);
    // cv::Mat texture = cv::imread("dataset/test.png");
    // vao->setTexture(texture);

    _VAOs.push_back(vao);

    return _VAOs.size() - 1;
}

void FaceProgram::render(){
    glUseProgram(_id);
    for (int i = 0; i < _VAOs.size(); ++i) {
        glBindVertexArray(_VAOs[i]->id());
        glBindTexture(GL_TEXTURE_2D, _VAOs[i]->texture());

        setUniformMat4("M", _VAOs[i]->model);
        setUniformMat4("V", _camera.View());
        setUniformMat4("P", _camera.Projection());

        for(int j=0; j<10 ; ++j){
            setUniformMat4(("Bone["+to_string(j)+"]").c_str(), _VAOs[i]->bones[j]);
        }
        glDrawElements(GL_TRIANGLES, _VAOs[i]->sizeOf(_VAOs[i]->VBO_INDICE), GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}




vector<cv::Vec2f> FaceProgram::genBoneIndexArray(int meshSize){
    vector<cv::Vec2f> vec;
    for(int i=0;i<meshSize;++i){
        switch(i){
            case 315: case 413: case 329: case 825: case 736:
            case 841: case 693: case 411: case 264: case 431:
                vec.push_back(cv::Vec2f(5,0));
                break;
            //left eye
            //right eye
            // case 614: case 624: case 605: case 610: case 607: case 606:
            // case 181: case 191: case 177:
                case 170:
                case 171:
                case 172:
                case 173:
                case 174:
                case 175:
                case 176:
                // case 177:
                // case 178:
                // case 179:
                // case 180:
                vec.push_back(cv::Vec2f(2,0));
                break;
            // case 170 ... 176: case 178 ... 180:
            //     vec.push_back(cv::Vec2f(3,0));
            //     break;
            case 540:
                vec.push_back(cv::Vec2f(3,0));break;
            default:
                vec.push_back(cv::Vec2f(0,0)); break;
        }
    }

    return vec;
}

vector<cv::Vec2f> FaceProgram::genBoneWeights(int meshSize){
    vector<cv::Vec2f> vec;
    for(int i=0;i<meshSize;++i){
        vec.push_back(cv::Vec2f(1,0));
    }
    return vec;
}