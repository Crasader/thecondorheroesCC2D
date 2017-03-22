#ifdef GL_ES
precision mediump float;
#endif

varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;

void main(){
	vec4 textureColor = texture2D(CC_Texture0, cc_FragTexCoord1);
	//vec4 gray = vec4(0.3086, 0.6094, 0.0820,1);
	// Tint the texture color by the sprite's color.
	gl_FragColor[0] = textureColor[0] * 0.3086 + textureColor[1]*0.6094 + textureColor[2]*0.0820;
	gl_FragColor[1] = textureColor[0] * 0.3086 + textureColor[1]*0.6094 + textureColor[2]*0.0820;
	gl_FragColor[2] = textureColor[0] * 0.3086 + textureColor[1]*0.6094 + textureColor[2]*0.0820;
	gl_FragColor[3] = 1;
}