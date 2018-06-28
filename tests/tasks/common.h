#pragma once
#include <math.h>
#include "../../include/emp_core.h"


void setOuterPercentage(GroundhogModel * model, float percent)
{
    if(percent > 0.9 || percent < 0.0)
        throw "Outer percentage too big or too small in setOuterPercentage";
    
    /* ADD LAYER */
    std::string layerName = "Layer0";
    model->addLayer(&layerName);
    
    /* ADD MATERIAL */
    Material * material = model->addDefaultMaterial();
    
    /* ADD A 20x20x20 METER CLOSED BOX */
    
    // Floor
    Polygon3D * p1 = new Polygon3D();
    Loop * outerLoop1 = p1->getOuterLoopRef();
    outerLoop1->addVertex(new Point3D(-10,-10,-10));
    outerLoop1->addVertex(new Point3D(10,-10,-10));
    outerLoop1->addVertex(new Point3D(10,10,-10));
    outerLoop1->addVertex(new Point3D(-10,10,-10));
    std::string floorName = "floor";
    Face * floor = new Face(&floorName);
    floor->setPolygon(p1);
    floor->setMaterial(material);
    model -> addObjectToLayer(&layerName,floor);
    
    // Ceiling
    Polygon3D * p2 = new Polygon3D();
    Loop * outerLoop2 = p2->getOuterLoopRef();
    outerLoop2->addVertex(new Point3D(-10,-10,10));
    outerLoop2->addVertex(new Point3D(10,-10,10));
    outerLoop2->addVertex(new Point3D(10,10,10));
    outerLoop2->addVertex(new Point3D(-10,10,10));
    std::string ceilingName = "ceiling";
    Face * ceiling = new Face(&ceilingName);
    ceiling->setPolygon(p2);
    ceiling->setMaterial(material);
    model -> addObjectToLayer(&layerName,ceiling);
    
    // North
    Polygon3D * p3 = new Polygon3D();
    Loop * outerLoop3 = p3->getOuterLoopRef();
    outerLoop3->addVertex(new Point3D(-10,10,-10));
    outerLoop3->addVertex(new Point3D( 10,10,-10));
    outerLoop3->addVertex(new Point3D( 10,10, 10));
    outerLoop3->addVertex(new Point3D( -10,10,10));
    std::string northName = "north";
    Face * north = new Face(&northName);
    north->setPolygon(p3);
    north->setMaterial(material);
    model -> addObjectToLayer(&layerName,north);
    
    // South
    Polygon3D * p4 = new Polygon3D();
    Loop * outerLoop4 = p4->getOuterLoopRef();
    outerLoop4->addVertex(new Point3D(-10,-10,-10));
    outerLoop4->addVertex(new Point3D( 10,-10,-10));
    outerLoop4->addVertex(new Point3D( 10,-10, 10));
    outerLoop4->addVertex(new Point3D( -10,-10,10));
    std::string southName = "south";
    Face * south = new Face(&southName);
    south->setPolygon(p4);
    south->setMaterial(material);
    model -> addObjectToLayer(&layerName,south);
    
    // East
    Polygon3D * p5 = new Polygon3D();
    Loop * outerLoop5 = p5->getOuterLoopRef();
    outerLoop5->addVertex(new Point3D(-10,-10,-10));
    outerLoop5->addVertex(new Point3D(-10, 10,-10));
    outerLoop5->addVertex(new Point3D(-10, 10, 10));
    outerLoop5->addVertex(new Point3D(-10,-10, 10));
    std::string eastName = "east";
    Face * east = new Face(&eastName);
    east->setPolygon(p5);
    east->setMaterial(material);
    model -> addObjectToLayer(&layerName,east);
    
    // West
    Polygon3D * p6 = new Polygon3D();
    Loop * outerLoop6 = p6->getOuterLoopRef();
    outerLoop6->addVertex(new Point3D(10,-10,-10));
    outerLoop6->addVertex(new Point3D(10, 10,-10));
    outerLoop6->addVertex(new Point3D(10, 10, 10));
    outerLoop6->addVertex(new Point3D(10,-10, 10));
    std::string westName = "west";
    Face * west = new Face(&westName);
    west->setPolygon(p6);
    west->setMaterial(material);
    model -> addObjectToLayer(&layerName,west);

    /* SET LOCATION */
    double latitude = -33.38;
    double longitude = 70.78;
    int time_zone = -4;
    
    // 0. Month, 1. Day, 2. Hour, 3. Direct Normal, 4. Diffuse horizontal,
    float wea[48][5] = {
        {1,1,0.5,0,0},
        {1,1,1.5,0,0},
        {1,1,2.5,0,0},
        {1,1,3.5,0,0},
        {1,1,4.5,0,0},
        {1,1,5.5,0,3},
        {1,1,6.5,44,61},
        {1,1,7.5,309,155},
        {1,1,8.5,600,175},
        {1,1,9.5,756,189},
        {1,1,10.5,871,173},
        {1,1,11.5,965,131},
        {1,1,12.5,968,143},
        {1,1,13.5,959,144},
        {1,1,14.5,906,167},
        {1,1,15.5,867,144},
        {1,1,16.5,738,155},
        {1,1,17.5,537,143},
        {1,1,18.5,175,117},
        {1,1,19.5,0,18},
        {1,1,20.5,0,0},
        {1,1,21.5,0,0},
        {1,1,22.5,0,0},
        {1,1,23.5,0,0},
        {1,2,0.5,0,0},
        {1,2,1.5,0,0},
        {1,2,2.5,0,0},
        {1,2,3.5,0,0},
        {1,2,4.5,0,0},
        {1,2,5.5,0,2},
        {1,2,6.5,41,60},
        {1,2,7.5,304,155},
        {1,2,8.5,605,171},
        {1,2,9.5,793,159},
        {1,2,10.5,861,178},
        {1,2,11.5,940,147},
        {1,2,12.5,946,158},
        {1,2,13.5,940,158},
        {1,2,14.5,931,140},
        {1,2,15.5,859,148},
        {1,2,16.5,754,143},
        {1,2,17.5,537,144},
        {1,2,18.5,209,108},
        {1,2,19.5,0,18},
        {1,2,20.5,0,0},
        {1,2,21.5,0,0},
        {1,2,22.5,0,0},
        {1,2,23.5,0,0}
    };
    
    Location * location = model->getLocation();
    location->setLatitude(latitude);
    location->setLongitude(longitude);
    location->setTimeZone(time_zone);
    location->markWeatherAsFilled();
    
    // Add some weather
    for(int i=0; i<48; i++){
        HourlyData h = HourlyData();
        h.month = wea[i][0];
        h.day = wea[i][1];
        h.hour = wea[i][2];
        h.direct_normal = wea[i][3];
        h.diffuse_horizontal=wea[i][4];
        location->addHourlyData(h);
    }
    
    /* ADD WORKPLANE */
    std::string wpName = "WP";
    
    // Interior polygon
    float side = 18.0;
    float innerArea = side*side;
    Polygon3D * interior = new Polygon3D();
    Loop * iLoop = interior -> getOuterLoopRef();
    iLoop->addVertex(new Point3D(-side/2.0,-side/2.0, 0));
    iLoop->addVertex(new Point3D( side/2.0,-side/2.0, 0));
    iLoop->addVertex(new Point3D( side/2.0, side/2.0, 0));
    iLoop->addVertex(new Point3D(-side/2.0, side/2.0, 0));
    model->addPolygonToWorkplane(&wpName, interior);
    interior->setNormal();
    
    // Exterior polygon
        
    if(percent > 0.01){
        float outerArea = percent * innerArea/(1-percent);
        side = sqrt(outerArea);
        Polygon3D * exterior = new Polygon3D();
        Loop * eLoop = exterior -> getOuterLoopRef();
        eLoop->addVertex(new Point3D(-side/2.0,-side/2.0, 20));
        eLoop->addVertex(new Point3D( side/2.0,-side/2.0, 20));
        eLoop->addVertex(new Point3D( side/2.0, side/2.0, 20));
        eLoop->addVertex(new Point3D(-side/2.0, side/2.0, 20));
        model->addPolygonToWorkplane(&wpName, exterior);
        exterior->setNormal();
    }
    
}

#define CREATE_MODEL(x) \
GroundhogModel model = GroundhogModel(); \
setOuterPercentage(&model,x); \
Workplane * workplane = model.getWorkplaneRef(0);

