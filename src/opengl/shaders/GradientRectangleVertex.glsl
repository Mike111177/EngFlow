#version 330 core
uniform uint xy;
uniform vec3 c1;
uniform vec3 c2;
in vec2 position;
varying highp vec4 colorGradient;
void main(){
    colorGradient = vec4(mix(c2,c1,(position[xy]+1)/2), 1.0f); 
    gl_Position = vec4(position, 0.0, 1.0);
}