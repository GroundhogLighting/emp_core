#pragma once

#include "./common.h"


TEST(SKPRead, location)
{
    FOR_ALL_VERSIONS {
        ASSERT_LOAD_MODEL("location",version);
        
        Location * loc = model.getLocation();
        ASSERT_EQ(loc->getCountry(),"New Zealand");
        ASSERT_EQ(loc->getCity(),"Wellington");
        ASSERT_EQ(loc->getLatitude(),-41.1f);
        ASSERT_EQ(loc->getLongitude(),174.1f);
        ASSERT_EQ(loc->getAlbedo(),0.2f);
        ASSERT_EQ(loc->getTimeZone(),12.0f);
    }
}


TEST(SKPRead, weather_file)
{
    FOR_ALL_VERSIONS {
        ASSERT_LOAD_MODEL("weather_file",version);
        
        Location * loc = model.getLocation();
        ASSERT_EQ(loc->getCountry(),"CHL");
        ASSERT_EQ(loc->getCity(),"SANTIAGO");
        ASSERT_EQ(loc->getLatitude(),-33.38);
        ASSERT_EQ(loc->getLongitude(),-70.78);
        ASSERT_EQ(loc->getAlbedo(),0.17);
        ASSERT_EQ(loc->getTimeZone(),-4);
    }
}
