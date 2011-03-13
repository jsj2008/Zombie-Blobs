varying vec3 normal;
varying vec3 vertex;
varying vec3 world;
uniform float time;

void main()
{
	gl_Position = ftransform();
  vertex = (gl_ModelViewMatrix * gl_Vertex).xyz;
  world = gl_Vertex.xyz; // /gl_Vertex.w;
	normal = gl_NormalMatrix * gl_Normal;
	gl_FrontColor = gl_Color;
}
