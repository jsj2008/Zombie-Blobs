varying vec3 normal;
varying vec3 vertex;
uniform float time;

void main()
{
  float c = gl_Vertex.x * gl_Vertex.y + gl_Vertex.z * gl_Vertex.y + gl_Vertex.x * gl_Vertex.z;
  gl_Vertex.xyz *= (0.8 + 0.3*sin(500*c + 3*time));

  gl_Position = ftransform();
  vertex = (gl_ModelViewMatrix * gl_Vertex).xyz;
  normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_FrontColor = gl_Color;
}
