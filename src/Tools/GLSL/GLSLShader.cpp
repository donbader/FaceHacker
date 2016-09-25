#include "GLSLShader.h"


GLSLShader::GLSLShader(const string& fileName){
	_id = compile(fileName);
}


GLint GLSLShader::type(const string& fileName){
	//GET ".vs" or ".fs"

    string file_type = fileName.substr(fileName.find_last_of(".") + 1);
    if (file_type.length() != 2 || file_type[1] != 's') {
        if(!fileName.compare("none"))
            cout << "Havn't link geometry shader\n";
        else
            cout << "ERROR:: " << fileName << " is not valid file type!!\n";
        return -1;
    }
    else{
    	switch(file_type[0]){
    		case 'v': return GL_VERTEX_SHADER;
            case 'f': return GL_FRAGMENT_SHADER;
    		case 'g': return GL_GEOMETRY_SHADER;
    		default : return -1;
    	}
    }

}


GLchar* GLSLShader::readFile(const string& fileName){
	try{

	    fstream file(fileName, ios::in);
	    int fileSize = ifstream(fileName, ifstream::ate | ifstream::binary).tellg();
	    GLchar* fileText = new GLchar[fileSize + 1] {0};
	    file.read(fileText, fileSize);
	    file.close();
    	return fileText;
	}
	catch(ifstream::failure e){
		cout << "ERROR::SHADER::FILE " << fileName << " NOT EXIST\n"  << endl;
        return NULL;
	}
}


GLuint GLSLShader::compile(const string& file){
	GLint shader_type = type(file);
	if(shader_type == -1) return -1;

    cout << "Compiling ["<<file<<"]...\n";
    GLchar* shaderSource;
    GLuint shader = glCreateShader(shader_type);

    shaderSource = readFile(file);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // Check compile success or not
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl;
    }
    else
    	cout << "Compile successfully.\n";


    return shader;
}