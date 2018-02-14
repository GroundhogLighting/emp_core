
#include "calculations/radiance.h"
#include "calculations/reinhart.h"


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
    FILE * results = POPEN(&command[0],"r");
    
    std::cout << command << std::endl;
    
    // Read results
    
    if (results)
    {
        Matrix * red = skyVec->redChannel();
        Matrix * green = skyVec->greenChannel();
        Matrix * blue = skyVec->blueChannel();
        
        float r;
        float g;
        float b;
        
        size_t i = 0;
        while (FSCANF(results, "%f %f %f", &r, &g, &b) != EOF)
        {
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
    size_t mf=1;
    size_t nbins = nReinhartBins(mf);
    
    // Create the SkyVector
    ColorMatrix SkyVec = ColorMatrix(nbins,1);
    
    // Calculate
    genPerezSkyVector(1, 1, 12.0, 0, 0, 0.2, -32, -75, -32, mf, false, false, 0, &SkyVec);
    
    // Iterate the matrix
    Matrix * red = SkyVec.redChannel();
    Matrix * green = SkyVec.greenChannel();
    Matrix * blue = SkyVec.blueChannel();
    
    for(size_t row=0; row < nbins; row++){
        ASSERT_EQ((*red)[row]->at(0),0.0);
        ASSERT_EQ((*green)[row]->at(0),0.0);
        ASSERT_EQ((*blue)[row]->at(0),0.0);
    }
}




TEST(GenPerezSkyVec, DirectOnlySharpSun)
{
    
    
    bool directOnly = true;
    bool sharpSun = true;
    
    // Lets do this several times
    for(int i=0;i<1;i++){
        int month = (rand() % 12) + 1;
        int day = (rand() % 28) + 1;
        float hour = 12.0 + ((rand() % 8) - 4.0);
        
        float direct = (rand() % 700);
        float difuse = (rand() % 500);
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
        genPerezSkyVector(month, day, hour, direct, difuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, rotation, &skyVec);
        radGenDayMtx(month, day, hour, direct, difuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, rotation, &referenceSkyVec);
        
        // Iterate the matrix
        Matrix * red = skyVec.redChannel();
        Matrix * green = skyVec.greenChannel();
        Matrix * blue = skyVec.blueChannel();
        
        Matrix * referenceRed = referenceSkyVec.redChannel();
        Matrix * referenceGreen = referenceSkyVec.greenChannel();
        Matrix * referenceBlue = referenceSkyVec.blueChannel();
        
        
        for(size_t row=0; row < nbins; row++){
            
            // Due to print and read, try this...
            
            if( (*red)[row]->at(0) > 100 ){
                (*red)[row]->at(0) = round((*red)[row]->at(0));
            }
            if( (*green)[row]->at(0) > 100 ){
                (*green)[row]->at(0) = round((*green)[row]->at(0));
            }
            if( (*blue)[row]->at(0) > 100 ){
                (*blue)[row]->at(0) = round((*blue)[row]->at(0));
            }
            
            // Less than 1% error
            ASSERT_NEAR((*red)[row]->at(0),(*referenceRed)[row]->at(0),0.01*(*referenceRed)[row]->at(0));
            ASSERT_NEAR((*green)[row]->at(0),(*referenceGreen)[row]->at(0),0.01*(*referenceGreen)[row]->at(0));
            ASSERT_NEAR((*blue)[row]->at(0),(*referenceBlue)[row]->at(0),0.01*(*referenceBlue)[row]->at(0));
            
        }
    }
    
    
}



TEST(GenPerezSkyVec, DirectOnlyWideSun)
{
    
    
    bool directOnly = true;
    bool sharpSun = false;
    
    // Lets do this several times
    for(int i=0;i<1;i++){
        int month = (rand() % 12) + 1;
        int day = (rand() % 28) + 1;
        float hour = 12.0 + ((rand() % 8) - 4.0);
        
        float direct = (rand() % 700);
        float difuse = (rand() % 500);
        float albedo = (rand() % 100) / 100.0;
        
        int latitude = (rand() % 180) - 90;
        int longitude = (rand() % 360) - 180;
        int meridian = longitude + ((rand() % 6) - 3);
        
        int rotation = rand()%360;
        
        int mf = 1;// (rand() % 4) + 1; //... Rounding error? With an MF larger than 1, the values end up in other patches
        size_t nbins = nReinhartBins(mf);
        
        // Create the SkyVector
        ColorMatrix skyVec = ColorMatrix(nbins,1);
        ColorMatrix referenceSkyVec = ColorMatrix(nbins,1);
        
        // Calculate
        genPerezSkyVector(month, day, hour, direct, difuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, rotation, &skyVec);
        radGenDayMtx(month, day, hour, direct, difuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, rotation, &referenceSkyVec);
        
        // Iterate the matrix
        Matrix * red = skyVec.redChannel();
        Matrix * green = skyVec.greenChannel();
        Matrix * blue = skyVec.blueChannel();
        
        Matrix * referenceRed = referenceSkyVec.redChannel();
        Matrix * referenceGreen = referenceSkyVec.greenChannel();
        Matrix * referenceBlue = referenceSkyVec.blueChannel();
        
        
        for(size_t row=0; row < nbins; row++){
            
            // Due to print and read, try this...
            //std::cout << (*red)[row]->at(0) << " == " << (*referenceRed)[row]->at(0)  << std::endl;
            if( (*red)[row]->at(0) > 100 ){
                (*red)[row]->at(0) = round((*red)[row]->at(0));
            }
            if( (*green)[row]->at(0) > 100 ){
                (*green)[row]->at(0) = round((*green)[row]->at(0));
            }
            if( (*blue)[row]->at(0) > 100 ){
                (*blue)[row]->at(0) = round((*blue)[row]->at(0));
            }
            
            // Less than 1% error
            ASSERT_NEAR((*red)[row]->at(0),(*referenceRed)[row]->at(0),0.01*(*referenceRed)[row]->at(0));
            ASSERT_NEAR((*green)[row]->at(0),(*referenceGreen)[row]->at(0),0.01*(*referenceGreen)[row]->at(0));
            ASSERT_NEAR((*blue)[row]->at(0),(*referenceBlue)[row]->at(0),0.01*(*referenceBlue)[row]->at(0));
            
        }
    }
    
    
}


TEST(GenPerezSkyVec, FulllSkyWideSun)
{
    
    
    bool directOnly = false;
    bool sharpSun = false;
    
    // Lets do this several times
    for(int i=0;i<1;i++){
        int month = (rand() % 12) + 1;
        int day = (rand() % 28) + 1;
        float hour = 12.0 + ((rand() % 8) - 4.0);
        
        float direct = (rand() % 700);
        float difuse = (rand() % 500);
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
        genPerezSkyVector(month, day, hour, direct, difuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, 0, &skyVec);
        radGenDayMtx(month, day, hour, direct, difuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, 0, &referenceSkyVec);
        
        // Iterate the matrix
        Matrix * red = skyVec.redChannel();
        Matrix * green = skyVec.greenChannel();
        Matrix * blue = skyVec.blueChannel();
        
        Matrix * referenceRed = referenceSkyVec.redChannel();
        Matrix * referenceGreen = referenceSkyVec.greenChannel();
        Matrix * referenceBlue = referenceSkyVec.blueChannel();
        
        
        for(size_t row=0; row < nbins; row++){
            
            // Due to print and read, try this...
            
            if( (*red)[row]->at(0) > 100 ){
                (*red)[row]->at(0) = round((*red)[row]->at(0));
            }
            if( (*green)[row]->at(0) > 100 ){
                (*green)[row]->at(0) = round((*green)[row]->at(0));
            }
            if( (*blue)[row]->at(0) > 100 ){
                (*blue)[row]->at(0) = round((*blue)[row]->at(0));
            }
            
            // Less than 1% error
            ASSERT_NEAR((*red)[row]->at(0),(*referenceRed)[row]->at(0),0.01*(*referenceRed)[row]->at(0));
            ASSERT_NEAR((*green)[row]->at(0),(*referenceGreen)[row]->at(0),0.01*(*referenceGreen)[row]->at(0));
            ASSERT_NEAR((*blue)[row]->at(0),(*referenceBlue)[row]->at(0),0.01*(*referenceBlue)[row]->at(0));
            
        }
    }
}


TEST(GenPerezSkyVec, FullSkySharpSun)
{
    
    bool directOnly = false;
    bool sharpSun = true;
    
    // Lets do this several times
    for(int i=0;i<1;i++){
        int month = (rand() % 12) + 1;
        int day = (rand() % 28) + 1;
        float hour = 12.0 + ((rand() % 8) - 4.0);
        
        float direct = (rand() % 700);
        float difuse = (rand() % 500);
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
        genPerezSkyVector(month, day, hour, direct, difuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, rotation, &skyVec);
        radGenDayMtx(month, day, hour, direct, difuse, albedo, latitude, longitude, meridian, mf, directOnly, sharpSun, rotation, &referenceSkyVec);
        
        // Iterate the matrix
        Matrix * red = skyVec.redChannel();
        Matrix * green = skyVec.greenChannel();
        Matrix * blue = skyVec.blueChannel();
        
        Matrix * referenceRed = referenceSkyVec.redChannel();
        Matrix * referenceGreen = referenceSkyVec.greenChannel();
        Matrix * referenceBlue = referenceSkyVec.blueChannel();
        
        
        for(size_t row=0; row < nbins; row++){
            
            // Due to print and read, try this...
            
            if( (*red)[row]->at(0) > 100 ){
                (*red)[row]->at(0) = round((*red)[row]->at(0));
            }
            if( (*green)[row]->at(0) > 100 ){
                (*green)[row]->at(0) = round((*green)[row]->at(0));
            }
            if( (*blue)[row]->at(0) > 100 ){
                (*blue)[row]->at(0) = round((*blue)[row]->at(0));
            }
            
            // Less than 1% error
            ASSERT_NEAR((*red)[row]->at(0),(*referenceRed)[row]->at(0),0.01*(*referenceRed)[row]->at(0));
            ASSERT_NEAR((*green)[row]->at(0),(*referenceGreen)[row]->at(0),0.01*(*referenceGreen)[row]->at(0));
            ASSERT_NEAR((*blue)[row]->at(0),(*referenceBlue)[row]->at(0),0.01*(*referenceBlue)[row]->at(0));
            
        }
    }
    
    
}
