#version 120
attribute vec2 coord2d;

varying vec2 texcoord;

uniform vec2 location;
uniform vec2 size;

void main(void) {

  gl_Position = vec4((coord2d*size)+location, 0.0, 1.0);
  //gl_Position.x = (coord2d.x * size.x)+location.x;
  //gl_Position.y = (coord2d.y * size.y)+location.y;

  texcoord = coord2d * vec2(2.5) + vec2(0.5);
  //f_texcoord = texcoord * vec2(0.5) + vec2(0.5);
}
