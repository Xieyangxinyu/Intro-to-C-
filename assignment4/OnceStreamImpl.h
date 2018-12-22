#pragma once

#include "StreamImpl.h"

namespace stream
{
    
    template <typename T>
    class OnceStreamImpl: public StreamImpl<T> {
    private:
        T val;
        bool stay;
    public:
        OnceStreamImpl(T val): val(val) {
            stay = true;
        };
        
        Optional<T> next() override{
            if(stay){
                stay = false;
                return Optional<T>(val);
            }else return Optional<T>();
        }
    };
    
}
