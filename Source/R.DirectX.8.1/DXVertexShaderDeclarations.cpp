/*
Copyright (c) 2022 AABBWare

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "DX.h"
#include "DXVertexShaderConstants.h"

namespace Renderer
{
    namespace External
    {
        namespace DX
        {
            namespace Shader
            {
                namespace Vertex
                {
                    const DWORD DECLARATION_XYZ_NORMAL_UV[]
                    {
                        D3DVSD_STREAM(0),
                        D3DVSD_REG(D3DVSDE_POSITION, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_NORMAL, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2),
                        D3DVSD_END()
                    };

                    const DWORD DECLARATION_XYZ_DIFFUSE_UV[]
                    {
                        D3DVSD_STREAM(0),
                        D3DVSD_REG(D3DVSDE_POSITION, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_DIFFUSE, D3DVSDT_D3DCOLOR),
                        D3DVSD_REG(D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2),
                        D3DVSD_END()
                    };

                    const DWORD DECLARATION_UNKNOWN[]
                    {
                        D3DVSD_STREAM(0),
                        D3DVSD_REG(D3DVSDE_POSITION, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_TEXCOORD3, D3DVSDT_D3DCOLOR),
                        D3DVSD_REG(D3DVSDE_DIFFUSE, D3DVSDT_D3DCOLOR),
                        D3DVSD_REG(D3DVSDE_TEXCOORD4, D3DVSDT_D3DCOLOR),
                        D3DVSD_REG(D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2),
                        D3DVSD_END()
                    };

                    const DWORD DECLARATION_XYZRHW_DIFFUSE_SPECULAR_UV[]
                    {
                        D3DVSD_STREAM(0),
                        D3DVSD_REG(D3DVSDE_POSITION, D3DVSDT_FLOAT4),
                        D3DVSD_REG(D3DVSDE_DIFFUSE, D3DVSDT_D3DCOLOR),
                        D3DVSD_REG(D3DVSDE_SPECULAR, D3DVSDT_D3DCOLOR),
                        D3DVSD_REG(D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2),
                        D3DVSD_END()
                    };

                    const DWORD DECLARATION_SKELETON[]
                    {
                        D3DVSD_STREAM(0),
                        D3DVSD_REG(D3DVSDE_POSITION2, D3DVSDT_D3DCOLOR),
                        D3DVSD_REG(D3DVSDE_POSITION, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_BLENDWEIGHT, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_BLENDINDICES, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_NORMAL, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_PSIZE, D3DVSDT_FLOAT4),
                        D3DVSD_REG(D3DVSDE_DIFFUSE, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_SPECULAR, D3DVSDT_FLOAT4),
                        D3DVSD_REG(D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2),
                        D3DVSD_REG(D3DVSDE_TEXCOORD1, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_TEXCOORD2, D3DVSDT_FLOAT3),
                        D3DVSD_END()
                    };

                    const DWORD DECLARATION_BASIS[]
                    {
                        D3DVSD_STREAM(0),
                        D3DVSD_REG(D3DVSDE_POSITION, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_NORMAL, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2),
                        D3DVSD_REG(D3DVSDE_BLENDWEIGHT, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_BLENDINDICES, D3DVSDT_FLOAT3),
                        D3DVSD_REG(D3DVSDE_TEXCOORD1, D3DVSDT_FLOAT2),
                        D3DVSD_REG(D3DVSDE_PSIZE, D3DVSDT_FLOAT3),
                        D3DVSD_END()
                    };
                }
            }
        }
    }
}