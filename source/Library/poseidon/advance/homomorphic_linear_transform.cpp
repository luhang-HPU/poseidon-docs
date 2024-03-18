//
// Created by lxs on 23-6-19.
//

#include "poseidon/advance/homomorphic_linear_transform.h"
//
namespace poseidon{
    bool IsInSliceInt(int x,const vector<int>& slice){
        for(int i : slice){
            if(i == x){
                return true;
            }
        }
        return false;
    }
}
