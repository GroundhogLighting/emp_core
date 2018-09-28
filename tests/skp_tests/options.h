#pragma once

#include "./common.h"


TEST(SKPRead, options)
{
    FOR_ALL_VERSIONS {
        ASSERT_LOAD_MODEL("options",version);
        
        RTraceOptions * options = model.getRTraceOptions();
        
        ASSERT_EQ(options->getOption<double>("aa"), 0.2);
        ASSERT_EQ(options->getOption<int>("ab"), 9);
        ASSERT_EQ(options->getOption<double>("dj"), 1.0);
        ASSERT_EQ(options->getOption<double>("lw"), 0.00003);
    }
}
