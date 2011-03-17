#version 150 compatibility

precision highp float;

in vec3 normal;
in vec3 vertex;
in vec3 world;

out vec4 diffuse;
out vec3 normals;
out float lindepth;

uniform float time;

uniform sampler1D permSampler, gradSampler;

vec3 fade(vec3 t) {
  return t * t * t * (t * (t * 6 - 15) + 10);
}

float perm(float x) {
  return texture1D(permSampler, x / 256.0).r * 256.0;
}

float grad(float x, vec3 p) {
  return dot(texture1D(gradSampler, x).xyz, p);
}

float inoise(vec3 p) {
  // todo: is this same as fmod in hlsl?
  vec3 P = mod(floor(p), 256.0);
  p -= floor(p);
  vec3 f = fade(p);

  // HASH COORDINATES FOR 6 OF THE 8 CUBE CORNERS

  float A = perm(P.x) + P.y;
  float AA = perm(A) + P.z;
  float AB = perm(A + 1) + P.z;
  float B =  perm(P.x + 1) + P.y;
  float BA = perm(B) + P.z;
  float BB = perm(B + 1) + P.z;

  // AND ADD BLENDED RESULTS FROM 8 CORNERS OF CUBE

  return mix(
    mix(mix(grad(perm(AA), p),
            grad(perm(BA), p + vec3(-1, 0, 0)), f.x),
        mix(grad(perm(AB), p + vec3(0, -1, 0)),
            grad(perm(BB), p + vec3(-1, -1, 0)), f.x), f.y),
    mix(mix(grad(perm(AA + 1), p + vec3(0, 0, -1)),
            grad(perm(BA + 1), p + vec3(-1, 0, -1)), f.x),
        mix(grad(perm(AB + 1), p + vec3(0, -1, -1)),
            grad(perm(BB + 1), p + vec3(-1, -1, -1)), f.x), f.y),
    f.z);
}

float turbulence(vec3 v) {
  const int depth = 2;

  float sum = 0.0, amp = 1.0, fscale = 1.0;
  float tmp = 0.5;
  for (int i = 0; i <= depth; i++, amp *= 0.5, fscale *= 2.0) {
    sum += inoise(v * fscale);
    tmp *= 2.0;
  }

  return sum * tmp / (tmp * 2.0 - 1.0);
}

float marble_intensity(vec3 v) {
//  v.z *= 0.1;
  v.y *= 0.5;
  v.x *= 0.5;

  float n = 5.0 * (v.x + v.y + v.z);
  float mi = n + 5.0 * turbulence(v);
  return 0.1 + 0.1 * sin(mi);
}

void main() {
  vec3 n = normalize(normal);
#if 0
  float lt = time * 0.5f;
  float scale = 0.3f;
  vec3 cols = vec3(sin(lt + 1.4*scale*vertex.x), sin(lt + scale*vertex.y), sin(lt + scale*vertex.z));
#endif

  float depth = -vertex.z/200.0;
  vec3 fog = vec3(1.0 - depth * depth);

  vec3 L = (gl_LightSource[0].position.xyz - vertex);
  float dist = 0.05 * length(L);
  float atten = min(1.0/dist, 1.0);
  L = normalize(L);
//  L = mix(normalize(L), n, marble_intensity(world*0.5));
  vec3 R = -reflect(n,L);
  vec4 diff = gl_Color * max(dot(L, n), 0.0);
  vec4 spec = 0.3 * gl_Color * pow(max(dot(R, L), 0.0), 8);
//  float id = 1.0 - dist*0.5;
//  spec += vec4(inoise(world*30)) * max(0.0, id*id*id) * 0.1;

  // Use face normals instead of interpolated normals to make sure that the tangent planes
  // associated with the normals are actually tangent to the surface
  normals = normalize(cross(dFdx(vertex), dFdy(vertex)));
//  diffuse.rgb = mix(vec3(turbulence(world)), (diff+spec).rgb, clamp(dist, 0, 1));
  diffuse = vec4((diff+spec).rgb * fog, 1.0);
  lindepth = -vertex.z; //(-vertex.z-0.1)/(200.0-0.1); // vertex.z;
}
