#include "groundhogmodel/groundhogmodel.h"




TEST(MaterialTest, Dielectric_FromJSON) {
    double r = 0.5;
    double g = 0.6;
    double b = 0.9;
    double refractionIndex = 1.5;
    double hartmanConstant = 0.0;
    
    std::string rad = "void dielectric %MAT_NAME% 0 0 5 " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " " + std::to_string(refractionIndex)+" " + std::to_string(hartmanConstant);
    std::string c = "plastic";
    std::string name = "theMaterial";
    
    json j = json();
    j["class"]=c;
    j["name"]=name;
    j["rad"]=rad;
    
    Dielectric mat = Dielectric(&j);
    
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    ASSERT_EQ(mat.refractionIndex,refractionIndex);
    ASSERT_EQ(mat.hartmanConstant,hartmanConstant);
}



TEST(MaterialTest, Glass_FromJSON) {
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
    
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
}



TEST(MaterialTest, Glow_FromJSON) {
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
    
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    ASSERT_EQ(mat.maxRadius,maxRadius);
}


TEST(MaterialTest, Interface_FromJSON) {
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
}




TEST(MaterialTest, Light_FromJSON) {
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
    
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
}



TEST(MaterialTest, Metal_FromJSON) {
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
    
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    ASSERT_EQ(mat.specularity,spec);
    ASSERT_EQ(mat.roughness,rough);
}


TEST(MaterialTest, Plastic_FromJSON) {
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
    
    Metal mat = Metal(&j);
    
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    ASSERT_EQ(mat.specularity,spec);
    ASSERT_EQ(mat.roughness,rough);
}


TEST(MaterialTest, Spotlight_FromJSON) {
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
    
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    ASSERT_EQ(mat.angle,angle);
    ASSERT_EQ(mat.xdir,xdir);
    ASSERT_EQ(mat.ydir,ydir);
    ASSERT_EQ(mat.zdir,zdir);
    
}


TEST(MaterialTest, Trans_FromJSON) {
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
    
    ASSERT_EQ(mat.name,name);
    ASSERT_EQ(mat.type,c);
    ASSERT_EQ(mat.r,r);
    ASSERT_EQ(mat.g,g);
    ASSERT_EQ(mat.b,b);
    ASSERT_EQ(mat.specularity,specularity);
    ASSERT_EQ(mat.roughness,roughness);
    ASSERT_EQ(mat.transmissivity,transmissivity);
    ASSERT_EQ(mat.tspec,tspec);
    
}


