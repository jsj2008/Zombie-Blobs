#version 150 compatibility

precision highp float;

uniform sampler2D diffuse;  // vec4
uniform sampler2D normals;  // vec3
uniform sampler2D lindepth; // float
uniform sampler2D ao;       // float

const vec2 size = vec2(0.35, 0.35);
const vec2 pos = vec2(0.01, 1.0-0.36);

void render3(sampler2D sampler, vec2 uv, float min = 0.0, float max = 1.0) {
  gl_FragColor = vec4((texture2D(sampler, uv).rgb-min)/(max-min), 1.0);
}

void render1(sampler2D sampler, vec2 uv, float min = 0.0, float max = 1.0) {
  gl_FragColor = vec4(vec3((texture2D(sampler, uv).r-min)/(max-min)), 1.0);
}

void main()
{
  vec2 uv = gl_TexCoord[0].st;
  if (uv.s >= pos.x && uv.s < size.x/2+pos.x) {
    if (uv.t >= pos.y && uv.t < size.y/2+pos.y)
      render3(diffuse, (uv-pos)/size*2);
    else if (uv.t > pos.y && uv.t < size.y+pos.y)
      render3(normals, ((uv-pos)-vec2(0, size.y/2))/size*2, -1.0, 1.0);
    else discard;
  } else if (uv.s > pos.x && uv.s < size.x+pos.x) {
    if (uv.t >= pos.y && uv.t < size.y/2+pos.y)
      render1(lindepth, ((uv-pos)-vec2(size.x/2, 0))/size*2, 0.1, 200.0);
    else if (uv.t >= pos.y && uv.t < size.y+pos.y)
      render1(ao, ((uv-pos)-vec2(size.x/2, size.y/2))/size*2);
    else discard;
  } else discard;
}
