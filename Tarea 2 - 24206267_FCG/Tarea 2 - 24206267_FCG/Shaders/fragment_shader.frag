#version 400
//#extension GL_NV_shadow_samplers_cube : enable
precision highp float;
struct DirLight {
	vec3 direction;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Modelo {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float biasM;
};

in vec3 normales;
in vec3 FragPos;
in vec2 coordTex;
in vec4 FragPosLuz;

in mat3 TBN;

out vec4 color; 

uniform DirLight dirLight;
uniform Modelo model;
uniform sampler2D tex;
uniform sampler2D normalMap;
uniform sampler2D depthText;
uniform samplerCube skyboxText; 
uniform int position_i;
uniform bool bb;
uniform bool hayNormalMapping;
uniform bool planar;
uniform vec3 view;
uniform int hayTex;
uniform float currentLight;
uniform float pointConstant;
uniform float pointLinear;
uniform float pointQuadratic;
uniform float innerCut;
uniform float outerCut;
uniform int hayLuz;

uniform float fRefract;


vec4 CalcDifLight(Modelo modelos, DirLight light, vec3 normal, vec3 viewDir, vec3 TangentLightPos, vec3 TangentFragPos)
{
	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
	vec4 diffuse;
	float diff = max(dot(normal, lightDir), 0.0);

	diffuse = vec4(light.diffuse * (diff * modelos.diffuse), 1.0);
	return (diffuse);
}

vec4 CalcSpecLight(Modelo modelos, DirLight light, vec3 normal, vec3 viewDir, vec3 TangentLightPos, vec3 TangentFragPos)
{
	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);

	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec4 specular = vec4(light.specular * (spec * modelos.specular), 1.0);
	return (specular);
}


float CalcShadow(vec4 fragPosL, vec3 normals, vec3 TangentLightPos, vec3 TangentFragPos, Modelo modelos)
{
    vec3 coordsProj = fragPosL.xyz / fragPosL.w;
    coordsProj = coordsProj * 0.5 + 0.5;
    //float closestDepth = texture(depthText, coordsProj.xy).r; 
    float actualDepth = coordsProj.z;
    vec3 normal = normalize(normals);
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), modelos.biasM);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthText, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float PCF = texture(depthText, coordsProj.xy + vec2(x, y) * texelSize).r; 
          	if(actualDepth - bias > PCF) shadow += 1.0;
 
        }    
    }
    shadow /= 9.0;
    
    // Mantener la sombra en 0 cuando se pasa del frustum de la luz
    if(coordsProj.z > 1.0) shadow = 0.0;
        
    return shadow;
}


vec4 CalcLightMap(){
	    
	//El Color difuso del modelo    
    vec4 color = texture2D(tex, coordTex);

    // El Color del lightmap (trato el lightmap como si fuese un mapa de normal, para no activar otra textura)
    vec4 lightColor = texture2D(normalMap, coordTex);

    // Unimos ambos colores;
    vec4 finalColor = color * lightColor;
    return finalColor;
}

void main()
{
	if(bb){
		color = vec4(1.0, 0.0, 0.0, 1.0);
	}else if (hayLuz != 1){
		if(position_i == 2 && !planar) color = CalcLightMap();
		else color = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else{
		vec3 norm = normalize(normales);
		vec3 TangentLightDir = dirLight.direction;
		vec3 TangentViewPos = view;
		vec3 TangentFragPos = FragPos;
		vec3 TangentSpotPos = dirLight.position;

		if(position_i == 0){

			vec3 I = normalize(TangentFragPos - TangentViewPos);
			vec3 R = reflect(I, normalize(norm));
			color = texture(skyboxText, R);

		}else if (position_i == 2 && !planar){

			color = CalcLightMap(); //Si es el modelo lightmap, calculo

		}else if(position_i == 3){

			vec3 I = normalize(TangentFragPos - TangentViewPos);
			vec3 R = refract(I, normalize(norm), fRefract);
			color = texture(skyboxText, R);
			
		}else{ 
			if((position_i == 1  || position_i == 6) && hayNormalMapping){
	    		norm = texture(normalMap, coordTex).rgb;
	    		norm = normalize(norm * 2.0 - 1.0); 
				norm = normalize(TBN * norm);
			}

			vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
			float shadow = CalcShadow(FragPosLuz, norm, TangentLightDir, TangentFragPos, model);  
			vec4 result = CalcDifLight(model, dirLight, norm, viewDir, TangentLightDir, TangentFragPos);
			result += CalcSpecLight(model, dirLight, norm, viewDir, TangentLightDir, TangentFragPos);


			if(position_i == 7 && (currentLight == 2 || currentLight == 1)) color = vec4(dirLight.specular * dirLight.diffuse, 1.0);
			else{
				float attenuation = 1.0;
				float intensity = 1.0;
				shadow = min(shadow, 0.75);

				if(currentLight == 1 ) {
					result *= (1.0 - shadow);
				} else if(currentLight == 2){
					// Atenuacion lineal
					float distance = length(TangentLightDir - TangentFragPos);
					attenuation = 1.0 / (pointConstant + pointLinear * distance + pointQuadratic * (distance * distance));  
				}else if (currentLight == 3){

					vec3 lightDir = normalize(TangentLightDir - TangentFragPos);
					// Spotlight (suavizado)
					float theta = dot(lightDir, normalize(-(TangentSpotPos))); 
					float epsilon = (innerCut - outerCut);
					intensity = clamp((theta - outerCut) / epsilon, 0.0, 1.0);

					// Atenuacion lineal
					float distance = length(TangentLightDir - TangentFragPos);
					attenuation = 1.0 / (pointConstant + pointLinear * distance + pointQuadratic * (distance * distance));  

					result *= (1.0 - shadow);
				}

				//Color ambiental
				result += vec4(dirLight.ambient, 1.0) * vec4(model.ambient,1.0);

 

				if(position_i == 7 && (currentLight == 3)) color = vec4(dirLight.specular * dirLight.diffuse, 1.0) * attenuation * intensity;
				else if(hayTex == -1) color = result * attenuation * intensity * texture2D(tex, coordTex);
				else color = result * attenuation * intensity;
			}

		}
	}
}