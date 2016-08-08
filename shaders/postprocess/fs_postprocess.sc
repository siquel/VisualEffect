$input v_texcoord0

#include "../common.sh"

uniform vec4 u_params;
#define u_time u_params.x

SAMPLER2D(s_albedo, 0);

void main()
{
  vec2 uv = v_texcoord0;
  uv.x += sin( uv.y * 4*2*M_PI + (u_time * 2*M_PI * 0.75)) / 100;
  gl_FragColor = texture2D(s_albedo, uv);
}
