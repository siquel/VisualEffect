$input v_texcoord0

#include "../common.sh"

// normals
SAMPLER2D(s_normal, 0);
// depth buffer
SAMPLER2D(s_depth, 1);

uniform mat4 u_invMVP;

void main()
{
  vec3 normal = (texture2D(s_normal, v_texcoord0).xyz) * 2.0 - 1.0;
  float depth = texture2D(s_depth, v_texcoord0).x;

  vec3 lightDir = vec3(-0.2, -1.0, -0.3);
  lightDir = normalize(lightDir);

  float ndotl = dot(normal, lightDir);
  vec3 reflected = lightDir - 2.0 * ndotl * normal;

  vec3 lightColor = vec3(0.1, 0.2, 0.3);
  gl_FragColor.xyz = lightColor;
  gl_FragColor.w = 1.0;
}
