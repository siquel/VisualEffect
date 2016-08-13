$input a_position, a_normal, a_tangent, a_texcoord0
$output v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0

#include "../common.sh"

void main()
{
	vec3 wpos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
	gl_Position = mul(u_viewProj, vec4(wpos, 1.0) );

	// transform normal to range [-1, 1]
	vec4 normal = a_normal * 2.0 - 1.0;
	// calculate normal in world space
	vec3 wnormal = mul(u_model[0], vec4(normal.xyz, 0.0)).xyz;

	// transform tangent to range [-1, 1]
	vec4 tangent = a_tangent * 2.0 - 1.0;
	// calculate tangent in world space
	vec3 wtangent = mul(u_model[0], vec4(tangent.xyz, 0.0)).xyz;

	// calculate TBN matrix
	vec3 viewTangent = normalize(mul(u_view, vec4(wtangent, 0.0)).xyz);
	vec3 viewNormal = normalize(mul(u_view, vec4(wnormal, 0.0)).xyz);
	// TBN vectors are perpendicular so we can use cross(T, N) to get B
	vec3 viewBitangent = cross(viewTangent, viewNormal) * tangent.w;

	// tbn is in view space
	mat3 tbn = mat3(viewTangent, viewBitangent, viewNormal);

	v_wpos = wpos;
	v_view = mul( mul(u_view, vec4(wpos, 0.0)).xyz, tbn );
	v_normal = viewNormal;
	v_tangent = viewTangent;
	v_bitangent = viewBitangent;
	v_texcoord0 = a_texcoord0;
}
