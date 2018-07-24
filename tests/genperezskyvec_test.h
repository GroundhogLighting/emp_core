
#include "../include/emp_core.h"
//#include "calculations/radiance.h"
//#include "calculations/reinhart.h"


void radGenDayMtx(int month, int day, float hour, float direct, float diffuse, float albedo, float latitude, float longitude, float standardMeridian, int skyMF, bool sunOnly, bool sharpSun, float rotation, ColorMatrix * skyVec)
{
    // Write WEA file
    std::string weafilename = "test.wea";
    std::ofstream weafile;
    weafile.open(weafilename);
    weafile << "place SANTIAGO_CHL" << std::endl;
    weafile << "latitude " << latitude << std::endl;
    weafile << "longitude " << longitude << std::endl;
    weafile << "time_zone " << standardMeridian << std::endl;
    weafile << "site_elevation " << 0 << std::endl;
    weafile << "weather_data_file_units 1" << std::endl;
    weafile << month << " " << day << " " << hour << " " << direct << " " << diffuse << std::endl;
    weafile.close();
    
    std::string dMode = sunOnly ? " -d " : "";
    std::string sharpSunMode = sharpSun ? " -5 0.533 " : "";
    
    // Calc gendaymtx
    std::string command = "gendaymtx -h -m " + std::to_string(skyMF) + " -g " + std::to_string(albedo) + " " + std::to_string(albedo) + " " + std::to_string(albedo) + " -c 1 1 1 -r " + std::to_string(rotation) + sharpSunMode + dMode +  " " + weafilename;
    
    std::cout << command << std::endl;
    
    FILE * results = POPEN(&command[0],"r");
        
    // Read results
    if (results)
    {
        Matrix * red = skyVec->r();
        Matrix * green = skyVec->g();
        Matrix * blue = skyVec->b();
        
        float r;
        float g;
        float b;
        
        size_t i = 0;
        while (FSCANF(results, "%f %f %f", &r, &g, &b) != EOF)
        {
            std::cout << r << "," << g << "," << b << std::endl;
            red->setElement(i,0,r);
            green->setElement(i,0,g);
            blue->setElement(i,0,b);
            i++;
        }
    }
    else {
        FATAL(err,"Unable to read results from command");
        return;
    }
    
    // Remove resulting file
    remove(&weafilename[0]);
}


TEST(GenPerezSkyVec, NoRadiation)
{
    // Lets do this several times
    for(int i=0;i<100;i++){
        int month = (rand() % 12) + 1;
        int day = (rand() % 28) + 1;
        float hour = 12.0 + ((rand() % 8) - 4.0);
        
        float direct = 0;
        float diffuse = 0;
        float albedo = (rand() % 100) / 100.0;
        
        int latitude = (rand() % 180) - 90;
        int longitude = (rand() % 360) - 180;
        int meridian = longitude + ((rand() % 6) - 3);
        
        int rotation = rand()%360;
        int mf = (rand() % 5) + 1;
        size_t nbins = nReinhartBins(mf);
        
        // Create the SkyVector
        ColorMatrix SkyVec = ColorMatrix(nbins,1);
        
        // Calculate
        genPerezSkyVector(month, day, hour, direct, diffuse, albedo, latitude, longitude, meridian, mf, false, false, rotation, &SkyVec);
        
        // Iterate the matrix
        const Matrix * red = SkyVec.redChannel();
        const Matrix * green = SkyVec.greenChannel();
        const Matrix * blue = SkyVec.blueChannel();
        
        
        for(size_t row=0; row < nbins; row++){
            ASSERT_EQ(red->getElement(row,0),0.0);
            ASSERT_EQ(green->getElement(row,0),0.0);
            ASSERT_EQ(blue->getElement(row,0),0.0);
        }
    }
}




TEST(GenPerezSkyVec, DirectOnlySharpSun)
{
    
    
    bool directOnly = true;
    bool sharpSun = true;
    
    // Lets do this several times
    for(int i=0;i<100;i++){
        int month = (rand() % 12) + 1;
        int day = (rand() % 28) + 1;
        float hour = 12.0 + ((rand() % 8) - 4.0);
        
        float direct = (rand() % 700);
        float diffuse = (rand() % 500);
        float albedo = (rand() % 100) / 100.0;
        
        int latitude = (rand() % 180) - 90;
        int longitude = (rand() % 360) - 180;
        int meridian = longitude + ((rand() % 6) - 3);
        
        int rotation = rand()%360;
        
        int mf = (rand() % 5) + 1;
        size_t nbins = nReinhartBins(mf);
        
        // Create the SkyVector
        ColorMatrix skyVec = ColorMatrix(nbins,1);
        ColorMatrix referenceSkyVec = ColorMatrix(nbins,1);
        
        // Calculate
        int patch = genPerezSkyVector(month, day, hour, direct, diffuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, rotation, &skyVec);
        radGenDayMtx(month, day, hour, direct, diffuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, rotation, &referenceSkyVec);
        
        // Iterate the matrix
        Matrix * red = skyVec.r();
        Matrix * green = skyVec.g();
        Matrix * blue = skyVec.b();
        
        const Matrix * referenceRed = referenceSkyVec.redChannel();
        const Matrix * referenceGreen = referenceSkyVec.greenChannel();
        const Matrix * referenceBlue = referenceSkyVec.blueChannel();
        
        
        for(size_t row=0; row < nbins; row++){

            
            // Due to print and read, try this...
            if( red->getElement(row,0) > 100 ){
                red->setElement(row,0,round(red->getElement(row,0)));
            }
            if( green->getElement(row,0) > 100 ){
                green->setElement(row,0,round(green->getElement(row,0)));
            }
            if( blue->getElement(row,0) > 100 ){
                blue->setElement(row,0, round(blue->getElement(row,0)));
            }
            
            // All rows, except PATCH, should be zero...
            // unless PATCH is -1, in which case all should be zero
            if (patch < 0 || (patch >= 0 && row != patch) ){
                ASSERT_EQ( 0, red->getElement(row,0) );
                ASSERT_EQ( 0, green->getElement(row,0) );
                ASSERT_EQ( 0, blue->getElement(row,0) );
            }
            
            // Less than 1% error
            ASSERT_NEAR(red->  getElement(row,0), referenceRed->getElement(row,0),0.02*referenceRed->getElement(row,0));
            ASSERT_NEAR(green->getElement(row,0), referenceGreen->getElement(row,0),0.02*referenceGreen->getElement(row,0));
            ASSERT_NEAR(blue-> getElement(row,0), referenceBlue ->getElement(row,0),0.02*referenceBlue ->getElement(row,0));
            
        }
    }
    
    
}



TEST(GenPerezSkyVec, DirectOnlyWideSun)
{
    
    
    bool directOnly = true;
    bool sharpSun = false;
    
    // Lets do this several times
    for(int i=0;i<100;i++){
        std::cout << i << std::endl;
        int month = (rand() % 12) + 1;
        int day = (rand() % 28) + 1;
        float hour = 12.0 + ((rand() % 8) - 4.0);
        
        float direct = (rand() % 700);
        float diffuse = (rand() % 500);
        float albedo = (rand() % 100) / 100.0;
        
        int latitude = (rand() % 180) - 90;
        int longitude = (rand() % 360) - 180;
        int meridian = longitude + ((rand() % 6) - 3);
        
        int rotation = rand()%360;
        
        int mf = (rand() % 4) + 1;
        size_t nbins = nReinhartBins(mf);
        
        // Create the SkyVector
        ColorMatrix skyVec = ColorMatrix(nbins,1);
        ColorMatrix referenceSkyVec = ColorMatrix(nbins,1);
        
        // Calculate
        genPerezSkyVector(month, day, hour, direct, diffuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, rotation, &skyVec);
        radGenDayMtx(month, day, hour, direct, diffuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, rotation, &referenceSkyVec);
        
        // Iterate the matrix
        Matrix * red = skyVec.r();
        Matrix * green = skyVec.g();
        Matrix * blue = skyVec.b();
        
        const Matrix * referenceRed = referenceSkyVec.redChannel();
        const Matrix * referenceGreen = referenceSkyVec.greenChannel();
        const Matrix * referenceBlue = referenceSkyVec.blueChannel();
        
        red->print();
        referenceRed->print();
        
        
        for(size_t row=0; row < nbins; row++){
            
            // Due to print and read, try this...
            if( red->getElement(row,0) > 100 ){
                red->setElement(row,0, round(red->getElement(row,0)));
            }
            if( green->getElement(row,0) > 100 ){
                green->setElement(row,0, round(green->getElement(row,0)));
            }
            if( blue->getElement(row,0) > 100 ){
                blue->setElement(row,0, round(blue->getElement(row,0)));
            }
            
            
            // Less than 1% error
            ASSERT_NEAR(red  ->getElement(row,0), referenceRed  ->getElement(row,0),0.02* referenceRed  ->getElement(row,0));
            ASSERT_NEAR(green->getElement(row,0), referenceGreen->getElement(row,0),0.02* referenceGreen->getElement(row,0));
            ASSERT_NEAR(blue ->getElement(row,0), referenceBlue ->getElement(row,0),0.02* referenceBlue ->getElement(row,0));
            
        }
    }
    
    
}


TEST(GenPerezSkyVec, FullSkyWideSun)
{
    
    
    bool directOnly = false;
    bool sharpSun = false;
    
    // Lets do this several times
    for(int i=0;i<100;i++){
        std::cout << i << std::endl;
        int month = (rand() % 12) + 1;
        int day = (rand() % 28) + 1;
        float hour = 12.0 + ((rand() % 8) - 4.0);
        
        float direct = (rand() % 700);
        float diffuse = (rand() % 500);
        float albedo = (rand() % 100) / 100.0;
        
        int latitude = (rand() % 180) - 90;
        int longitude = (rand() % 360) - 180;
        int meridian = longitude + ((rand() % 6) - 3);
        
        
        int mf = (rand() % 5) + 1;
        size_t nbins = nReinhartBins(mf);
        
        // Create the SkyVector
        ColorMatrix skyVec = ColorMatrix(nbins,1);
        ColorMatrix referenceSkyVec = ColorMatrix(nbins,1);
        
        // Calculate
        genPerezSkyVector(month, day, hour, direct, diffuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, 0, &skyVec);
        radGenDayMtx(month, day, hour, direct, diffuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, 0, &referenceSkyVec);
        
        // Iterate the matrix
        Matrix * red = skyVec.r();
        Matrix * green = skyVec.g();
        Matrix * blue = skyVec.b();
        
        const Matrix * referenceRed = referenceSkyVec.redChannel();
        const Matrix * referenceGreen = referenceSkyVec.greenChannel();
        const Matrix * referenceBlue = referenceSkyVec.blueChannel();
        
        
        for(size_t row=0; row < nbins; row++){
            
            // Due to print and read, try this...
            
            if( red->getElement(row,0) > 100 ){
                red->setElement(row,0, round(red->getElement(row,0)));
            }
            if( green->getElement(row,0)> 100 ){
                green->setElement(row,0,round(green->getElement(row,0)));
            }
            if( blue->getElement(row,0) > 100 ){
                blue->setElement(row,0, round(blue->getElement(row,0)));
            }
            
            // Less than 1% error
            ASSERT_NEAR(red  ->getElement(row,0),referenceRed  ->getElement(row,0),0.02* referenceRed  ->getElement(row,0));
            ASSERT_NEAR(green->getElement(row,0),referenceGreen->getElement(row,0),0.02* referenceGreen->getElement(row,0));
            ASSERT_NEAR(blue ->getElement(row,0),referenceBlue ->getElement(row,0),0.02* referenceBlue ->getElement(row,0));
            
        }
    }
}


TEST(GenPerezSkyVec, FullSkySharpSun)
{
    
    bool directOnly = false;
    bool sharpSun = true;
    
    // Lets do this several times
    for(int i=0;i<100;i++){
        int month = (rand() % 12) + 1;
        int day = (rand() % 28) + 1;
        float hour = 12.0 + ((rand() % 8) - 4.0);
        
        float direct = (rand() % 700);
        float diffuse = (rand() % 500);
        float albedo = (rand() % 100) / 100.0;
        
        int latitude = (rand() % 180) - 90;
        int longitude = (rand() % 360) - 180;
        int meridian = longitude + ((rand() % 6) - 3);
        
        int rotation = rand()%360;
        
        int mf = (rand() % 5) + 1;
        size_t nbins = nReinhartBins(mf);
        
        // Create the SkyVector
        ColorMatrix skyVec = ColorMatrix(nbins,1);
        ColorMatrix referenceSkyVec = ColorMatrix(nbins,1);
        
        // Calculate
        genPerezSkyVector(month, day, hour, direct, diffuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, rotation, &skyVec);
        radGenDayMtx(month, day, hour, direct, diffuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, rotation, &referenceSkyVec);
        
        // Iterate the matrix
        Matrix * red = skyVec.r();
        Matrix * green = skyVec.g();
        Matrix * blue = skyVec.b();
        
        const Matrix * referenceRed = referenceSkyVec.redChannel();
        const Matrix * referenceGreen = referenceSkyVec.greenChannel();
        const Matrix * referenceBlue = referenceSkyVec.blueChannel();
        
        
        for(size_t row=0; row < nbins; row++){
            
            // Due to print and read, try this...
            
            if( red->getElement(row,0) > 100 ){
                red->setElement(row,0,round(red->getElement(row,0)));
            }
            if( green->getElement(row,0) > 100 ){
                green->setElement(row,0, round(green->getElement(row,0)));
            }
            if( blue->getElement(row,0) > 100 ){
                blue->setElement(row,0,round(blue->getElement(row,0)));
            }
            
            // Less than 1% error
            ASSERT_NEAR(red  ->getElement(row,0), referenceRed  ->getElement(row,0),0.02* referenceRed  ->getElement(row,0));
            ASSERT_NEAR(green->getElement(row,0), referenceGreen->getElement(row,0),0.02* referenceGreen->getElement(row,0));
            ASSERT_NEAR(blue ->getElement(row,0), referenceBlue ->getElement(row,0),0.02* referenceBlue ->getElement(row,0));
            
        }
    }
}
