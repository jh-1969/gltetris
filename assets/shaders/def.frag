#version 460 core

#extension GL_ARB_bindless_texture : require

layout(binding = 0, std430) readonly buffer ssbo1 {
  sampler2D textures[];
};

uniform vec3 colors[300];
uniform uint texIndexes[300];



in vec2 uv;
in flat uint instanceID;

out vec4 fragColor;

void main() {
  vec3 texColor = texture(textures[texIndexes[instanceID]], uv).rgb;
  fragColor = vec4(texColor * colors[instanceID], 1.0);
}
