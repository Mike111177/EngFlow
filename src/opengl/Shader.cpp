#include <cstdio>

#include <Shader.h>

GLUtil::Shader::Shader(): shaderid(NULL), compiled(false){}

void GLUtil::Shader::compile(char const * const vertexSource, char const* const fragmentSource){
    if (compiled) glDeleteShader(shaderid);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    shaderid = glCreateProgram();
    glAttachShader(shaderid, vertexShader);
    glAttachShader(shaderid, fragmentShader);
    glBindFragDataLocation(shaderid, 0, "outColor");
    glLinkProgram(shaderid);

    glDetachShader(shaderid, vertexShader);
    glDetachShader(shaderid, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    compiled = true;
}
GLUtil::Shader::~Shader(){
    glDeleteShader(shaderid);
}
GLuint GLUtil::Shader::id(){
    return shaderid;
}
bool GLUtil::Shader::isCompiled(){
    return compiled;
}
void GLUtil::Shader::select(){
    glUseProgram(shaderid);
}

GLuint GLUtil::Shader::getAttrib(const char* name){
    return glGetAttribLocation(shaderid, name);
}

GLuint GLUtil::Shader::getUniform(const char* name){
    return glGetUniformLocation(shaderid, name);
}
