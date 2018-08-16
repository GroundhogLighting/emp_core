
#include "../include/emp_core.h"

// Santiago weather, but located in 70.78E instead of W, and in time zone GMT+4 instead of -4
const float latitude = -33.38f;
const float longitude = 70.78f;
const float time_zone = -4.0f;

int skyMF = 1;
int sunMF = 6;

// 0. Month, 1. Day, 2. Hour, 3. Direct Normal, 4. Diffuse horizontal,
const float wea[48][5] = {
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


// 0. Global DDC, 1. Direct Patch, 2. Direct sharp sun, 3. RTrace value
const double emptyReference[48][4] = {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {1.986573,0,0,2.05218},
    {41.38419,2.619209,2.807025,40.9866},
    {156.5315,49.06445,49.87806,158.55},
    {302.2271,170.733,170.4271,302.893},
    {434.7068,291.3482,296.918,441.171},
    {537.5763,415.2368,421.7068,543.913},
    {612.8468,519.7388,526.9948,617.689},
    {640.804,541.0325,549.7664,647.132},
    {621.3898,520.364,535.1064,637.423},
    {579.7381,463.3158,468.8085,586.408},
    {482.2672,376.5324,390.4463,499.397},
    {391.0479,269.1377,268.4266,383.613},
    {228.7684,118.7692,131.0732,236.553},
    {100.6313,22.76053,20.89805,97.6037},
    {11.38123,0,0,11.4239},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {1.364446,0,0,1.40612},
    {40.56894,2.398103,2.603289,40.1267},
    {155.3983,47.99317,48.94138,157.269},
    {300.6089,171.5687,171.5338,301.169},
    {432.1906,307.0813,313.8433,439.285},
    {529.4937,403.8172,416.5148,541.056},
    {609.4606,505.0173,512.873,613.768},
    {638.0722,527.9791,537.0743,643.55},
    {619.7086,509.3056,524.6013,635.404},
    {579.8121,479.1306,484.7419,586.049},
    {480.8931,372.9667,386.1732,498.177},
    {390.5045,276.1482,275.1485,383.08},
    {229.3761,119.0588,131.0732,237.521},
    {102.6244,26.10413,24.07007,99.267},
    {11.39055,0,0,11.4238},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

// 0. Global DDC, 1. Direct Patch, 2. Direct sharp sun, 3. RTrace value
const double simpleReference[48][5] = {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {1.781778,0,0,1.835162011},
    {40.25919,2.761479,2.831319,39.49558659},
    {153.8296,50.28972,50.20926,155.2877095},
    {299.0263,167.8832,169.8229,300.9905028},
    {354.1573,213.81,0,135.7385475},
    {155.9966,42.53947,0,112.9435754},
    {125.5203,34.27351,0,90.78212291},
    {306.9643,204.7589,0,100.3441341},
    {604.6513,489.4207,534.9597,650.8156425},
    {595.106,460.7691,469.705,602.0223464},
    {509.6289,387.7278,390.3772,515.7804469},
    {394.4555,260.093,267.7437,394.7949721},
    {238.0435,121.8459,130.9517,241.6050279},
    {98.28493,21.62625,20.98299,96.6452514},
    {10.34289,0,0,10.35547486},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {1.222316,0,0,1.259011173},
    {39.45731,2.529635,2.62582,38.68368715},
    {152.5968,49.16741,49.26636,153.9357542},
    {297.8191,168.7178,170.9256,299.6027933},
    {353.1079,226.7017,0,121.4184358},
    {201.0061,85.17037,0,115.1592179},
    {131.6168,32.86225,0,97.74469274},
    {307.4785,198.0175,0,107.2608939},
    {600.5533,477.5821,524.4575,647.3631285},
    {597.004,476.2529,485.6688,603.3519553},
    {507.9969,384.1958,386.105,514.1988827},
    {394.3977,266.8553,274.4485,394.9927374},
    {238.6112,122.1291,130.9517,242.4212291},
    {100.6708,24.80616,24.16791,98.83072626},
    {10.34988,0,0,10.33586592},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

std::vector<RAY> rays = std::vector<RAY>(1);





// Create options
RTraceOptions options = RTraceOptions();
options.setOption("ab",10);
options.setOption("ad",50000);
options.setOption("lw",0.000001);
options.setOption("aa",0.1);

// Create Models
EmpModel simpleModel = EmpModel();
EmpModel emptyModel = EmpModel();

// Fill simple model
std::string layerName = "Layer 1";
simpleModel.addLayer(&layerName);
Material * material = simpleModel.addDefaultMaterial();

Polygon3D * p = new Polygon3D();
Loop * outerLoop = p->getOuterLoopRef();
outerLoop->addVertex(new Point3D(0,0,1));
outerLoop->addVertex(new Point3D(1,0,1));
outerLoop->addVertex(new Point3D(1,1,1));
outerLoop->addVertex(new Point3D(0,1,1));
std::string faceName = "face";
Face * face = new Face(&faceName);
face->setPolygon(p);
face->setMaterial(material);
simpleModel.addObjectToLayer(&layerName,face);

p = new Polygon3D();
outerLoop = p->getOuterLoopRef();
outerLoop->addVertex(new Point3D(-10,-10,-0.1));
outerLoop->addVertex(new Point3D( 10,-10,-0.1));
outerLoop->addVertex(new Point3D( 10, 10,-0.1));
outerLoop->addVertex(new Point3D(-10, 10,-0.1));
faceName = "face";
face = new Face(&faceName);
face->setPolygon(p);
face->setMaterial(material);
simpleModel.addObjectToLayer(&layerName,face);

// Set location: Santiago, Chile
Location * emptyModelLocation = emptyModel.getLocation();
emptyModelLocation->setLatitude(latitude);
emptyModelLocation->setLongitude(longitude);
emptyModelLocation->setTimeZone(time_zone);
emptyModelLocation->markWeatherAsFilled();

Location * simpleModelLocation = simpleModel.getLocation();
simpleModelLocation->setLatitude(latitude);
simpleModelLocation->setLongitude(longitude);
simpleModelLocation->setTimeZone(time_zone);
simpleModelLocation->markWeatherAsFilled();

// Add some weather
for(int i=0; i<48; i++){
    HourlyData h = HourlyData();
    h.month = (int)wea[i][0];
    h.day = (int)wea[i][1];
    h.hour = (float)wea[i][2];
    h.direct_normal = (float)wea[i][3];
    h.diffuse_horizontal= (float)wea[i][4];
    emptyModelLocation->addHourlyData(h);
    simpleModelLocation->addHourlyData(h);
}

// Create rays... at the origin, pointing up
FVECT origin = {0,0,0};
FVECT dir = {0,0,1};
VCOPY(rays.at(0).rorg, origin);
VCOPY(rays.at(0).rdir, dir);




