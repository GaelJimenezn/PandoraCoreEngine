#include "DeviceContext.h"

void 
DeviceContext::destroy() {
  SAFE_RELEASE(m_deviceContext);
}

void
DeviceContext::rsSetViewports(unsigned int numViewports,
                              const D3D11_VIEWPORT* pViewports) {
  if (!pViewports) {
    ERROR("DeviceContext", "rsSetViewports", "pViewports is nullptr");
    return;
  }
  m_deviceContext->RSSetViewports(numViewports, pViewports);
}

void
DeviceContext::psSetShaderResources(
    unsigned int startSlot, 
    unsigned int numViews, 
    ID3D11ShaderResourceView* const* ppShaderResourceViews) {
  if (!ppShaderResourceViews) {
    ERROR("DeviceContext", "psSetShaderResources", 
          "ppShaderResourceViews is nullptr");
    return;
  }
  m_deviceContext->PSSetShaderResources(startSlot, 
                                        numViews, 
                                        ppShaderResourceViews);
}

void
DeviceContext::iaSetInputLayout(ID3D11InputLayout* pInputLayout) {
  if (!pInputLayout) {
    ERROR("DeviceContext", "iaSetInputLayout", "pInputLayout is nullptr");
    return;
  }
  m_deviceContext->IASetInputLayout(pInputLayout);
}

void
DeviceContext::vsSetShader(ID3D11VertexShader* pVertexShader, 
                           ID3D11ClassInstance* const* ppClassInstances, 
                           unsigned int numClassInstances) {
  if (!pVertexShader) {
    ERROR("DeviceContext", "vsSetShader", "pVertexShader is nullptr");
    return;
  }
  m_deviceContext->VSSetShader(pVertexShader, 
                               ppClassInstances, 
                               numClassInstances);
}

void
DeviceContext::psSetShader(ID3D11PixelShader* pPixelShader, 
                           ID3D11ClassInstance* const* ppClassInstances, 
                           unsigned int numClassInstances) {
  if (!pPixelShader) {
    ERROR("DeviceContext", "psSetShader", "pPixelShader is nullptr");
    return;
  }
  m_deviceContext->PSSetShader(pPixelShader, 
                               ppClassInstances, 
                               numClassInstances);
}

void
DeviceContext::updateSubresource(ID3D11Resource* pDstResource, 
                                 unsigned int dstSubresource, 
                                 const D3D11_BOX* pDstBox, 
                                 const void* pSrcData, 
                                 unsigned int srcRowPitch, 
                                 unsigned int srcDepthPitch) {
  if (!pDstResource || !pSrcData) {
    ERROR("DeviceContext", "updateSubresource", 
          "pDstResource or pSrcData is nullptr");
    return;
  }
  m_deviceContext->UpdateSubresource(pDstResource, 
                                     dstSubresource, 
                                     pDstBox, 
                                     pSrcData, 
                                     srcRowPitch, 
                                     srcDepthPitch);
}

void
DeviceContext::iaSetVertexBuffers(
    unsigned int startSlot, 
    unsigned int numBuffers, 
    ID3D11Buffer* const* ppVertexBuffers, 
    const unsigned int* pStrides, 
    const unsigned int* pOffsets) {
  if (!ppVertexBuffers || !pStrides || !pOffsets) {
    ERROR("DeviceContext", "iaSetVertexBuffers", 
          "ppVertexBuffers, pStrides, or pOffsets is nullptr");
    return;
  }
  m_deviceContext->IASetVertexBuffers(startSlot, 
                                      numBuffers, 
                                      ppVertexBuffers, 
                                      pStrides, 
                                      pOffsets);
}

void
DeviceContext::iaSetIndexBuffer(ID3D11Buffer* pIndexBuffer, 
                                DXGI_FORMAT format, 
                                unsigned int offset) {
  if (!pIndexBuffer) {
    ERROR("DeviceContext", "iaSetIndexBuffer", "pIndexBuffer is nullptr");
    return;
  }
  m_deviceContext->IASetIndexBuffer(pIndexBuffer, format, offset);
}

void
DeviceContext::psSetSamplers(unsigned int startSlot, 
                             unsigned int numSamplers, 
                             ID3D11SamplerState* const* ppSamplers) {
  if (!ppSamplers) {
    ERROR("DeviceContext", "psSetSamplers", "ppSamplers is nullptr");
    return;
  }
  m_deviceContext->PSSetSamplers(startSlot, numSamplers, ppSamplers);
}

void
DeviceContext::rsSetState(ID3D11RasterizerState* pRasterizerState) {
  if (!pRasterizerState) {
    ERROR("DeviceContext", "rsSetState", "pRasterizerState is nullptr");
    return;
  }
  m_deviceContext->RSSetState(pRasterizerState);
}

void
DeviceContext::omSetBlendState(ID3D11BlendState* pBlendState, 
                               const float blendFactor[4], 
                               unsigned int sampleMask) {
  if (!pBlendState) {
    ERROR("DeviceContext", "omSetBlendState", "pBlendState is nullptr");
    return;
  }
  m_deviceContext->OMSetBlendState(pBlendState, blendFactor, sampleMask);
}

void
DeviceContext::omSetRenderTargets(
    unsigned int numViews,
    ID3D11RenderTargetView* const* ppRenderTargetViews,
    ID3D11DepthStencilView* pDepthStencilView) {
  if (!ppRenderTargetViews && !pDepthStencilView) {
    ERROR("DeviceContext", "omSetRenderTargets", "All view pointers are null");
    return;
  }
  m_deviceContext->OMSetRenderTargets(numViews, 
                                      ppRenderTargetViews, 
                                      pDepthStencilView);
}

void
DeviceContext::iaSetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology) {
  m_deviceContext->IASetPrimitiveTopology(topology);
}

void
DeviceContext::clearRenderTargetView(
    ID3D11RenderTargetView* pRenderTargetView, 
    const float colorRGBA[4]) {
  if (!pRenderTargetView) {
    ERROR("DeviceContext", "clearRenderTargetView", 
          "pRenderTargetView is nullptr");
    return;
  }
  if (!colorRGBA) {
    ERROR("DeviceContext", "clearRenderTargetView", "colorRGBA is nullptr");
    return;
  }
  m_deviceContext->ClearRenderTargetView(pRenderTargetView, colorRGBA);
}

void
DeviceContext::clearDepthStencilView(
    ID3D11DepthStencilView* pDepthStencilView, 
    unsigned int clearFlags, 
    float depth, 
    UINT8 stencil) {
  if (!pDepthStencilView) {
    ERROR("DeviceContext", "clearDepthStencilView", 
          "pDepthStencilView is nullptr");
    return;
  }
  m_deviceContext->ClearDepthStencilView(pDepthStencilView, 
                                         clearFlags, 
                                         depth, 
                                         stencil);
}

void
DeviceContext::vsSetConstantBuffers(
    unsigned int startSlot, 
    unsigned int numBuffers, 
    ID3D11Buffer* const* ppConstantBuffers) {
  if (!ppConstantBuffers) {
    ERROR("DeviceContext", "vsSetConstantBuffers", 
          "ppConstantBuffers is nullptr");
    return;
  }
  m_deviceContext->VSSetConstantBuffers(startSlot, 
                                        numBuffers, 
                                        ppConstantBuffers);
}

void
DeviceContext::psSetConstantBuffers(
    unsigned int startSlot, 
    unsigned int numBuffers, 
    ID3D11Buffer* const* ppConstantBuffers) {
  if (!ppConstantBuffers) {
    ERROR("DeviceContext", "psSetConstantBuffers", 
          "ppConstantBuffers is nullptr");
    return;
  }
  m_deviceContext->PSSetConstantBuffers(startSlot, 
                                        numBuffers, 
                                        ppConstantBuffers);
}

void
DeviceContext::drawIndexed(unsigned int indexCount, 
                           unsigned int startIndexLocation, 
                           int baseVertexLocation) {
  if (indexCount == 0) {
    ERROR("DeviceContext", "drawIndexed", "IndexCount is zero");
    return;
  }
  m_deviceContext->DrawIndexed(indexCount, 
                               startIndexLocation, 
                               baseVertexLocation);
}