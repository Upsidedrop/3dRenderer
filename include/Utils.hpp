#pragma once

namespace Utils{
    template<typename T>
    int getSize(int p_length){
        return p_length * sizeof(T);
    }
}