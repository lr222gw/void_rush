#include "TrashCollector.h"

TC& TC::GetInst()
{
    return s_Instance;
}

void TC::empty()
{
    for (int i = 0; i < shaderRSV.size(); i++) {
        shaderRSV[i]->Release();
    }
    for (int i = 0; i < Matrial.size(); i++) {
        delete Matrial[i];
    }
}

void TC::add(Material* matrial)
{
    Matrial.push_back(matrial);
}

void TC::add(ID3D11ShaderResourceView* SRV)
{
    //std::lock_guard<std::mutex> guard(SRV_mutex);
    shaderRSV.push_back(SRV);
    //guard.
    
}

TC TC::s_Instance;