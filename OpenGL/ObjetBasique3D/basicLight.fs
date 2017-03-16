#version 330 compatibility

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;
uniform int bLight;
 
in vec3 ambiant;
in vec3 specular;
in vec3 diffuse;

out vec3 color;

void main(void){

	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = diffuse;
	float LightPower = 50.0;
	
    vec3 MaterialDiffuseColor = vec3(0,0,0);
	vec3 MaterialAmbientColor = vec3(0,0,0);
	vec3 MaterialSpecularColor = vec3(0,0,0);

	// proprietes material
    if(bLight == 1)
    {
	    MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;
	    MaterialAmbientColor = ambiant * MaterialDiffuseColor;
	    MaterialSpecularColor = specular;
    }
    else
    {
        MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;
	    MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	    MaterialSpecularColor = vec3(0.3,0.3,0.3);
    }
	// Distance de la lumiere
	float distance = length( LightPosition_worldspace - Position_worldspace );

	// Normalisation par rapport à la camera
	vec3 n = normalize( Normal_cameraspace );
	// Direction de la lumiere
	vec3 l = normalize( LightDirection_cameraspace );

    // Calcul spéculaire
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);

	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	color = 
		// Ambient 
		MaterialAmbientColor +
		// Diffuse 
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular 
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);

}