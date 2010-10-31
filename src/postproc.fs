uniform sampler2D scene;
uniform sampler2D sceneDepth;

float mangleDepth(float z) {
	float near = 0.1;
	float far = 1000.0;
	return (2*near)/(far + near - z * (far - near));
}

void main() {
  vec2 c = vec2(cos(gl_FragCoord.x), sin(gl_FragCoord.y))/50.0;
  c.x += 1; c.y += 1;
	c.x = 1; c.y = 1;
  vec2 tx = vec2(gl_TexCoord[0].x*c.x, gl_TexCoord[0].y*c.y);
  gl_FragColor = texture2D(scene, tx);
  float z1 = mangleDepth(texture2D(sceneDepth, tx).x);
  float z2 = mangleDepth(texture2D(sceneDepth, vec2(tx.x+0.002, tx.y)).x);
  float z3 = mangleDepth(texture2D(sceneDepth, vec2(tx.x, tx.y+0.002)).x);
  float z4 = mangleDepth(texture2D(sceneDepth, vec2(tx.x-0.002, tx.y)).x);
  float z5 = mangleDepth(texture2D(sceneDepth, vec2(tx.x, tx.y-0.002)).x);
  float dz = (z2-z1) + (z3-z1) + (z4-z1) + (z5-z1);
  gl_FragColor *= clamp(pow(1-10*dz, 100), 0.5, 1.1);
}
