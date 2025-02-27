#include "numeros.h"

//Padrões dos números
const int ZERO[5][5][3] = {
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}}
};
const int UM[5][5][3] = {
    {{0, 0, 0}, {0, 0, 0}, {33, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {33, 0, 0}, {33, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{33, 0, 0}, {0, 0, 0}, {33, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {33, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {33, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};
const int DOIS[5][5][3] = {
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}}
};

const int TRES[5][5][3] = {
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}}
};

const int QUATRO[5][5][3] = {
    {{33, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}}
};

const int CINCO[5][5][3] = {
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}}
};

const int SEIS[5][5][3] = {
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}}
};

const int SETE[5][5][3] = {
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {33, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {33, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};

const int OITO[5][5][3] = {
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}}
};

const int NOVE[5][5][3] = {
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{33, 0, 0}, {33, 0, 0}, {33,0, 0}, {33, 0, 0}, {33, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {33, 0, 0}}
};