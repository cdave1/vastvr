#include "PointKernel.h"

using namespace hd::graphics;

PointKernel::PointKernel(unsigned int ID) : Component(ID),
    m_kernel(NULL) {
    
    m_kernel = new hd::Vector<Vec3>(DEFAULT_KERNEL_SIZE);
    Reset();
}


void PointKernel::Reset() {
    m_kernel->Clear();
}


void PointKernel::GenerateSSAO(unsigned size) {
    m_kernel->Clear();
    
	for (unsigned i = 0; i < size; ++i) {
        Vec3 vec(hd::randomf(-1.0f, 1.0f),
                 hd::randomf(-1.0f, 1.0f),
                 hd::randomf(-1.0f, 1.0f));
        vec = hd::normalize(vec);
        vec *= hd::randomf(0.0f, 1.0f);
        
        float scale = float(i) / float(size);
        vec *= hd::lerp(0.001f, 1.0f, scale  * scale);
        m_kernel->Add(vec);
    }
}


void PointKernel::GenerateKernel(unsigned size) {
    m_kernel->Clear();
    
	for (unsigned i = 0; i < size; ++i) {
        Vec3 vec(hd::randomf(-1.0f, 1.0f),
                 hd::randomf(-1.0f, 1.0f),
                 hd::randomf(-1.0f, 1.0f));
        vec = hd::normalize(vec);
        vec *= hd::randomf(0.0f, 1.0f);
        
        float scale = float(i) / float(size);
        vec *= hd::lerp(0.001f, 1.0f, scale * scale);
        m_kernel->Add(vec);
    }
}


unsigned int PointKernel::GetKernelSize() const {
    return m_kernel->GetSize();
}


Vec3 * PointKernel::GetKernelLocation() const {
    Vec3 *location = m_kernel->GetItems();
    return location;
}
