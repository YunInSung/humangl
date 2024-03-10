#version 400 core

in vec2 TEXCOR;

out vec4 color;

uniform sampler2D TEXTURE;

// LDR
void main() {
	// default
	float gamma = 2.2;
	color = texture(TEXTURE, TEXCOR);
    color.rgb = pow(color.rgb, vec3(1.0/gamma));

	// inversion
	// color = vec4(1 - vec3(color), 1);

	// grayscale
	// float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	// color = vec4(average, average, average, 1);
}


// HDR
// float exposure = 5;

// void main()
// {             
//     const float gamma = 2.2;
//     vec3 hdrColor = texture(TEXTURE, TEXCOR).rgb;
  
//     // Exposure tone mapping
//     vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
//     // Gamma correction 
//     // mapped = pow(mapped, vec3(1.0 / gamma));
  
//     color = vec4(mapped, 1.0);

// 	// color = texture(TEXTURE, TEXCOR);
// }