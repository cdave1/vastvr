#ifndef _VAST_PIXEL_BUFFER_OBJECT_H_
#define _VAST_PIXEL_BUFFER_OBJECT_H_

#include <Vast/VastMath.h>
#define MAX_PIXEL_BUFFER_OBJECTS 4

typedef uint32_t depthPixelDataType_t;

namespace hd {
    class Entity;

    namespace graphics {
        
        class PixelBufferObject {
        private:
            Size2 m_size;

            uint32_t m_pixelCount;

            size_t m_colorPixelSize;

            uint8_t *m_colorCopyBuffer;

            uint32_t m_colorPixelBufferIds[MAX_PIXEL_BUFFER_OBJECTS];

            uint32_t m_totalColorPixelBuffers;

            int m_colorVram2sys;

            int m_colorGpu2vram;

            
        public:

            PixelBufferObject();

            ~PixelBufferObject();
            
            void Reset();
            
            void SetSize(const Size2 &size);
            
            void Update();
            
            Size2 GetSize() const;
            
            Color4f GetColor(const Vec2 &offset) const;
            
        };
    }
}

#endif