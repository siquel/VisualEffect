$input v_texcoord0

#include "../common.sh"

SAMPLER2D(s_diffuse, 0);
SAMPLER2D(s_normal, 1);

void main()
{
	vec3 normal;
	// Transform normal vector to range [-1,1]
	normal.xy = texture2D(s_normal, v_texcoord0).xy * 2.0 - 1.0;
	gl_FragColor = texture2D(s_diffuse, v_texcoord0);
}
