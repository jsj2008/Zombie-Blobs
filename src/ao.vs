#version 150 compatibility

precision highp float;

out vec2 texUV;
out vec2 tex;
out vec3 pos;

const vec2 g_Resolution = vec2(1024.0, 800.0);
const vec2 g_FocalLen = vec2(1.0 / 0.414213562373095 * g_Resolution.y / g_Resolution.x,
                             1.0 / 0.414213562373095);

void main() {
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
  texUV = (gl_TextureMatrix[0] * gl_MultiTexCoord0).xy;
  tex = texUV * 2.0 - vec2(1.0);
  tex /= g_FocalLen;
}
