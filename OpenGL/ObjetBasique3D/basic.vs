attribute vec3 a_Position;
attribute vec3 a_Color;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 world;

varying vec4 v_Color;

void main(void)
{
	v_Color = vec4(a_Color, 1.0);
	
	
	gl_Position = projection* modelview * world  * vec4(a_Position, 1.);
}
