$input v_texcoord0

#include "../common.sh"

SAMPLER2D(s_diffuse, 0);

void main()
{
	gl_FragColor = texture2D(s_diffuse, v_texcoord0);
}
