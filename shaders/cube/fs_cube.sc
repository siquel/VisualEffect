$input v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0

#include "../common.sh"

SAMPLER2D(s_diffuse, 0);
SAMPLER2D(s_normal, 1);

void main()
{
	vec3 normal;
	// Transform normal vector to range [-1,1]
	normal.xy = texture2D(s_normal, v_texcoord0).xy * 2.0 - 1.0;
	normal.z = sqrt(1.0 - dot(normal.xy, normal.xy));

	mat3 tbn = mat3(
		normalize(v_tangent),
		normalize(v_bitangent),
		normalize(v_normal)
	);


	normal = normalize(mul(tbn, normal));
	vec3 wnormal = normalize(mul(u_invView, vec4(normal, 0.0) ).xyz);

	gl_FragData[0] = texture2D(s_diffuse, v_texcoord0);
	gl_FragData[1] = vec4(encodeNormalUint(wnormal), 1.0);
}
