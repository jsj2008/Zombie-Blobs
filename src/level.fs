#version 120
varying vec3 normal;
varying vec3 vertex;
uniform float time;

void main() {
  vec3 n = normalize(normal);
#if 0
  float lt = time * 0.5f;
  float scale = 0.3f;
  vec3 cols = vec3(sin(lt + 1.4*scale*vertex.x), sin(lt + scale*vertex.y), sin(lt + scale*vertex.z));
#endif

  vec3 L = (gl_LightSource[0].position.xyz - vertex);
  float dist = 0.05 * length(L);
  float atten = min(1.0/dist, 1.0);
  L = normalize(L);
  vec3 R = -reflect(n,L);
  vec4 diff = gl_Color * max(dot(L, n), 0.0);
  vec4 spec = 0.3 * gl_Color * pow(max(dot(R, L), 0.0), 8);
  gl_FragData[0] = diff + spec;
	gl_FragData[0].a = 1.0;
//	gl_FragData[0] = gl_Color;
	gl_FragData[0].rgb *= atten;
  gl_FragData[1].xyz = n;	
}
