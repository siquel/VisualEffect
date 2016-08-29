$input v_texcoord0

#include "../common.sh"

SAMPLER2D(s_albedo, 0); // base
SAMPLER2D(s_light, 1); // bloom



vec4 adjustSaturation(vec4 color, float saturation)
{
    // The constants 0.3, 0.59, and 0.11 are chosen because the
    // human eye is more sensitive to green light, and less to blue.
    float grey = dot(color.xyz, vec3(0.3, 0.59, 0.11));

    return lerp(grey, color, saturation);
}


void main()
{
  vec4 base = texture2D(s_albedo, v_texcoord0);
  vec4 bloom = texture2D(s_light, v_texcoord0);

  float BloomSaturation = 1.0;
  float BloomIntensity = 4.0;

  base = adjustSaturation(base, BloomSaturation) * BloomIntensity;
  bloom = adjustSaturation(bloom, BloomSaturation) * BloomIntensity;

  gl_FragColor = base + bloom;
}
