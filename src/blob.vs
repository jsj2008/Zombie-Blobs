#version 150 compatibility

precision highp float;

out vec3 normal;
out vec3 vertex;
uniform float time;

void main()
{
  float c = gl_Vertex.x * gl_Vertex.y + gl_Vertex.z * gl_Vertex.y + gl_Vertex.x * gl_Vertex.z;
  vec4 v = vec4(gl_Vertex.xyz * (0.8 + 0.3*sin(500*c + 3*time)), gl_Vertex.w);

  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * v;
  vertex = (gl_ModelViewMatrix * v).xyz;
  normal = gl_NormalMatrix * gl_Normal;
	gl_FrontColor = gl_Color;
}
