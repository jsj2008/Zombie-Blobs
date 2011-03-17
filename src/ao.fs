#version 150 compatibility

precision highp float;

uniform sampler2D lindepth;
uniform sampler2D normals;
uniform sampler2D random;

in vec2 texUV;
in vec2 tex;
in vec3 pos;

out float value;

#define pi 3.141592653589793238462643383279

const float fovy = 45.0 * pi / 180.0;
const vec2 g_Resolution = vec2(1024.0, 800.0);
const vec2 g_InvResolution = 1.0 / g_Resolution;
//const vec2 g_FocalLen = vec2(1.0 / tan(fovy * 0.5) * g_Resolution.y / g_Resolution.x,
//                             1.0 / tan(fovy * 0.5));
const vec2 g_FocalLen = vec2(1.0 / 0.414213562373095 * g_Resolution.y / g_Resolution.x,
                             1.0 / 0.414213562373095);
const vec2 g_InvFocalLen = 1.0 / g_FocalLen;
const float g_R = 2.0;
const float g_inv_R = 1.0 / g_R;
const float g_sqr_R = g_R * g_R;
const float g_NumSteps = 12.0;
const float g_NumDir = 12.0;
const float g_Attenuation = 0.5;
const float g_AngleBias = 10.0 * pi / 180.0;
const float g_Contrast = 1.25 / (1.0f - sin(g_AngleBias));


float tangent(vec3 P, vec3 S) {
  return (P.z - S.z) / length(S.xy - P.xy);
}

vec3 uv_to_eye(vec2 uv, float eye_z) {
  uv = (uv * vec2(2.0, -2.0) - vec2(1.0, -1.0));
//  uv = (uv * 2.0 - 1.0);
  return vec3(uv * g_InvFocalLen * eye_z, eye_z);
}

vec3 fetch_eye_pos(vec2 uv) {
  float z = texture2D(lindepth, uv).r;
  return uv_to_eye(uv, z);
}

vec3 tangent_eye_pos(vec2 uv, vec4 tangentPlane) {
  // view vector going through the surface point at uv
  vec3 V = fetch_eye_pos(uv);
  //float NdotV = dot(tangentPlane.xyz, V);
  // intersect with tangent plane except for silhouette edges
  /// @todo what should this do and why does it look so silly?
  //if (NdotV < 0.0) V *= (tangentPlane.w / NdotV);
  return V;
}

float length2(vec3 v) { return dot(v, v); } 

vec3 min_diff(vec3 P, vec3 Pr, vec3 Pl) {
  vec3 V1 = Pr - P;
  vec3 V2 = P - Pl;
  return (length2(V1) < length2(V2)) ? V1 : V2;
}

float falloff(float r) {
  return 1.0f - g_Attenuation*r*r;
}

vec2 snap_uv_offset(vec2 uv) {
  return round(uv * g_Resolution) * g_InvResolution;
}

float invlength(vec2 v) {
  return inversesqrt(dot(v,v));
}

float tangent(vec3 T) {
  return -T.z * invlength(T.xy);
}

float biased_tangent(vec3 T) {
  float phi = atan(tangent(T)) + g_AngleBias;
  return tan(min(phi, pi*0.5));
}

float AccumulatedHorizonOcclusion(vec2 deltaUV, 
                                  vec2 uv0, 
                                  vec3 P, 
                                  float numSteps, 
                                  float randstep,
                                  vec3 dPdu,
                                  vec3 dPdv) {
  // Randomize starting point within the first sample distance
  vec2 uv = uv0 + snap_uv_offset(randstep * deltaUV);
    
  // Snap increments to pixels to avoid disparities between xy 
  // and z sample locations and sample along a line
  deltaUV = snap_uv_offset(deltaUV);

  // Compute tangent vector using the tangent plane
  vec3 T = deltaUV.x * dPdu + deltaUV.y * dPdv;

  float tanH = biased_tangent(T);
  float sinH = tanH / sqrt(1.0f + tanH*tanH);

  float ao = 0;
  for(float j = 1; j <= numSteps; ++j) {
    uv += deltaUV;
    vec3 S = fetch_eye_pos(uv);
        
    // Ignore any samples outside the radius of influence
    float d2  = length2(S - P);
    if (d2 < g_sqr_R) {
      float tanS = tangent(P, S);

      if(tanS > tanH) {
        // Accumulate AO between the horizon and the sample
        float sinS = tanS / sqrt(1.0f + tanS*tanS);
        float r = sqrt(d2) * g_inv_R;
        ao += falloff(r) * (sinS - sinH);
                
        // Update the current horizon angle
        tanH = tanS;
        sinH = sinS;
      }
    }
  }

  return ao;
}

vec2 rotate_direction(vec2 Dir, vec2 CosSin) {
  return vec2(Dir.x*CosSin.x - Dir.y*CosSin.y, 
              Dir.x*CosSin.y + Dir.y*CosSin.x);
}

void main() {
  vec3 P = fetch_eye_pos(texUV);

  // Project the radius of influence g_R from eye space to texture space.
  // The scaling by 0.5 is to go from [-1,1] to [0,1].
  vec2 step_size = 0.5 * g_R  * g_FocalLen / P.z;

  // Early out if the projected radius is smaller than 1 pixel.
  float numSteps = min(g_NumSteps, min(step_size.x * g_Resolution.x, step_size.y * g_Resolution.y));
  if (numSteps < 1.0) {
    value = 1.0;
    return;
  }

  step_size /= numSteps + 1.0;

  // Nearest neighbor pixels on the tangent plane
  vec3 N = texture2D(normals, texUV).rgb;
  vec4 tangentPlane = vec4(N, dot(P, N));
  vec3 Pr = tangent_eye_pos(texUV + vec2(g_InvResolution.x, 0), tangentPlane);
  vec3 Pl = tangent_eye_pos(texUV + vec2(-g_InvResolution.x, 0), tangentPlane);
  vec3 Pt = tangent_eye_pos(texUV + vec2(0, g_InvResolution.y), tangentPlane);
  vec3 Pb = tangent_eye_pos(texUV + vec2(0, -g_InvResolution.y), tangentPlane);

/*  vec3 Pr = fetch_eye_pos(texUV + vec2(g_InvResolution.x, 0));
  vec3 Pl = fetch_eye_pos(texUV + vec2(-g_InvResolution.x, 0));
  vec3 Pt = fetch_eye_pos(texUV + vec2(0, g_InvResolution.y));
  vec3 Pb = fetch_eye_pos(texUV + vec2(0, -g_InvResolution.y));
  vec3 N = normalize(cross(Pr - Pl, Pt - Pb));
  vec4 tangentPlane = vec4(N, dot(P, N));*/

  // Screen-aligned basis for the tangent plane
  vec3 dPdu = min_diff(P, Pr, Pl);
  vec3 dPdv = min_diff(P, Pt, Pb) * (g_Resolution.y * g_InvResolution.x);

  // (cos(alpha),sin(alpha),jitter)
  vec3 rand = texture2D(random, pos.xy).xyz;
  
  float ao = 0;
  float d;
  float alpha = 2.0f * pi / g_NumDir;

  for (d = 0; d < g_NumDir; d++) {
    float angle = alpha * d;
    vec2 dir = vec2(cos(angle), sin(angle));
    vec2 deltaUV = rotate_direction(dir, rand.xy) * step_size.xy;
    ao += AccumulatedHorizonOcclusion(deltaUV, texUV, P, numSteps, rand.z, dPdu, dPdv);
  }

  value = 1.0 - ao / g_NumDir * g_Contrast;
}
