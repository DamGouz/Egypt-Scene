# version 150 core
uniform sampler2D roomTex1;
uniform sampler2D roomTex2;
uniform sampler2D roomTex3;
uniform int useTexture ;
uniform int side ;
uniform float redh ;
uniform int numLights ;

uniform vec3 cameraPos ;

uniform struct Light {
	 vec4 lightColour ;
	 vec3 lightPos ;
	 float lightRadius ;
	 vec3 direction;
} allLights[20];

in Vertex {
vec2 texCoord;
vec4 colour;
vec3 normal ;
vec3 worldPos;
} IN ;


out vec4 fragColor ;
void main ( void ) {
	vec4 diffuse;
	if( useTexture > 0) {
		if (side == 1){
		diffuse = texture(roomTex2, IN.texCoord*4);
		}
		else if (side == 2){
		diffuse = texture(roomTex3, IN.texCoord*4);
		}
		else{
		diffuse = texture(roomTex1, IN.texCoord*4);
		}
	}
	fragColor= vec4(0,0,0,0);
for (int i=0; i<numLights; i++){
  vec3 incident = normalize ( allLights[i].lightPos - IN.worldPos );
  float lambert = max (0.0 , dot ( incident , allLights[i].direction ));
  float dist = length ( allLights[i].lightPos - IN.worldPos );
  float atten = 1.0 - clamp ( dist / allLights[i].lightRadius , 0.0 , 1.0);
  vec3 viewDir = normalize ( cameraPos - IN.worldPos );
  vec3 halfDir = normalize ( incident + viewDir );

  float rFactor = max (0.0 , dot ( halfDir , allLights[i].direction ));
  float sFactor = pow ( rFactor , 50.0 );
  vec3 colour = ( diffuse.rgb * allLights[i].lightColour.rgb );
  colour += ( allLights[i].lightColour.rgb * sFactor ) * 0.33;
  vec4 tempColor = vec4 ( colour * atten * lambert , diffuse.a );
  
  fragColor += tempColor;
  }
  fragColor.rgb += ( diffuse.rgb * allLights[0].lightColour.rgb )*0.2;
}