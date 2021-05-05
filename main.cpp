#include "newSIR.hpp"
#include <cassert>
#include <iostream>

int main () {

    epidemia::SIR mondo = epidemia::insert();
    mondo.riempimento();
    mondo.print();

}

