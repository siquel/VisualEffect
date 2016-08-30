$input v_texcoord0

#include "../common.sh"

SAMPLER2D(s_albedo, 0); // base

void main()
{
  vec2 tex_offset = u_viewTexel.xy;
  vec3 result = texture2D(s_albedo, v_texcoord0).xyz * 0.2270270270;

  result += texture2D(s_albedo, v_texcoord0 + vec2(tex_offset.x * 0, 0.0)).xyz * 0.2270270270;
  result += texture2D(s_albedo, v_texcoord0 - vec2(tex_offset.x * 0, 0.0)).xyz * 0.2270270270;

  result += texture2D(s_albedo, v_texcoord0 + vec2(tex_offset.x * 1, 0.0)).xyz * 0.1945945946;
  result += texture2D(s_albedo, v_texcoord0 - vec2(tex_offset.x * 1, 0.0)).xyz * 0.1945945946;

  result += texture2D(s_albedo, v_texcoord0 + vec2(tex_offset.x * 2, 0.0)).xyz * 0.1216216216;
  result += texture2D(s_albedo, v_texcoord0 - vec2(tex_offset.x * 2, 0.0)).xyz * 0.1216216216;

  result += texture2D(s_albedo, v_texcoord0 + vec2(tex_offset.x * 3, 0.0)).xyz * 0.0540540541;
  result += texture2D(s_albedo, v_texcoord0 - vec2(tex_offset.x * 3, 0.0)).xyz * 0.0540540541;

  result += texture2D(s_albedo, v_texcoord0 + vec2(tex_offset.x * 4, 0.0)).xyz * 0.0162162162;
  result += texture2D(s_albedo, v_texcoord0 - vec2(tex_offset.x * 4, 0.0)).xyz * 0.0162162162;

  gl_FragColor = vec4(result, 1.0);
}
