$input v_texcoord0

#include "../common.sh"

uniform vec4 u_params;
#define u_time u_params.x

SAMPLER2D(s_albedo, 0);
SAMPLER2D(s_light, 1);

void main()
{
  vec2 uv = v_texcoord0;
//  uv.x += sin( uv.y * 4*2*M_PI + (u_time * 2*M_PI * 0.75)) / 100;
  vec4 color = texture2D(s_albedo, uv);
  // linear
  color = vec4(pow(abs(color.xyz), vec3_splat(2.2) ), color.w);
  vec4 light = toLinear(texture2D(s_light, uv));


  gl_FragColor = toGamma(color * light);
}
