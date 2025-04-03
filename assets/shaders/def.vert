#version 460 core

#extension GL_ARB_bindless_texture : require



layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aUv;



uniform mat3 models[300];



out vec2 uv;
out flat uint instanceID;

void main() {
  vec3 position = models[gl_InstanceID] * vec3(aPosition, 1.0);
  
  gl_Position = vec4(position, 1.0);
  
  uv = aUv;
  instanceID = gl_InstanceID;
}
