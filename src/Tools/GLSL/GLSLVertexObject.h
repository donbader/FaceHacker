#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <opencv2/opencv.hpp>
#include "../Debugger.h"


class GLSLVertexBufferObject{
public:
	template<class CType>
	GLSLVertexBufferObject(vector<CType> data, GLuint type) : _type(type){
		glGenBuffers(1, &_id);
		convertAndSet(data);
	}

	GLSLVertexBufferObject(GLfloat* data, GLuint size, GLuint dimension)
		: _type(GL_ARRAY_BUFFER), _size(size), _dimension(dimension)
	{
		glGenBuffers(1, &_id);
		_data = new GLfloat[_size];
		for(int i=0;i<_size;++i)
			_data[i] = data[i];

	}

	GLSLVertexBufferObject(GLuint* data, GLuint size, GLuint dimension)
		: _type(GL_ELEMENT_ARRAY_BUFFER), _size(size), _dimension(dimension)
	{
		glGenBuffers(1, &_id);
		_elementData = new GLuint[_size];
		for(int i=0;i<_size;++i)
			_elementData[i] = data[i];

	}


	~GLSLVertexBufferObject(){
		if(_data) delete[] _data;
		if(_elementData) delete[] _elementData;
		glDeleteBuffers(1, &_id);
		_DEBUG_MSG("____VBO["<< _id <<"] deleted.");
	}

	void bindAt(GLuint index){
		glBindBuffer(_type, _id);
		switch(_type){
			case GL_ELEMENT_ARRAY_BUFFER:
			    glBufferData(_type, sizeof(GLuint) * _size, _elementData, GL_STATIC_DRAW);
			    break;
			case GL_ARRAY_BUFFER:
				glBufferData(_type, sizeof(GLfloat) * _size, _data, GL_STATIC_DRAW);
				glVertexAttribPointer(index, _dimension, GL_FLOAT, GL_FALSE, 0, 0);
				break;
			default:
				cerr << "ERROR: (unknown type) binding vbo["<< _id<< "] index:"<<index<<"\n";
				break;
		}
	}


	//getter
	GLfloat* data(){return _data;}
	GLuint* elementData(){return _elementData;}
	GLuint size(){return _size;}
	GLuint dimension(){return _dimension;}
	GLuint type(){return _type;}
	GLuint id(){return _id;}

private:

	void convertAndSet(vector<cv::Vec4f> data){
		_size = data.size() * 3;
		_dimension = 3;
		_data = new GLfloat[_size];
		for(int i=0;i<data.size();++i){
			_data[ 3*i ] =     data[i][0];
			_data[ 3*i + 1 ] = data[i][1];
			_data[ 3*i + 2 ] = data[i][2];
		}
	}
	void convertAndSet(vector<cv::Vec3f> data){
		_size = data.size() * 3;
		_dimension = 3;
		_data = new GLfloat[_size];
		for(int i=0;i<data.size();++i){
			_data[ 3*i ] =     data[i][0];
			_data[ 3*i + 1 ] = data[i][1];
			_data[ 3*i + 2 ] = data[i][2];
		}
	}
	void convertAndSet(vector<cv::Vec2f> data){
		_size = data.size() * 2;
		_dimension = 2;
		_data = new GLfloat[_size];
		for(int i=0;i<data.size();++i){
			_data[ 2*i ] =     data[i][0];
			_data[ 2*i + 1 ] = data[i][1];
		}
	}
	void convertAndSet(vector<array<int, 3>> data){
		_size = data.size() * 3;
		_dimension = 3;
		_elementData = new GLuint[_size];
		for(int i=0;i<data.size();++i){
			_elementData[ 3*i ] = data[i][0];
			_elementData[ 3*i + 1 ] = data[i][1];
			_elementData[ 3*i + 2 ] = data[i][2];
		}
	}




	GLuint _id;
	GLuint _type;
	GLuint _size;
	GLuint _dimension;
	GLfloat* _data = NULL;
	GLuint* _elementData = NULL;

};



class GLSLVertexArrayObject{
public:

	const static GLuint VBO_POS 		= 0;
	const static GLuint VBO_TEXCOORDS 	= 1;
	const static GLuint VBO_NORMALS 	= 2;
	const static GLuint VBO_BONE_INDEX  = 3;
	const static GLuint VBO_BONE_WEIGHT = 4;
	const static GLuint VBO_INDICE      = 5;

	const static GLuint VBO_NUM = VBO_INDICE + 1;

	GLSLVertexArrayObject(): model() {
		glGenVertexArrays(1, &_id);
		glGenTextures(1, &_texture);
	}
	~GLSLVertexArrayObject(){
		for(int i=0;i<VBO_NUM;++i){
			if(_VBOs[i])
				delete _VBOs[i];
		}
		glDeleteVertexArrays(1, &_id);
		glDeleteTextures(1, &_texture);
		_DEBUG_MSG("__VAO["<< _id <<"] deleted.");
	}


	void setBuffer(GLuint index, GLSLVertexBufferObject* vbo){
		_VBOs[index] = vbo;
		//start binding//////////////
	    glBindVertexArray(_id);
	    /////////////////////////////

	    //enable attribute[buffer_index]
	    if(index != VBO_INDICE) glEnableVertexAttribArray(index);
	    vbo->bindAt(index);

		//end binding////////////////
	    glBindVertexArray(0);
	    /////////////////////////////
	}

	void setTexture(cv::Mat img){
		//start binding//////////////
	    glBindVertexArray(_id);
	    /////////////////////////////
        glBindTexture( GL_TEXTURE_2D, _texture);
        GLenum format = (img.channels() == 3? GL_BGR: GL_BGRA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.cols, img.rows, 0, format, GL_UNSIGNED_BYTE, img.ptr());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);

		//end binding////////////////
	    glBindVertexArray(0);
	    /////////////////////////////
	}
	void scale_Model(GLfloat x, GLfloat y, GLfloat z ){
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(x,y,z));
		this->model = scaleMatrix * this->model;
	}

	static glm::mat4 rotateMatrix(GLfloat radx, GLfloat rady, GLfloat radz){
		auto r_x = glm::rotate(glm::mat4(1.0f), radx, glm::vec3(1,0,0));
		auto r_y = glm::rotate(glm::mat4(1.0f), rady, glm::vec3(0,1,0));
		auto r_z = glm::rotate(glm::mat4(1.0f), radz, glm::vec3(0,0,1));
		return r_z * r_x * r_y;
	}

	void rotate_Model(GLfloat radx, GLfloat rady, GLfloat radz){
		auto r_x = glm::rotate(glm::mat4(1.0f), radx, glm::vec3(1,0,0));
		auto r_y = glm::rotate(glm::mat4(1.0f), rady, glm::vec3(0,1,0));
		auto r_z = glm::rotate(glm::mat4(1.0f), radz, glm::vec3(0,0,1));
		this->model = r_z * r_x * r_y * this->model;
	}

	static glm::mat4 quatRotateMatrix(GLfloat pitch, GLfloat yaw, GLfloat roll){
		glm::quat Quat = glm::quat(glm::eulerAngleYXZ(yaw, pitch, roll));
		glm::mat4 RotationMatrix = glm::toMat4(Quat);
		return RotationMatrix;
	}
	void quatRotate_Model(GLfloat pitch, GLfloat yaw, GLfloat roll){
		glm::quat Quat = glm::quat(glm::eulerAngleYXZ(yaw, pitch, roll));
		glm::mat4 RotationMatrix = glm::toMat4(Quat);
		this->model = RotationMatrix * this->model;
	}

	void translate_Model(GLfloat x, GLfloat y, GLfloat z ){
		glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,z));
		this->model = translateMatrix * this->model;
	}

	void translate_Bone(int index, GLfloat x, GLfloat y, GLfloat z ){
		glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,z));
		this->bones[index] = translateMatrix * this->bones[index];
	}



	//getter
	GLuint id(){return _id;}
	GLuint texture(){return _texture;}
	GLuint sizeOf(GLuint index){return _VBOs[index]->size();}


	glm::mat4 model;
	glm::mat4 bones[10];

private:
	GLuint _id;
	GLuint _texture;
	GLSLVertexBufferObject* _VBOs[VBO_NUM] = {NULL};
};

