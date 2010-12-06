varying vec3 normal;
varying vec3 vertex;
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
	gl_FragData[0] = diff + spec;
	gl_FragData[0].a = 1.0;
//	gl_FragData[0] = gl_Color;
	gl_FragData[0].rgb *= atten;
  gl_FragData[1].xyz = n;
}
