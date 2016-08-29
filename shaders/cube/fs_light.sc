$input v_texcoord0

#include "../common.sh"

// normals
SAMPLER2D(s_normal, 0);
// depth buffer
SAMPLER2D(s_depth, 1);

uniform mat4 u_invMVP;

uniform vec4 u_lightColorRGBRadius;
uniform vec4 u_lightPositionInnerRadius;

#define u_lightColor u_lightColorRGBRadius.xyz
#define u_radius u_lightColorRGBRadius.w
#define u_lightPosition u_lightPositionInnerRadius.xyz
#define u_innerRadius u_lightPositionInnerRadius.w

vec3 calcLight(vec3 wpos, vec3 normal, vec3 viewDir)
{
  vec3 lightPos = u_lightPosition - wpos;
  float attenuation = 1.0 - smoothstep(u_innerRadius, 1.0, length(lightPos) / u_radius);

  vec3 lightDir = normalize(lightPos);

  float nDotL = dot(normal, lightDir);
  vec3 reflected = lightDir - 2.0 * nDotL * normal;

  float rDotV = dot(reflected, viewDir);

  float diffuse = max(0.0, nDotL);
  float specular = step(0.0, nDotL) * max(0.0, rDotV);

  vec3 rgb = u_lightColor * saturate(diffuse) * attenuation;

  return rgb;
}

vec3 clipToWorld(mat4 invViewProj, vec3 clipPos)
{
	vec4 wpos = mul(invViewProj, vec4(clipPos, 1.0));
	return wpos.xyz / wpos.w;
}

void main()
{
  vec3 normal = decodeNormalUint(texture2D(s_normal, v_texcoord0).xyz);
  float depth = texture2D(s_depth, v_texcoord0).x;

  vec3 clip = vec3(v_texcoord0 * 2.0 - 1.0, depth);
  clip.y = -clip.y;

  vec3 wpos = clipToWorld(u_invMVP, clip);

  vec3 viewDir = mul(u_view, vec4(wpos, 0.0)).xyz;
  viewDir = -normalize(viewDir);

  float ambient = 0.1;

  vec3 lightColor;
  lightColor = calcLight(wpos, normal, viewDir); // * ambient;

  gl_FragColor = vec4(toGamma(lightColor.xyz), 1.0);
}
