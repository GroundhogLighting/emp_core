#include "../include/emp_core.h"
//#include "emp_model/emp_model.h"



/*
TEST(MaterialTest, Dielectric) {
    double r = 0.5;
    double g = 0.6;
    double b = 0.9;
    double refractionIndex = 1.5;
    double hartmannConstant = 0.0;
    
    std::string rad = "void dielectric %MAT_NAME% 0 0 5 " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " " + std::to_string(refractionIndex)+" " + std::to_string(hartmannConstant);
    std::string c = "dielectric";
    std::string name = "theMaterial";
    
    json j = json();
    j["class"]=c;
    j["name"]=name;
    j["rad"]=rad;
    
    Dielectric mat = Dielectric(&j);
    
    // Test From JSON
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    ASSERT_EQ(mat.refractionIndex,refractionIndex);
    ASSERT_EQ(mat.hartmannConstant,hartmannConstant);
    
    // Test write to Radiance.
    std::string fileName = "material.mat";
    FOPEN(file, &fileName[0], "w");
    mat.writeInRadianceFormat(file);
    fclose(file);
    
    std::ifstream in;
    in.open(fileName);
    std::string line;
    
    std::string newRad = "";
    while(std::getline(in, line)){
        newRad += (line+" ");
    }
    
    json newJson = json();
    newJson["class"]=c;
    newJson["name"]=name;
    newJson["rad"]=newRad;

    Dielectric newMat = Dielectric(&newJson);
    
    ASSERT_EQ(mat.name,newMat.name);
    ASSERT_EQ(mat.type,newMat.type);
    ASSERT_EQ(mat.r,newMat.r);
    ASSERT_EQ(mat.g,newMat.g);
    ASSERT_EQ(mat.b,newMat.b);
    ASSERT_EQ(mat.refractionIndex,newMat.refractionIndex);
    ASSERT_EQ(mat.hartmannConstant,newMat.hartmannConstant);
    
    // Add to model
    EmpModel model = EmpModel();
    ASSERT_TRUE(nullptr != model.addMaterial(&newJson));
    
    remove(fileName.c_str());
}



TEST(MaterialTest, Glass) {
    double r = 0.5;
    double g = 0.6;
    double b = 0.9;
    
    std::string rad = "void glass %MAT_NAME% 0 0 3 " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b);
    std::string c = "glass";
    std::string name = "theMaterial";
    
    json j = json();
    j["class"]=c;
    j["name"]=name;
    j["rad"]=rad;
    
    Glass mat = Glass(&j);
    
    // Test From JSON
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    
    // Test write to Radiance.
    std::string fileName = "material.mat";
    FOPEN(file, &fileName[0], "w");
    mat.writeInRadianceFormat(file);
    fclose(file);
    
    std::ifstream in;
    in.open(fileName);
    std::string line;
    
    std::string newRad = "";
    while(std::getline(in, line)){
        newRad += (line+" ");
    }
    
    json newJson = json();
    newJson["class"]=c;
    newJson["name"]=name;
    newJson["rad"]=newRad;
    
    Glass newMat = Glass(&newJson);
    
    ASSERT_EQ(mat.name,newMat.name);
    ASSERT_EQ(mat.type,newMat.type);
    ASSERT_EQ(mat.r,newMat.r);
    ASSERT_EQ(mat.g,newMat.g);
    ASSERT_EQ(mat.b,newMat.b);
    
    // Add to model
    EmpModel model = EmpModel();
    ASSERT_TRUE(nullptr != model.addMaterial(&newJson));
    
    remove(fileName.c_str());
}



TEST(MaterialTest, Glow) {
    double r = 0.5;
    double g = 0.6;
    double b = 0.9;
    double maxRadius = 1.5;
    
    std::string rad = "void glow %MAT_NAME% 0 0 4 " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " " + std::to_string(maxRadius);
    std::string c = "glow";
    std::string name = "theMaterial";
    
    json j = json();
    j["class"]=c;
    j["name"]=name;
    j["rad"]=rad;
    
    Glow mat = Glow(&j);
    
    // Test From JSON
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    ASSERT_EQ(mat.maxRadius,maxRadius);
    
    // Test write to Radiance.
    std::string fileName = "material.mat";
    FOPEN(file, &fileName[0], "w");
    mat.writeInRadianceFormat(file);
    fclose(file);
    
    std::ifstream in;
    in.open(fileName);
    std::string line;
    
    std::string newRad = "";
    while(std::getline(in, line)){
        newRad += (line+" ");
    }
    
    json newJson = json();
    newJson["class"]=c;
    newJson["name"]=name;
    newJson["rad"]=newRad;
    
    Glow newMat = Glow(&newJson);
    
    ASSERT_EQ(mat.name,newMat.name);
    ASSERT_EQ(mat.type,newMat.type);
    ASSERT_EQ(mat.r,newMat.r);
    ASSERT_EQ(mat.g,newMat.g);
    ASSERT_EQ(mat.b,newMat.b);
    ASSERT_EQ(mat.maxRadius,newMat.maxRadius);
    
    // Add to model
    EmpModel model = EmpModel();
    ASSERT_TRUE(nullptr != model.addMaterial(&newJson));
    
    remove(fileName.c_str());
}


TEST(MaterialTest, Interface) {
    double r1 = 0.5;
    double g1 = 0.6;
    double b1 = 0.9;
    double refractionIndex1 = 1.5;
    
    double r2 = 0.5112;
    double g2 = 0.63434;
    double b2 = 0.9141;
    double refractionIndex2 = 1.7;
    
    std::string rad = "void interface %MAT_NAME% 0 0 8 " +
        std::to_string(r1) + " " + std::to_string(g1) + " " + std::to_string(b1) + " " + std::to_string(refractionIndex1) + " "+
        std::to_string(r2) + " " + std::to_string(g2) + " " + std::to_string(b2) + " " + std::to_string(refractionIndex2);
    std::string c = "interface";
    std::string name = "theMaterial";
    
    json j = json();
    j["class"]=c;
    j["name"]=name;
    j["rad"]=rad;
    
    Interface mat = Interface(&j);
    
    // Test From JSON
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r1,r1);
    ASSERT_EQ(mat.g1,g1);
    ASSERT_EQ(mat.b1,b1);
    ASSERT_EQ(mat.refractionIndex1,refractionIndex1);
    ASSERT_EQ(mat.r2,r2);
    ASSERT_EQ(mat.g2,g2);
    ASSERT_EQ(mat.b2,b2);
    ASSERT_EQ(mat.refractionIndex2,refractionIndex2);
    
    // Test write to Radiance.
    std::string fileName = "material.mat";
    FOPEN(file, &fileName[0], "w");
    mat.writeInRadianceFormat(file);
    fclose(file);
    
    std::ifstream in;
    in.open(fileName);
    std::string line;
    
    std::string newRad = "";
    while(std::getline(in, line)){
        newRad += (line+" ");
    }
    
    json newJson = json();
    newJson["class"]=c;
    newJson["name"]=name;
    newJson["rad"]=newRad;
    
    Interface newMat = Interface(&newJson);
    
    ASSERT_EQ(mat.name,newMat.name);
    ASSERT_EQ(mat.type,newMat.type);
    ASSERT_EQ(mat.r1,newMat.r1);
    ASSERT_EQ(mat.g1,newMat.g1);
    ASSERT_EQ(mat.b1,newMat.b1);
    ASSERT_EQ(mat.refractionIndex1,newMat.refractionIndex1);
    ASSERT_EQ(mat.r2,newMat.r2);
    ASSERT_EQ(mat.g2,newMat.g2);
    ASSERT_EQ(mat.b2,newMat.b2);
    ASSERT_EQ(mat.refractionIndex2,newMat.refractionIndex2);
    
    // Add to model
    EmpModel model = EmpModel();
    ASSERT_TRUE(nullptr != model.addMaterial(&newJson));
    
    remove(fileName.c_str());
}




TEST(MaterialTest, Light) {
    double r = 0.5;
    double g = 0.6;
    double b = 0.9;
    
    std::string rad = "void light %MAT_NAME% 0 0 3 " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b);
    std::string c = "light";
    std::string name = "theMaterial";
    
    json j = json();
    j["class"]=c;
    j["name"]=name;
    j["rad"]=rad;
    
    Light mat = Light(&j);
    
    // Test From JSON
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    
    // Test write to Radiance.
    std::string fileName = "material.mat";
    FOPEN(file, &fileName[0], "w");
    mat.writeInRadianceFormat(file);
    fclose(file);
    
    std::ifstream in;
    in.open(fileName);
    std::string line;
    
    std::string newRad = "";
    while(std::getline(in, line)){
        newRad += (line+" ");
    }
    
    json newJson = json();
    newJson["class"]=c;
    newJson["name"]=name;
    newJson["rad"]=newRad;
    
    Light newMat = Light(&newJson);
    
    ASSERT_EQ(mat.name,newMat.name);
    ASSERT_EQ(mat.type,newMat.type);
    ASSERT_EQ(mat.r,newMat.r);
    ASSERT_EQ(mat.g,newMat.g);
    ASSERT_EQ(mat.b,newMat.b);
    
    // Add to model
    EmpModel model = EmpModel();
    ASSERT_TRUE(nullptr != model.addMaterial(&newJson));
    
    remove(fileName.c_str());
    
}



TEST(MaterialTest, Metal) {
    double r = 0.5;
    double g = 0.6;
    double b = 0.9;
    double spec = 0.0;
    double rough = 0.0;
    
    std::string rad = "void metal %MAT_NAME% 0 0 5 " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " " + std::to_string(spec)+" " + std::to_string(rough);
    std::string c = "metal";
    std::string name = "theMaterial";
    
    json j = json();
    j["class"]=c;
    j["name"]=name;
    j["rad"]=rad;
    
    Metal mat = Metal(&j);
    
    // Test From JSON
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    ASSERT_EQ(mat.specularity,spec);
    ASSERT_EQ(mat.roughness,rough);
    
    // Test write to Radiance.
    std::string fileName = "material.mat";
    FOPEN(file, &fileName[0], "w");
    mat.writeInRadianceFormat(file);
    fclose(file);
    
    std::ifstream in;
    in.open(fileName);
    std::string line;
    
    std::string newRad = "";
    while(std::getline(in, line)){
        newRad += (line+" ");
    }
    
    json newJson = json();
    newJson["class"]=c;
    newJson["name"]=name;
    newJson["rad"]=newRad;
    
    Metal newMat = Metal(&newJson);
    
    ASSERT_EQ(mat.name,newMat.name);
    ASSERT_EQ(mat.type,newMat.type);
    ASSERT_EQ(mat.r,newMat.r);
    ASSERT_EQ(mat.g,newMat.g);
    ASSERT_EQ(mat.b,newMat.b);
    ASSERT_EQ(mat.specularity,newMat.specularity);
    ASSERT_EQ(mat.roughness,newMat.roughness);
    
    // Add to model
    EmpModel model = EmpModel();
    ASSERT_TRUE(nullptr != model.addMaterial(&newJson));
    
    remove(fileName.c_str());
}


TEST(MaterialTest, Plastic) {
    double r = 0.5;
    double g = 0.6;
    double b = 0.9;
    double spec = 0.0;
    double rough = 0.0;
    
    std::string rad = "void plastic %MAT_NAME% 0 0 5 " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " " + std::to_string(spec)+" " + std::to_string(rough);
    std::string c = "plastic";
    std::string name = "theMaterial";
    
    json j = json();
    j["class"]=c;
    j["name"]=name;
    j["rad"]=rad;
    
    Plastic mat = Plastic(&j);
    
    // Test From JSON
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    ASSERT_EQ(mat.specularity,spec);
    ASSERT_EQ(mat.roughness,rough);
    
    // Test write to Radiance.
    std::string fileName = "material.mat";
    FOPEN(file, &fileName[0], "w");
    mat.writeInRadianceFormat(file);
    fclose(file);
    
    std::ifstream in;
    in.open(fileName);
    std::string line;
    
    std::string newRad = "";
    while(std::getline(in, line)){
        newRad += (line+" ");
    }
    
    json newJson = json();
    newJson["class"]=c;
    newJson["name"]=name;
    newJson["rad"]=newRad;
    
    Plastic newMat = Plastic(&newJson);
    
    ASSERT_EQ(mat.name,newMat.name);
    ASSERT_EQ(mat.type,newMat.type);
    ASSERT_EQ(mat.r,newMat.r);
    ASSERT_EQ(mat.g,newMat.g);
    ASSERT_EQ(mat.b,newMat.b);
    ASSERT_EQ(mat.specularity,newMat.specularity);
    ASSERT_EQ(mat.roughness,newMat.roughness);
    
    // Add to model
    EmpModel model = EmpModel();
    ASSERT_TRUE(nullptr != model.addMaterial(&newJson));
    
    remove(fileName.c_str());
}


TEST(MaterialTest, Spotlight) {
    double r = 0.5;
    double g = 0.6;
    double b = 0.9;
    double angle = 0.0;
    double xdir = 0.0;
    double ydir = 1.0;
    double zdir = 2.0;
    
    std::string rad = "void spotlight %MAT_NAME% 0 0 7 " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " " + std::to_string(angle)+" " + std::to_string(xdir)+" " + std::to_string(ydir)+" " + std::to_string(zdir);
    std::string c = "spotlight";
    std::string name = "theMaterial";
    
    json j = json();
    j["class"]=c;
    j["name"]=name;
    j["rad"]=rad;
    
    Spotlight mat = Spotlight(&j);
    
    // Test From JSON
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    ASSERT_EQ(mat.angle,angle);
    ASSERT_EQ(mat.direction.getX(),xdir);
    ASSERT_EQ(mat.direction.getY(),ydir);
    ASSERT_EQ(mat.direction.getZ(),zdir);
    
    // Test write to Radiance.
    std::string fileName = "material.mat";
    FOPEN(file, &fileName[0], "w");
    mat.writeInRadianceFormat(file);
    fclose(file);
    
    std::ifstream in;
    in.open(fileName);
    std::string line;
    
    std::string newRad = "";
    while(std::getline(in, line)){
        newRad += (line+" ");
    }
    
    json newJson = json();
    newJson["class"]=c;
    newJson["name"]=name;
    newJson["rad"]=newRad;
    
    Spotlight newMat = Spotlight(&newJson);
    
    ASSERT_EQ(mat.name,newMat.name);
    ASSERT_EQ(mat.type,newMat.type);
    ASSERT_EQ(mat.r,newMat.r);
    ASSERT_EQ(mat.g,newMat.g);
    ASSERT_EQ(mat.b,newMat.b);
    ASSERT_EQ(mat.angle,newMat.angle);
    ASSERT_EQ(mat.direction.getX(),newMat.direction.getX());
    ASSERT_EQ(mat.direction.getY(),newMat.direction.getY());
    ASSERT_EQ(mat.direction.getZ(),newMat.direction.getZ());
    
    // Add to model
    EmpModel model = EmpModel();
    ASSERT_TRUE(nullptr != model.addMaterial(&newJson));
    
    remove(fileName.c_str());
}


TEST(MaterialTest, Trans) {
    double r = 0.5;
    double g = 0.6;
    double b = 0.9;
    double specularity = 0.123;
    double roughness = 0.1;
    double transmissivity = 0.8;
    double tspec = 0.8;
    
    std::string rad = "void trans %MAT_NAME% 0 0 7 " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " " + std::to_string(specularity)+" " + std::to_string(roughness)+" " + std::to_string(transmissivity)+" " + std::to_string(tspec);
    std::string c = "trans";
    std::string name = "theMaterial";
    
    json j = json();
    j["class"]=c;
    j["name"]=name;
    j["rad"]=rad;
    
    Trans mat = Trans(&j);
    
    // Test From JSON
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    ASSERT_EQ(mat.specularity,specularity);
    ASSERT_EQ(mat.roughness,roughness);
    ASSERT_EQ(mat.transmissivity,transmissivity);
    ASSERT_EQ(mat.tspec,tspec);
    
    // Test write to Radiance.
    std::string fileName = "material.mat";
    FOPEN(file, &fileName[0], "w");
    mat.writeInRadianceFormat(file);
    fclose(file);
    
    std::ifstream in;
    in.open(fileName);
    std::string line;
    
    std::string newRad = "";
    while(std::getline(in, line)){
        newRad += (line+" ");
    }
    
    json newJson = json();
    newJson["class"]=c;
    newJson["name"]=name;
    newJson["rad"]=newRad;
    
    Trans newMat = Trans(&newJson);
    
    ASSERT_EQ(mat.name,newMat.name);
    ASSERT_EQ(mat.type,newMat.type);
    ASSERT_EQ(mat.r,newMat.r);
    ASSERT_EQ(mat.g,newMat.g);
    ASSERT_EQ(mat.b,newMat.b);
    ASSERT_EQ(mat.specularity,newMat.specularity);
    ASSERT_EQ(mat.roughness,newMat.roughness);
    ASSERT_EQ(mat.transmissivity,newMat.transmissivity);
    ASSERT_EQ(mat.tspec,newMat.tspec);
    
    // Add to model
    EmpModel model = EmpModel();
    ASSERT_TRUE(nullptr != model.addMaterial(&newJson));
    
    remove(fileName.c_str());
    
}
 */




