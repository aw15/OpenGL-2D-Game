//---fragment shader: fragment.glfs
#version 330 
in  vec3 ex_Color; // 버텍스세이더에게서전달받음 
out vec4 gl_FragColor; // 색상출력


void main(void) { 
	gl_FragColor = vec4(ex_Color,1.0); 
}