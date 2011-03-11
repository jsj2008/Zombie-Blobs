varying vec3 normal;
varying vec3 vertex;
uniform float time;

void main()
{
	gl_Position = ftransform();
  vertex = (gl_ModelViewMatrix * gl_Vertex).xyz;
	normal = gl_NormalMatrix * gl_Normal;
	gl_FrontColor = gl_Color;
}
