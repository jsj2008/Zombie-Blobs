uniform sampler2D scene;
uniform sampler2D sceneDepth;
uniform sampler2D normals;

const float zNear = 0.1;
const float zFar = 200.0;

float mangleDepth(float z) {
  return (2.0*zNear)/(zFar + zNear - z * (zFar - zNear));
}

// see T Möller, 1999: Efficiently building a matrix to rotate one vector to another
mat3 rotateNormalVecToAnother(vec3 f, vec3 t) {
  vec3 v = cross(f,t);
  float c = dot(f,t);
  float h = (1-c)/(1-c*c);
  return mat3(c+h*v.x*v.x, h*v.x*v.y+v.z, h*v.x*v.z-v.y,
      h*v.x*v.y-v.z, c+h*v.y*v.y,h*v.y*v.z+v.x,
      h*v.x*v.z+v.y, h*v.y*v.z-v.x, c+h*v.z*v.z);
}

void main()
{
  vec2 tx = gl_TexCoord[0].st;
  vec2 px = vec2(1/800.0, 1/600.0);

  float depth = mangleDepth(texture2D(sceneDepth, tx).x);
  vec3 norm = normalize(texture2D(normals, tx).rgb);

  float radius = 1.0f;
  float zOne = 1.0/(zFar-zNear);
  float zRange = radius * zOne;

  // describes rotation from normal vector to (0,0,1)
  // todo: precompute special case?
  mat3 rotator = rotateNormalVecToAnother(norm, vec3(0,0,1));

  // result of line sampling
  // See Loos & Sloan: Volumetric Obscurance
  // http://www.cs.utah.edu/~loos/publications/vo/vo.pdf
  float hemi = 0.0f;
  float maxi = 0.0f;

  for (int dx=-2; dx <= 2; ++dx) {
    for (int dy=-2; dy <= 2; ++dy) {
      float dx_ = 0.3 * dx;
      float dy_ = 0.3 * dy;

      // z coordinate of (x,y) in unit sphere
      // (local coordinates, upper hemisphere)
      float zs = sqrt(1 - dx_*dx_ - dy_*dy_);
      vec3 localCoord = vec3(dx*px.x, dy*px.y, 0);
      // just a guess, should sample some constant sized disc in world coords
      vec2 coord = tx + (10 - 8*depth)*localCoord.xy;
      localCoord.z = mangleDepth(texture2D(sceneDepth, coord).x);
      localCoord.z -= depth;
      // now it should really be in local coordinates
      localCoord = rotator * localCoord;
      float dr = localCoord.z/zRange;
      float v = max(min(zs, dr) + zs, 0);
      maxi += zs;
      hemi += v;
    }
  }
  hemi = hemi/maxi;
  gl_FragColor = texture2D(scene, tx);
  //  gl_FragColor.rgb += norm;
  gl_FragColor.rgb *= hemi;
}
