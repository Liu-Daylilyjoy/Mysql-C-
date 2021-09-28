#include <iostream>
#include "ownsql.h"
#include <memory>


int main(int argc,char** argv){
    std::shared_ptr<sqltrue> sql(new sqltrue);

    if(argc!=2){
        std::cout << "use 2 paragamer";

        exit(1);
    }

    sql->request(argv[1][0]);

    exit(0);
}