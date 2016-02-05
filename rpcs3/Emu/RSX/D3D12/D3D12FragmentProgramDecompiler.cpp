#include "stdafx.h"
#include "stdafx_d3d12.h"
#ifdef _MSC_VER
#include "D3D12FragmentProgramDecompiler.h"
#include "D3D12CommonDecompiler.h"
#include "Emu/Memory/Memory.h"
#include "Emu/System.h"

D3D12FragmentDecompiler::D3D12FragmentDecompiler(const RSXFragmentProgram &prog, u32& size) :
	FragmentProgramDecompiler(prog, size)
{

}

std::string D3D12FragmentDecompiler::getFloatTypeName(size_t elementCount)
{
	return getFloatTypeNameImp(elementCount);
}

std::string D3D12FragmentDecompiler::getFunction(enum class FUNCTION f)
{
	return getFunctionImp(f);
}

std::string D3D12FragmentDecompiler::saturate(const std::string & code)
{
	return "saturate(" + code + ")";
}

std::string D3D12FragmentDecompiler::compareFunction(COMPARE f, const std::string &Op0, const std::string &Op1)
{
	return compareFunctionImp(f, Op0, Op1);
}

void D3D12FragmentDecompiler::insertHeader(std::stringstream & OS)
{
	OS << "cbuffer SCALE_OFFSET : register(b0)" << std::endl;
	OS << "{" << std::endl;
	OS << "	float4x4 scaleOffsetMat;" << std::endl;
	OS << "	int isAlphaTested;" << std::endl;
	OS << "	float alphaRef;" << std::endl;
	OS << "};" << std::endl;
}

void D3D12FragmentDecompiler::insertIntputs(std::stringstream & OS)
{
	OS << "struct PixelInput" << std::endl;
	OS << "{" << std::endl;
	OS << "	float4 Position : SV_POSITION;" << std::endl;
	OS << "	float4 diff_color : COLOR0;" << std::endl;
	OS << "	float4 spec_color : COLOR1;" << std::endl;
	OS << "	float4 dst_reg3 : COLOR2;" << std::endl;
	OS << "	float4 dst_reg4 : COLOR3;" << std::endl;
	OS << "	float fogc : FOG;" << std::endl;
	OS << "	float4 tc9 : TEXCOORD9;" << std::endl;
	OS << "	float4 tc0 : TEXCOORD0;" << std::endl;
	OS << "	float4 tc1 : TEXCOORD1;" << std::endl;
	OS << "	float4 tc2 : TEXCOORD2;" << std::endl;
	OS << "	float4 tc3 : TEXCOORD3;" << std::endl;
	OS << "	float4 tc4 : TEXCOORD4;" << std::endl;
	OS << "	float4 tc5 : TEXCOORD5;" << std::endl;
	OS << "	float4 tc6 : TEXCOORD6;" << std::endl;
	OS << "	float4 tc7 : TEXCOORD7;" << std::endl;
	OS << "	float4 tc8 : TEXCOORD8;" << std::endl;
	OS << "};" << std::endl;
}

void D3D12FragmentDecompiler::insertOutputs(std::stringstream & OS)
{
	OS << "struct PixelOutput" << std::endl;
	OS << "{" << std::endl;
	const std::pair<std::string, std::string> table[] =
	{
		{ "ocol0", m_ctrl & CELL_GCM_SHADER_CONTROL_32_BITS_EXPORTS ? "r0" : "h0" },
		{ "ocol1", m_ctrl & CELL_GCM_SHADER_CONTROL_32_BITS_EXPORTS ? "r2" : "h4" },
		{ "ocol2", m_ctrl & CELL_GCM_SHADER_CONTROL_32_BITS_EXPORTS ? "r3" : "h6" },
		{ "ocol3", m_ctrl & CELL_GCM_SHADER_CONTROL_32_BITS_EXPORTS ? "r4" : "h8" },
	};
	size_t idx = 0;
	for (int i = 0; i < sizeof(table) / sizeof(*table); ++i)
	{
		if (m_parr.HasParam(PF_PARAM_NONE, "float4", table[i].second))
			OS << "	" << "float4" << " " << table[i].first << " : SV_TARGET" << idx++ << ";" << std::endl;
	}
	if (m_ctrl & CELL_GCM_SHADER_CONTROL_DEPTH_EXPORT)
		OS << "	float depth : SV_Depth;" << std::endl;
	OS << "};" << std::endl;
}

void D3D12FragmentDecompiler::insertConstants(std::stringstream & OS)
{
	OS << "cbuffer CONSTANT : register(b2)" << std::endl;
	OS << "{" << std::endl;
	for (const ParamType &PT : m_parr.params[PF_PARAM_UNIFORM])
	{
		if (PT.type == "sampler1D" || PT.type == "sampler2D" || PT.type == "samplerCube" || PT.type == "sampler3D")
			continue;
		for (const ParamItem &PI : PT.items)
			OS << "	" << PT.type << " " << PI.name << ";" << std::endl;
	}
	OS << "};" << std::endl << std::endl;

	for (const ParamType &PT : m_parr.params[PF_PARAM_UNIFORM])
	{
		if (PT.type == "sampler1D")
		{
			for (const ParamItem &PI : PT.items)
			{
				size_t textureIndex = atoi(PI.name.data() + 3);
				OS << "Texture1D " << PI.name << " : register(t" << textureIndex + 16 << ");" << std::endl;
				OS << "sampler " << PI.name << "sampler : register(s" << textureIndex << ");" << std::endl;
			}
		}
		else if (PT.type == "sampler2D")
		{
			for (const ParamItem &PI : PT.items)
			{
				size_t textureIndex = atoi(PI.name.data() + 3);
				OS << "Texture2D " << PI.name << " : register(t" << textureIndex + 16 << ");" << std::endl;
				OS << "sampler " << PI.name << "sampler : register(s" << textureIndex << ");" << std::endl;
			}
		}
		else if (PT.type == "sampler3D")
		{
			for (const ParamItem &PI : PT.items)
			{
				size_t textureIndex = atoi(PI.name.data() + 3);
				OS << "Texture3D " << PI.name << " : register(t" << textureIndex + 16 << ");" << std::endl;
				OS << "sampler " << PI.name << "sampler : register(s" << textureIndex << ");" << std::endl;
			}
		}
		else if (PT.type == "samplerCube")
		{
			for (const ParamItem &PI : PT.items)
			{
				size_t textureIndex = atoi(PI.name.data() + 3);
				OS << "TextureCube " << PI.name << " : register(t" << textureIndex + 16 << ");" << std::endl;
				OS << "sampler " << PI.name << "sampler : register(s" << textureIndex << ");" << std::endl;
			}
		}
	}
}

void D3D12FragmentDecompiler::insertMainStart(std::stringstream & OS)
{
	insert_d3d12_legacy_function(OS);

	const std::set<std::string> output_value =
	{
		"r0", "r1", "r2", "r3", "r4",
		"h0", "h2", "h4", "h6", "h8"
	};
	OS << "void ps_impl(bool is_front_face, PixelInput In, inout float4 r0, inout float4 h0, inout float4 r1, inout float4 h2, inout float4 r2, inout float4 h4, inout float4 r3, inout float4 h6, inout float4 r4, inout float4 h8)" << std::endl;
	OS << "{" << std::endl;
	for (const ParamType &PT : m_parr.params[PF_PARAM_IN])
	{
		for (const ParamItem &PI : PT.items)
		{
			if (m_prog.front_back_color_enabled)
			{
				if (PI.name == "spec_color" && m_prog.back_color_specular_output)
				{
					OS << "	float4 spec_color = is_front_face ? In.dst_reg4 : In.spec_color;\n";
					continue;
				}
				if (PI.name == "diff_color" && m_prog.back_color_diffuse_output)
				{
					OS << "	float4 diff_color = is_front_face ? In.dst_reg3 : In.diff_color;\n";
					continue;
				}
			}
			OS << "	" << PT.type << " " << PI.name << " = In." << PI.name << ";" << std::endl;
		}
	}
	// A bit unclean, but works.
	OS << "	" << "float4 gl_Position = In.Position;" << std::endl;
	if (m_prog.origin_mode == rsx::window_origin::bottom)
		OS << "	gl_Position.y = (" << std::to_string(m_prog.height) << " - gl_Position.y);\n";
	// Declare output
	for (const ParamType &PT : m_parr.params[PF_PARAM_NONE])
	{
		for (const ParamItem &PI : PT.items)
			if (output_value.find(PI.name) == output_value.end())
				OS << "	" << PT.type << " " << PI.name << " = float4(0., 0., 0., 0.);" << std::endl;
	}
	// Declare texture coordinate scaling component (to handle unormalized texture coordinates)

	for (const ParamType &PT : m_parr.params[PF_PARAM_UNIFORM])
	{
		if (PT.type != "sampler2D")
			continue;
		for (const ParamItem& PI : PT.items)
		{
			size_t textureIndex = atoi(PI.name.data() + 3);
			bool is_unorm = !!(m_prog.unnormalized_coords & (1 << textureIndex));
			if (!is_unorm)
			{
				OS << "	float2  " << PI.name << "_scale = float2(1., 1.);" << std::endl;
				continue;
			}
			OS << "	float2  " << PI.name << "_dim;" << std::endl;
			OS << "	" << PI.name << ".GetDimensions(" << PI.name << "_dim.x, " << PI.name << "_dim.y);" << std::endl;
			OS << "	float2  " << PI.name << "_scale = float2(1., 1.) / " << PI.name << "_dim;" << std::endl;
		}
	}
}

void D3D12FragmentDecompiler::insertMainEnd(std::stringstream & OS)
{
	OS << "}" << std::endl;
	OS << std::endl;
	OS << "PixelOutput main(PixelInput In, bool is_front_face : SV_IsFrontFace)" << std::endl;
	OS << "{" << std::endl;
	OS << "	float4 r0 = float4(0., 0., 0., 0.);" << std::endl;
	OS << "	float4 r1 = float4(0., 0., 0., 0.);" << std::endl;
	OS << "	float4 r2 = float4(0., 0., 0., 0.);" << std::endl;
	OS << "	float4 r3 = float4(0., 0., 0., 0.);" << std::endl;
	OS << "	float4 r4 = float4(0., 0., 0., 0.);" << std::endl;
	OS << "	float4 h0 = float4(0., 0., 0., 0.);" << std::endl;
	OS << "	float4 h2 = float4(0., 0., 0., 0.);" << std::endl;
	OS << "	float4 h4 = float4(0., 0., 0., 0.);" << std::endl;
	OS << "	float4 h6 = float4(0., 0., 0., 0.);" << std::endl;
	OS << "	float4 h8 = float4(0., 0., 0., 0.);" << std::endl;
	OS << "	ps_impl(is_front_face, In, r0, h0, r1, h2, r2, h4, r3, h6, r4, h8);" << std::endl;

	const std::pair<std::string, std::string> table[] =
	{
		{ "ocol0", m_ctrl & CELL_GCM_SHADER_CONTROL_32_BITS_EXPORTS ? "r0" : "h0" },
		{ "ocol1", m_ctrl & CELL_GCM_SHADER_CONTROL_32_BITS_EXPORTS ? "r2" : "h4" },
		{ "ocol2", m_ctrl & CELL_GCM_SHADER_CONTROL_32_BITS_EXPORTS ? "r3" : "h6" },
		{ "ocol3", m_ctrl & CELL_GCM_SHADER_CONTROL_32_BITS_EXPORTS ? "r4" : "h8" },
	};

	std::string first_output_name;
	OS << "	PixelOutput Out = (PixelOutput)0;" << std::endl;
	for (int i = 0; i < sizeof(table) / sizeof(*table); ++i)
	{
		if (m_parr.HasParam(PF_PARAM_NONE, "float4", table[i].second))
		{
			OS << "	Out." << table[i].first << " = " << table[i].second << ";" << std::endl;
			if (first_output_name.empty()) first_output_name = table[i].first;
		}
	}
	if (m_ctrl & CELL_GCM_SHADER_CONTROL_DEPTH_EXPORT)
	{
		/**
		 * Note: Naruto Shippuden : Ultimate Ninja Storm 2 sets CELL_GCM_SHADER_CONTROL_32_BITS_EXPORTS in a shader
		 * but it writes depth in r1.z and not h2.z.
		 * Maybe there's a different flag for depth ?
		 */
		//		OS << "	Out.depth = " << ((m_ctrl & CELL_GCM_SHADER_CONTROL_32_BITS_EXPORTS) ? "r1.z;" : "h2.z;") << std::endl;
		OS << "	Out.depth = r1.z;\n";
	}
	// Shaders don't always output colors (for instance if they write to depth only)
	if (!first_output_name.empty())
		OS << "	if (isAlphaTested && Out." << first_output_name << ".a <= alphaRef) discard;\n";
	OS << "	return Out;" << std::endl;
	OS << "}" << std::endl;
}
#endif
