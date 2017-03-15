attribute vec4 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_TexCoords;

uniform mat4 u_WorldMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelViewMatrix;

uniform float u_Time;

varying vec3 v_Normal;
varying vec3 v_Position;
varying vec2 v_TexCoords;

void main(void)
{
	v_TexCoords = a_TexCoords;

	// si on est sur que la matrice u_worldMatrix est orthogonale
	// on peut simplifier avec inverse(inverse()) qui donne la matrice initiale
	v_Normal = vec3(u_WorldMatrix * vec4(a_Normal.xyz, 0.0));

	// position du vertex dans le repere du monde
	v_Position = vec3(u_WorldMatrix * a_Position);

	gl_Position = u_ProjectionMatrix * u_WorldMatrix * u_ModelViewMatrix * a_Position;
}
