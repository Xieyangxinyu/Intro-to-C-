#pragma once

#include "StreamImpl.h"

namespace stream
{
    
    template <typename T>
    class ChainStreamImpl: public StreamImpl<T> {
    private:
        Stream<T> first, second;
        Optional<T> temp;
    public:
        ChainStreamImpl(Stream<T> first, Stream<T> second):first(first), second(second){};
        
        Optional<T> next() override{
            temp = first.next();
            if((bool)temp)
                return temp;
            else {
                temp = second.next();
                if((bool)temp)
                    return temp;
                else
                    return Optional<T>();
            }
        }
    };
    
}
