#include "GLSLProgram.h"

GLSLProgram::GLSLProgram(const string& vsFile, const string& fsFile, const string& gsFile){
	_id = glCreateProgram();
	GLSLShader vertexShader(vsFile);
	GLSLShader fragmentShader(fsFile);
	GLSLShader geometryShader(gsFile);
    link(vertexShader.id(), fragmentShader.id(), geometryShader.id());
}

GLSLProgram::~GLSLProgram(){
    glDeleteProgram(_id);
    for(int i=0;i<_VAOs.size();++i)
        delete _VAOs[i];
}

void GLSLProgram::link(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader){
    glAttachShader(_id, vertexShader);
    glAttachShader(_id, fragmentShader);
    if(geometryShader != -1) glAttachShader(_id, geometryShader);
    glLinkProgram(_id);

    // Check for linking errors
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_id, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if(geometryShader != -1)glDeleteShader(geometryShader);
}

GLint GLSLProgram::getLocation(const char* name){
    return glGetUniformLocation(_id, name);
}

void GLSLProgram::setUniformMat4(const char *name, glm::mat4 &mat){
    glUseProgram(_id);
    GLint loc = getLocation(name);
    if(loc < 0 )return;
    glUniformMatrix4fv(loc, 1, GL_FALSE, &(mat[0][0]));
}

void GLSLProgram::setUniformVec3(const char *name, glm::vec3 &vec){
    glUseProgram(_id);
    GLint loc = getLocation(name);
    if(loc < 0 )return;
    glUniform3f(loc, vec.x, vec.y, vec.z);
}

void GLSLProgram::setUniformFloat(const char *name, float f){
    glUseProgram(_id);
    GLint loc = getLocation(name);
    if(loc < 0 )return;
    glUniform1f(loc, f);
}

void GLSLProgram::setUniformInt(const char *name, int i){
    glUseProgram(_id);
    GLint loc = getLocation(name);
    if(loc < 0 )return;
    glUniform1i(loc, i);
}



GLSLVertexArrayObject* GLSLProgram::object(int index){
    if(index<_VAOs.size() && index >= 0)
        return _VAOs[index];
    else
        return NULL;
}



void GLSLProgram::render(){
    glUseProgram(_id);
    for (int i = 0; i < _VAOs.size(); ++i) {
        glBindVertexArray(_VAOs[i]->id());
        glBindTexture(GL_TEXTURE_2D, _VAOs[i]->texture());

        setUniformMat4("M", _VAOs[i]->model);
        setUniformMat4("V", _camera.View());
        setUniformMat4("P", _camera.Projection());

        glDrawElements(GL_TRIANGLES, _VAOs[i]->sizeOf(_VAOs[i]->VBO_INDICE), GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}


GLfloat* GLSLProgram::computeVertexNormals(GLfloat* vertex, GLuint vertex_size, GLuint* indice, GLuint indice_size){
    GLfloat* normals = new GLfloat[vertex_size];
    vector<vector<array<int, 3>>> triangles(vertex_size/3);
    for(int i=0; i<indice_size; ++i){
        array<int, 3> triangle;
        triangle[0] = indice[i-(i%3)];
        triangle[1] = indice[i+1-(i%3)];
        triangle[2] = indice[i+2-(i%3)];
        triangles[indice[i]].push_back(triangle);
    }

    for(int i=0; i<vertex_size/3 ;++i){
        normals[i*3] = 0;
        normals[i*3+1] = 0;
        normals[i*3+2] = 0;
        for(int j=0;j<triangles[i].size();++j){
            int index_0 = triangles[i][j][0] * 3 ;
            int index_1 = triangles[i][j][1] * 3 ;
            int index_2 = triangles[i][j][2] * 3 ;
            glm::vec3 v1 = glm::vec3(
                            vertex[index_1] - vertex[index_0],
                            vertex[index_1+1] - vertex[index_0+1],
                            vertex[index_1+2] - vertex[index_0+2]
                            );
            glm::vec3 v2 = glm::vec3(
                            vertex[index_2] - vertex[index_0],
                            vertex[index_2+1] - vertex[index_0+1],
                            vertex[index_2+2] - vertex[index_0+2]
                            );

            glm::vec3 normal = glm::cross(v1,v2);
            normals[i*3] += normal.x;
            normals[i*3+1] += normal.y;
            normals[i*3+2] += normal.z;
        }

    }
    return normals;
}





int GLSLProgram::addObj(
    GLfloat* positions, GLuint positions_size,
    GLfloat* texcoords, GLuint texcoords_size,
    GLfloat* normals, GLuint normals_size,
    GLuint* indices, GLuint indices_size,
    cv::Mat& img)
{
    GLSLVertexArrayObject* vao = new GLSLVertexArrayObject();
    GLSLVertexBufferObject* pos = new GLSLVertexBufferObject(positions, positions_size, 3);
    GLSLVertexBufferObject* tcd = new GLSLVertexBufferObject(texcoords, texcoords_size, 2);
    GLSLVertexBufferObject* idc = new GLSLVertexBufferObject(indices, indices_size, 3);
    GLSLVertexBufferObject* nmls = new GLSLVertexBufferObject(normals, normals_size, 3);


    vao->setBuffer(vao->VBO_POS, pos);
    vao->setBuffer(vao->VBO_TEXCOORDS, tcd);
    vao->setBuffer(vao->VBO_NORMALS, nmls);
    vao->setBuffer(vao->VBO_INDICE, idc);
    vao->setTexture(img);

    _VAOs.push_back(vao);
    return _VAOs.size() - 1;
}

void GLSLProgram::deleteObj(int index){
    if(index >= 0 && index < _VAOs.size()){
        delete _VAOs[index];
        _VAOs.erase(_VAOs.begin() + index);
    }
}

