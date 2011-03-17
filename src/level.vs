#version 150 compatibility

precision highp float;

out vec3 normal;
out vec3 vertex;
out vec3 world;

void main()
{
  vec4 v = gl_ModelViewMatrix * gl_Vertex;
  gl_Position = gl_ProjectionMatrix * v;
	vertex = v.xyz;
  world = gl_Vertex.xyz; // /gl_Vertex.w;
	normal = gl_NormalMatrix * gl_Normal;
	gl_FrontColor = gl_Color;
}
