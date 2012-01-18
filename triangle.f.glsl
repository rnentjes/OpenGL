#version 120

//uniform float fade;
uniform sampler2D mytexture;

varying vec2 texcoord;

void main(void) {
  //gl_FragColor[0] = 0.0;
  //gl_FragColor[1] = 0.0;
  //gl_FragColor[2] = 1.0;

  //gl_FragColor[0] = gl_FragCoord.x/640.0;
  //gl_FragColor[1] = gl_FragCoord.y/480.0;
  //gl_FragColor[2] = 0.5;
  //gl_FragColor[3] = 0.5;
  //gl_FragColor[3] = fade;

  //vec4 bla = texture2D(mytexture, vec2(f_texcoord.s, f_texcoord.t));
  vec4 bla = texture2D(mytexture, texcoord);
  //bla.r = 1.0;
  //bla.g = 0.5;
  //bla.b = texture2D(mytexture, vec2(0.0, 0.0)).b;

  gl_FragColor = bla;
}
