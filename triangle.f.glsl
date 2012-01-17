#version 120

uniform float fade;

void main(void) {
  //gl_FragColor[0] = 0.0;
  //gl_FragColor[1] = 0.0;
  //gl_FragColor[2] = 1.0;

  gl_FragColor[0] = gl_FragCoord.x/640.0;
  gl_FragColor[1] = gl_FragCoord.y/480.0;
  gl_FragColor[2] = 0.5;
  gl_FragColor[3] = fade;
}