#version 150 compatibility

precision highp float;

in vec3 normal;
in vec3 vertex;

out vec4 diffuse;
out vec3 normals;
out float lindepth;

uniform float time;

void main() {
  vec3 n = normalize(normal);
  vec3 L = (gl_LightSource[0].position.xyz - vertex);
  float dist = 0.05 * length(L);
  float atten = min(1.0/dist, 1.0);
  L = normalize(L);
  //L = normalize(gl_LightSource[0].position.xyz);
  vec3 R = -reflect(n,L);
  vec4 diff = vec4(0, 1, 0, 1) * max(dot(L, n), 0.0);
  diff *= max(dot(L, vec3(0, 0, 1)), 0.0);
  vec4 spec = vec4(0, 0.3, 0, 1) * 0.5 * pow(max(dot(R, L), 0.0), 8);

  normals = normalize(cross(dFdx(vertex), dFdy(vertex)));
  diffuse = vec4((diff+spec).rgb, 1.0);
  lindepth = -vertex.z;
}
