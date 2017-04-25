//
//  UtilitiesIO.cpp
//  Gauss
//
//  Created by David Levin on 4/23/17.
//
//

#include <UtilitiesIO.h>

//Written by Desai Chen for SIMIT
int Gauss::openIfstream(std::ifstream &in, std::string filename)
{
    in.open(filename);
    if(!in.good()){
        std::cout<<"Cannot read "<<filename<<"\n";
        return -1;
    }
    return 0;
}


//Lions share of code borrowed from Desai Chen for SIMIT
void Gauss::readTetgen(Eigen::MatrixXd &V, Eigen::MatrixXi &F, const std::string nodeFile, const std::string eleFile) {
    
    std::cout<<"Reading tetrahedral mesh from "<<nodeFile<<" and "<<eleFile<<"\n";
    
    std::ifstream nodeIn, eleIn;
    
    int status = openIfstream(nodeIn, nodeFile);
    if(status<0){
        return;
    }
    status = openIfstream(eleIn, eleFile);
    if(status<0){
        return;
    }
    
    status = loadTet(V, F, nodeIn, eleIn);
    nodeIn.close();
    eleIn.close();
    return;
}

int Gauss::loadTet(Eigen::MatrixXd &V, Eigen::MatrixXi &F, std::istream & nodeIn, std::istream & eleIn) {
    
    //load vertices
    int intVal;
    nodeIn>>intVal;
    
    V.resize(intVal,3); //resize vertex array
    std::string line;
    unsigned int cnt = 0;
    //discard rest of the first line.
    getline(nodeIn,line);
    while(1) {
        getline(nodeIn,line);
        if(nodeIn.eof()) {
            break;
        }
        //skip empty lines
        if(line.size()<3) {
            continue;
        }
        //skip comments
        if(line.at(0)=='#') {
            continue;
        }
        std::stringstream ss(line);
        
        ss>>intVal;
        for(int ii = 0;ii<3;ii++){
            ss>>V(cnt,ii);
        }
        cnt ++ ;
        if(cnt>=V.rows()){
            break;
        }
    }
    
    //load elements
    eleIn>>intVal;
    F.resize(intVal, 4); //resize face list
    
    int nV=0;
    eleIn>>nV;
    cnt = 0;
    getline(eleIn,line);
    while(1) {
        getline(eleIn,line);
        if(eleIn.eof()) {
            break;
        }
        if(line.size()<3) {
            continue;
        }
        if(line.at(0)=='#') {
            continue;
        }
        std::stringstream ss(line);
        ss>>intVal;
        
        //e[cnt].resize(nV);
        for(int ii = 0;ii<nV;ii++){
            ss>>F(cnt,ii);
        }
        
        cnt ++ ;
        if(cnt>=F.rows()){
            break;
        }
    }
    return 0;
}