
uniform sampler2D scene;

void main() {
  vec2 c = vec2(cos(gl_FragCoord.x), sin(gl_FragCoord.y))/50.0;
  c.x += 1; c.y += 1;
  vec2 tx = vec2(gl_TexCoord[0].x*c.x, gl_TexCoord[0].y*c.y);
  gl_FragColor = texture2D(scene, tx);
}
