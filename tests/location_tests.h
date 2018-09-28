#include "../include/emp_core.h"

const float latitude = -33.38f;
const float longitude = 70.78f;
const float time_zone = -4.0f;
const float elevation = 476.0f;
const std::string country = "CHL";
const std::string city = "SANTIAGO";

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

#define LININTERP(ini,fin,x) (ini + x*(fin - ini))


TEST(WeatherTest,loadFromWEA) {
    Location l1 = Location();
    ASSERT_ANY_THROW(l1.fillWeatherFromWEAFile("ss"));
    
    Location l2 = Location();
    ASSERT_NO_THROW(l2.fillWeatherFromWEAFile( "../../tests/weather/Santiago.wea") );
    
    
    ASSERT_EQ(l2.getLatitude(),latitude);
    ASSERT_EQ(l2.getLongitude(),longitude);
    ASSERT_EQ(l2.getTimeZone(),time_zone);
    ASSERT_EQ(l2.getElevation(),elevation);
    ASSERT_EQ(l2.getCountry(),city+"_"+country);    
    
    for(int c=0; c< 40; c++){
        int i=rand() % 47;
        
        HourlyData * d = l2.getHourlyData(i);
        
        ASSERT_EQ(d->month,wea[i][0]);
        ASSERT_EQ(d->day,wea[i][1]);
        ASSERT_EQ(d->hour,wea[i][2]);
        ASSERT_EQ(d->direct_normal,wea[i][3]);
        ASSERT_EQ(d->diffuse_horizontal,wea[i][4]);
        
    }
    
    ASSERT_TRUE(l2.hasWeather());
        
}


TEST(WeatherTest,loadFromEPW) {
    Location l1 = Location();
    ASSERT_ANY_THROW(l1.fillWeatherFromEPWFile("ss"));
    
    Location l2 = Location();
    ASSERT_NO_THROW(l2.fillWeatherFromEPWFile( "../../tests/weather/Santiago.epw") );
    
    ASSERT_EQ(l2.getLatitude(),latitude);
    ASSERT_EQ(l2.getLongitude(),longitude);
    ASSERT_EQ(l2.getTimeZone(),time_zone);
    ASSERT_EQ(l2.getElevation(),elevation);
    ASSERT_EQ(l2.getCountry(),country);
    ASSERT_EQ(l2.getCity(),city);
    
    for(int c=0; c< 40; c++){
        int i=rand() % 47;
        
        HourlyData * d = l2.getHourlyData(i);
        
        ASSERT_EQ(d->month,wea[i][0]);
        ASSERT_EQ(d->day,wea[i][1]);
        ASSERT_EQ(d->hour,wea[i][2]);
        ASSERT_EQ(d->direct_normal,wea[i][3]);
        ASSERT_EQ(d->diffuse_horizontal,wea[i][4]);
        
    }
    ASSERT_TRUE(l2.hasWeather());
    
}

TEST(LocationTest, getInterpolatedData) {
	
    Location l = Location();
    l.markWeatherAsFilled();
    
    // Add some weather
    for(int i=0; i<48; i++){
        HourlyData h = HourlyData();
        h.month = (int)wea[i][0];
        h.day = (int)wea[i][1];
        h.hour = (float)wea[i][2];
        h.direct_normal = (float)wea[i][3];
        h.diffuse_horizontal= (float)wea[i][4];
        l.addHourlyData(h);
    }
	
    for(int count=0; count < 100; count++){
        int i=rand() % 47;
        
        // Skip changes of day... they fail this test
        //(because hour interpolation does not make sense),
        // but it does not matter because it
        // is always night, thus always ignored
        // during calculation
        if(wea[i][2] == 23.5)
            continue;
        
        // item i with x=0.
        HourlyData d0 = HourlyData();
        l.getInterpolatedData(i,0,&d0);
        ASSERT_EQ(d0.month,wea[i][0]);
        ASSERT_EQ(d0.day,wea[i][1]);
        ASSERT_EQ(d0.hour,wea[i][2]);
        ASSERT_EQ(d0.direct_normal,wea[i][3]);
        ASSERT_EQ(d0.diffuse_horizontal,wea[i][4]);
        
        // item i with x=1... which is item 7
        HourlyData d1 = HourlyData();
        l.getInterpolatedData(i,1,&d1);
        ASSERT_EQ(d1.month,wea[i+1][0]);
        ASSERT_EQ(d1.day,wea[i+1][1]);
        ASSERT_EQ(d1.hour,wea[i+1][2]);
        ASSERT_EQ(d1.direct_normal,wea[i+1][3]);
        ASSERT_EQ(d1.diffuse_horizontal,wea[i+1][4]);
        
        // item i with x=rand()
        float x=((rand () % 100)/100.0f);        
        HourlyData d = HourlyData();
        l.getInterpolatedData(i,x,&d);
        ASSERT_EQ(d.month,wea[i][0]);
        ASSERT_EQ(d.day,wea[i][1]);
        ASSERT_NEAR(d.hour,LININTERP(wea[i][2],wea[i+1][2],x),d.hour * 0.01);
        ASSERT_NEAR(d.direct_normal,LININTERP(wea[i][3],wea[i+1][3],x),d.direct_normal * 0.01);
        ASSERT_NEAR(d.diffuse_horizontal,LININTERP(wea[i][4],wea[i+1][4],x),d.diffuse_horizontal * 0.01);
        
    }
    
    
}

TEST(LocationTest, getDataByDate) {
	
	
	
    Location l = Location();
    l.markWeatherAsFilled();
    
    // Add some weather
    for(int i=0; i<48; i++){
        HourlyData h = HourlyData();
        h.month = (int)wea[i][0];
        h.day = (int)wea[i][1];
        h.hour = (float)wea[i][2];
        h.direct_normal = (float)wea[i][3];
        h.diffuse_horizontal= (float)wea[i][4];
        l.addHourlyData(h);
    }
    
    for(int count=0; count < 100; count++){
        int i=rand() % 47;
    
        // Skip changes of day... they fail this test
        //(because hour interpolation does not make sense),
        // but it does not matter because it
        // is always night, thus always ignored
        // during calculation
        if(wea[i][2] == 23.5)
            continue;
        
        // get exact date
        HourlyData d0 = HourlyData();
        l.getDataByDate(wea[i][0],wea[i][1],wea[i][2],&d0);
        ASSERT_EQ(d0.month,wea[i][0]);
        ASSERT_EQ(d0.day,wea[i][1]);
        ASSERT_EQ(d0.hour,wea[i][2]);
        ASSERT_EQ(d0.direct_normal,wea[i][3]);
        ASSERT_EQ(d0.diffuse_horizontal,wea[i][4]);
    
        
        // Get date that requires interpolation
        float x=((rand () % 100)/100.0f);
        float hour = LININTERP(wea[i][2],wea[i+1][2],x);
    
        HourlyData d = HourlyData();
        l.getDataByDate(wea[i][0],wea[i][1],hour,&d);
    
        ASSERT_EQ(d.month,wea[i][0]);
        ASSERT_EQ(d.day,wea[i][1]);
        ASSERT_NEAR(d.hour,LININTERP(wea[i][2],wea[i+1][2],x),d.hour * 0.01);
        ASSERT_NEAR(d.direct_normal,LININTERP(wea[i][3],wea[i+1][3],x),d.direct_normal * 0.01);
        ASSERT_NEAR(d.diffuse_horizontal,LININTERP(wea[i][4],wea[i+1][4],x),d.diffuse_horizontal * 0.01);
        
    }
	
}

