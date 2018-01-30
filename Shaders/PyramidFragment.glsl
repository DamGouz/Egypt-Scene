# version 150 core
uniform sampler2D pyramidTex1;
uniform sampler2D pyramidTex2;
uniform int useTexture ;
uniform float redh ;
uniform vec3 cameraPos ;
uniform vec4 lightColour ;
uniform vec3 lightPos ;
uniform float lightRadius ;

uniform sampler2D bumpTex ;
uniform sampler2DShadow shadowTex ; // NEW !


 in Vertex {
 vec3 colour ;
 vec2 texCoord ;
 vec3 normal ;
 vec3 tangent ;
 vec3 binormal ;
 vec3 worldPos ;
 vec4 shadowProj ; // New !
 } IN ;


out vec4 fragColor ;


void main ( void ) {
	vec4 diffuse;
	if( useTexture > 0) {
	vec4 clr = vec4(0.9f, 0.9f, 0.9f, 1.0f);
	if (IN.texCoord.y > redh){
	clr = vec4(0.1411f, 0.127f, 0.127f, 1.0f);
	}
			diffuse = texture(pyramidTex2, IN.texCoord*2)*vec4(0.9f, 0.9f, 0.9f, 1.0f);
			
			if (diffuse.a < 0.3){
				diffuse=texture(pyramidTex1, IN.texCoord*5)*clr;
			}
			else{
				diffuse.a = 1.0f;
			}
	}
	
   mat3 TBN = mat3 ( IN . tangent , IN . binormal , IN . normal );
 vec3 normal = IN.normal;

 vec3 incident = normalize ( lightPos - IN . worldPos );
 float lambert = max (0.0 , dot ( incident , normal )); // Different !

 float dist = length ( lightPos - IN . worldPos );
 float atten = 1.0 - clamp ( dist / lightRadius , 0.0 , 1.0);

 vec3 viewDir = normalize ( cameraPos - IN . worldPos );
 vec3 halfDir = normalize ( incident + viewDir );

 float rFactor = max (0.0 , dot ( halfDir , normal )); // Different !
 float sFactor = pow ( rFactor , 33.0 );
 float shadow = 1.0; // New !

 if( IN . shadowProj . w > 0.0) { // New !
 shadow = textureProj ( shadowTex , IN . shadowProj );
 }

 lambert *= shadow ; // New !

 vec3 colour = ( diffuse . rgb * lightColour . rgb );
 colour += ( lightColour . rgb * sFactor ) * 0.33;
 fragColor = vec4 ( colour * atten * lambert , diffuse . a );
 fragColor . rgb += ( diffuse . rgb * lightColour . rgb ) * 0.7;

}