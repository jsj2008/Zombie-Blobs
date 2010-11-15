uniform sampler2D scene;
uniform sampler2D sceneDepth;
uniform sampler2D normals;


float mangleDepth(float z) {
  float near = 0.1;
  float far = 1000.0;
  return (2.0*near)/(far + near - z * (far - near));
}


void main()
{
  vec2 c = vec2(cos(gl_FragCoord.x*0.05), sin(gl_FragCoord.y*0.05))/50.0;
  c.x += 1.0; c.y += 1.0;
	c.x = 1.0; c.y = 1.0;
  vec2 tx = vec2(gl_TexCoord[0].x*c.x, gl_TexCoord[0].y*c.y);
  gl_FragColor = texture2D(scene, tx);
  vec2 px = vec2(1/800.0, 1/600.0);
  float z1 = mangleDepth(texture2D(sceneDepth, tx).x);
  float z2 = mangleDepth(texture2D(sceneDepth, vec2(tx.x+px.x, tx.y)).x);
  float z3 = mangleDepth(texture2D(sceneDepth, vec2(tx.x, tx.y+px.y)).x);
  float z4 = mangleDepth(texture2D(sceneDepth, vec2(tx.x-px.x, tx.y)).x);
  float z5 = mangleDepth(texture2D(sceneDepth, vec2(tx.x, tx.y-px.y)).x);
  float dz = (z2-z1) + (z3-z1) + (z4-z1) + (z5-z1);
  gl_FragColor.rgb += texture2D(normals, tx).rgb;
  gl_FragColor.rgb *= clamp(pow(1.0-10.0*dz, 100.0), 0.5, 1.1);
}
