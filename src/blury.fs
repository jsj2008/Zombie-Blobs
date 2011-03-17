#version 150 compatibility

precision highp float;

uniform sampler2D lindepth;
uniform sampler2D blurred;
uniform sampler2D diffuse;

out vec4 color;

const vec2 g_Resolution = vec2(1024.0, 800.0);
const vec2 g_InvResolution = 1.0 / g_Resolution;
const float g_BlurRadius = 16.0;
const float g_BlurFalloff = 1.0f / (2*((g_BlurRadius+1.0)/2.0)*((g_BlurRadius+1.0)/2.0));
const float g_Sharpness = 32.0;

float fetch_eye_z(vec2 uv) {
  return texture2D(lindepth, uv).r;
}

float BlurFunction(vec2 uv, float r, float center_d, inout float w_total) {
  float c = texture2D(blurred, uv).r;
  float d = fetch_eye_z(uv);

  float ddiff = d - center_d;
  float w = exp(-r*r*g_BlurFalloff - ddiff*ddiff*g_Sharpness);
  w_total += w;

  return w*c;
}

void main() {
//  color = vec4(vec3(texture2D(blurred, gl_TexCoord[0].st).r), 1.0);

  vec4 base = vec4(texture2D(diffuse, gl_TexCoord[0].st).rgb, 1.0);

  float b = 0;
  float w_total = 0;
  float center_d = fetch_eye_z(gl_TexCoord[0].st);
    
  for (float r = -g_BlurRadius; r <= g_BlurRadius; ++r) {
    vec2 uv = gl_TexCoord[0].st + vec2(0, r*g_InvResolution.y); 
    b += BlurFunction(uv, r, center_d, w_total);
  }
  color = vec4(b/w_total)*base;
//  color = vec4(b/w_total);
}
