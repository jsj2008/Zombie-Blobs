#version 150 compatibility

precision highp float;

uniform sampler2D lindepth;
uniform sampler2D image;

out float blurred;

const vec2 g_Resolution = vec2(1024.0, 800.0);
const vec2 g_InvResolution = 1.0 / g_Resolution;
const float g_BlurRadius = 16.0;
const float g_BlurFalloff = 1.0f / (2*((g_BlurRadius+1.0)/2.0)*((g_BlurRadius+1.0)/2.0));
const float g_Sharpness = 32.0;

float fetch_eye_z(vec2 uv) {
  return texture2D(lindepth, uv).r;
}

float BlurFunction(vec2 uv, float r, float center_d, inout float w_total) {
  float c = texture2D(image, uv).r;
  float d = fetch_eye_z(uv);

  float ddiff = d - center_d;
  float w = exp(-r*r*g_BlurFalloff - ddiff*ddiff*g_Sharpness);
  w_total += w;

  return w*c;
}

void main() {
  float b = 0;
  float w_total = 0;
  float center_d = fetch_eye_z(gl_TexCoord[0].st);
    
  for (float r = -g_BlurRadius; r <= g_BlurRadius; ++r) {
    vec2 uv = gl_TexCoord[0].st + vec2(r*g_InvResolution.x, 0); 
    b += BlurFunction(uv, r, center_d, w_total);
  }
  blurred = b/w_total;
}
