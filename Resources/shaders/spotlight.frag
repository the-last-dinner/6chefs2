varying vec4 v_color;
varying vec2 v_lightCoord;
varying vec2 v_texCoord;

//texture samplers
uniform sampler2D u_texture; //diffuse map
//uniform sampler2D u_lightmap;   //light map

//additional parameters for the shader
uniform vec4 ambientColor; //ambient RGB, alpha channel is intensity

void main() {
//	vec4 diffuseColor = texture2D(u_texture, v_lightCoord);
//	vec4 light = texture2D(u_lightmap, v_lightCoord);
// 
//	vec3 ambient = ambientColor.rgb * ambientColor.a;
//	vec3 intensity = ambient + light.rgb;
//	vec3 finalColor = diffuseColor.rgb * intensity;
// 
//	gl_FragColor = v_color * vec4(finalColor, diffuseColor.a);
    
    vec4 diffuseColor = texture2D(u_texture, v_texCoord);
    
    vec3 final = diffuseColor.rgb * ambientColor.rgb;
    gl_FragColor = vec4(final, diffuseColor.a);
}