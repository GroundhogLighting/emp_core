
//extern "C" {


/****   
    THIS FILE CONTAINS SECTIONS OF RADIANCE CODE, COPIED AND PASTED;
    OBTAINED FROM GENDAYMTX.C AND SUN.C

    GENDAYMTX.C ALSO CONTAINS CODE OBTAINED ELSEWHERE
****/

/* Zenith is along the Z-axis */
/* X-axis points east */
/* Y-axis points north */
/* azimuth is measured as degrees or radians east of North */

/* Include files */
#define	_USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rtmath.h"
#include "rtio.h"
#include "resolu.h"
#include "platform.h"
#include "color.h"
#include "resolu.h"

/* Degrees into radians */
#define DegToRad(deg)    ((deg)*(PI/180.))

/* Radiuans into degrees */
#define RadToDeg(rad)    ((rad)*(180./PI))

#define        vector(v,alt,azi)    (    (v)[1] = tcos(alt), \
(v)[0] = (v)[1]*tsin(azi), \
(v)[1] *= tcos(azi), \
(v)[2] = tsin(alt) )

#define        rh_vector(v,i)        vector(v,rh_palt[i],rh_pazi[i])
#define        rh_cos(i)        tsin(rh_palt[i])

class GenDayMtx {
    
public:
    const char * gendaymtxname = "genPerezSkyVec";
    char errmsg[128];                            /* Error message buffer */
    const double DC_SolarConstantE = 1367.0;    /* Solar constant W/m^2 */
    const double DC_SolarConstantL = 127.5;        /* Solar constant klux */
    
    double altitude;            /* Solar altitude (radians) */
    double azimuth;                /* Solar azimuth (radians) */
    double apwc;                /* Atmospheric precipitable water content */
    double dew_point = 11.0;        /* Surface dew point temperature (deg. C) */
    double diff_illum;            /* Diffuse illuminance */
    double diff_irrad;            /* Diffuse irradiance */
    double dir_illum;            /* Direct illuminance */
    double dir_irrad;            /* Direct irradiance */
    int julian_date;            /* Julian date */
    double perez_param[5];            /* Perez sky model parameters */
    double sky_brightness;            /* Sky brightness */
    double sky_clearness;            /* Sky clearness */
    double solar_rad;            /* Solar radiance */
    double sun_zenith;            /* Sun zenith angle (radians) */
    int    input = 1;                /* Input type */
    int    output = 0;                /* Output type... change to 1 to produce solar radiance */
    double  s_latitude = 0.66;    /* site latitude (radians) */
    double  s_longitude = 2.13;    /* site longitude (radians) */
    double  s_meridian = 2.0944;    /* standard meridian (radians) */



    
  


    /* Perez sky model coefficients */

    /* Reference:	Perez, R., R. Seals, and J. Michalsky, 1993. "All- */
    /*				Weather Model for Sky Luminance Distribution - */
    /*				Preliminary Configuration and Validation," Solar */
    /*				Energy 50(3):235-245, Table 1. */

    const double PerezCoeff[8][20] =
    {
        /* Sky clearness (epsilon): 1.000 to 1.065 */
        {   1.3525,  -0.2576,  -0.2690,  -1.4366,   -0.7670,
            0.0007,   1.2734,  -0.1233,   2.8000,    0.6004,
            1.2375,   1.0000,   1.8734,   0.6297,    0.9738,
            0.2809,   0.0356,  -0.1246,  -0.5718,    0.9938 },
        /* Sky clearness (epsilon): 1.065 to 1.230 */
        {  -1.2219,  -0.7730,   1.4148,   1.1016,   -0.2054,
            0.0367,  -3.9128,   0.9156,   6.9750,    0.1774,
            6.4477,  -0.1239,  -1.5798,  -0.5081,   -1.7812,
            0.1080,   0.2624,   0.0672,  -0.2190,   -0.4285 },
        /* Sky clearness (epsilon): 1.230 to 1.500 */
        {  -1.1000,  -0.2515,   0.8952,   0.0156,    0.2782,
           -0.1812, - 4.5000,   1.1766,  24.7219,  -13.0812,
          -37.7000,  34.8438,  -5.0000,   1.5218,    3.9229,
           -2.6204,  -0.0156,   0.1597,   0.4199,   -0.5562 },
        /* Sky clearness (epsilon): 1.500 to 1.950 */
        {  -0.5484,  -0.6654,  -0.2672,   0.7117,   0.7234,
           -0.6219,  -5.6812,   2.6297,  33.3389, -18.3000,
          -62.2500,  52.0781,  -3.5000,   0.0016,   1.1477,
            0.1062,   0.4659,  -0.3296,  -0.0876,  -0.0329 },
        /* Sky clearness (epsilon): 1.950 to 2.800 */
        {  -0.6000,  -0.3566,  -2.5000,   2.3250,   0.2937,
            0.0496,  -5.6812,   1.8415,  21.0000,  -4.7656 ,
          -21.5906,   7.2492,  -3.5000,  -0.1554,   1.4062,
            0.3988,   0.0032,   0.0766,  -0.0656,  -0.1294 },
        /* Sky clearness (epsilon): 2.800 to 4.500 */
        {  -1.0156,  -0.3670,   1.0078,   1.4051,   0.2875,
           -0.5328,  -3.8500,   3.3750,  14.0000,  -0.9999,
           -7.1406,   7.5469,  -3.4000,  -0.1078,  -1.0750,
            1.5702,  -0.0672,   0.4016,   0.3017,  -0.4844 },
        /* Sky clearness (epsilon): 4.500 to 6.200 */
        {  -1.0000,   0.0211,   0.5025,  -0.5119,  -0.3000,
            0.1922,   0.7023,  -1.6317,  19.0000,  -5.0000,
            1.2438,  -1.9094,  -4.0000,   0.0250,   0.3844,
            0.2656,   1.0468,  -0.3788,  -2.4517,   1.4656 },
        /* Sky clearness (epsilon): 6.200 to ... */
        {  -1.0500,   0.0289,   0.4260,   0.3590,  -0.3250,
            0.1156,   0.7781,   0.0025,  31.0625, -14.5000,
          -46.1148,  55.3750,  -7.2312,   0.4050,  13.3500,
            0.6234,   1.5000,  -0.6426,   1.8564,   0.5636 }
    };

    /* Perez irradiance component model coefficients */

    /* Reference:	Perez, R., P. Ineichen, R. Seals, J. Michalsky, and R. */
    /*				Stewart. 1990. ìModeling Daylight Availability and */
    /*				Irradiance Components from Direct and Global */
    /*				Irradiance,î Solar Energy 44(5):271-289. */

    typedef struct
    {
        double lower;	/* Lower bound */
        double upper;	/* Upper bound */
    } CategoryBounds;

    /* Perez sky clearness (epsilon) categories (Table 1) */
    const CategoryBounds SkyClearCat[8] =
    {
        { 1.000, 1.065 },	/* Overcast */
        { 1.065, 1.230 },
        { 1.230, 1.500 },
        { 1.500, 1.950 },
        { 1.950, 2.800 },
        { 2.800, 4.500 },
        { 4.500, 6.200 },
        { 6.200, 12.01 }	/* Clear */
    };

    /* Luminous efficacy model coefficients */
    typedef struct
    {
        double a;
        double b;
        double c;
        double d;
    } ModelCoeff;

    /* Diffuse luminous efficacy model coefficients (Table 4, Eqn. 7) */
    const ModelCoeff DiffuseLumEff[8] =
    {
        {  97.24, -0.46,  12.00,  -8.91 },
        { 107.22,  1.15,   0.59,  -3.95 },
        { 104.97,  2.96,  -5.53,  -8.77 },
        { 102.39,  5.59, -13.95, -13.90 },
        { 100.71,  5.94, -22.75, -23.74 },
        { 106.42,  3.83, -36.15, -28.83 },
        { 141.88,  1.90, -53.24, -14.03 },
        { 152.23,  0.35, -45.27,  -7.98 }
    };

    /* Direct luminous efficacy model coefficients (Table 4, Eqn. 8) */
    const ModelCoeff DirectLumEff[8] =
    {
        {  57.20, -4.55, -2.98, 117.12 },
        {  98.99, -3.46, -1.21,  12.38 },
        { 109.83, -4.90, -1.71,  -8.81 },
        { 110.34, -5.84, -1.99,  -4.56 },
        { 106.36, -3.97, -1.75,  -6.16 },
        { 107.19, -1.25, -1.51, -26.73 },
        { 105.75,  0.77, -1.26, -34.44 },
        { 101.18,  1.58, -1.10,  -8.29 }
    };

    #ifndef NSUNPATCH
    #define	NSUNPATCH	4		/* max. # patches to spread sun into */
    #endif

    
   

    int
    jdate(        /* Julian date (days into year) */
          int month,
          int day
          )
    {
        static short  mo_da[12] = {0,31,59,90,120,151,181,212,243,273,304,334};
        
        return(mo_da[month-1] + day);
    }


    double
    stadj(        /* solar time adjustment from Julian date */
          int  jd
          )
    {
        return( 0.170 * sin( (4*PI/373) * (jd - 80) ) -
               0.129 * sin( (2*PI/355) * (jd - 8) ) +
               12 * (s_meridian - s_longitude) / PI );
    }


    double
    sdec(        /* solar declination angle from Julian date */
         int  jd
         )
    {
        return( 0.4093 * sin( (2*PI/368) * (jd - 81) ) );
    }


    double
    salt(    /* solar altitude from solar declination and solar time */
         double sd,
         double st
         )
    {
        return( asin( sin(s_latitude) * sin(sd) -
                     cos(s_latitude) * cos(sd) * cos(st*(PI/12)) ) );
    }


    double
    sazi(    /* solar azimuth from solar declination and solar time */
         double sd,
         double st
         )
    {
        return( -atan2( cos(sd)*sin(st*(PI/12)),
                       -cos(s_latitude)*sin(sd) -
                       sin(s_latitude)*cos(sd)*cos(st*(PI/12)) ) );
    }

    int		nsuns = NSUNPATCH;	/* number of sun patches to use */
    double		fixed_sun_sa = -1;	/* fixed solid angle per sun? */

    int		rhsubdiv = 1;		/* Reinhart sky subdivisions */

    COLOR		skycolor = {1.0, 1.0, 1.0}; //{.96, 1.004, 1.118};	/* sky coloration */
    COLOR		suncolor = {1., 1., 1.};	/* sun color */
    COLOR		grefl = {.2, .2, .2};		/* ground reflectance */

    int		nskypatch;		/* number of Reinhart patches */
    float		*rh_palt;		/* sky patch altitudes (radians) */
    float		*rh_pazi;		/* sky patch azimuths (radians) */
    float		*rh_dom;		/* sky patch solid angle (sr) */

    

    //extern int	rh_init(void);
    //extern float *	resize_dmatrix(float *mtx_data, int nsteps, int npatch);
    //extern void	AddDirect(float *parr);


    static const char *
    getfmtname(int fmt)
    {
        switch (fmt) {
        case 'a':
            return("ascii");
        case 'f':
            return("float");
        case 'd':
            return("double");
        }
        return("unknown");
    }

    /* Return maximum of two doubles */
    double dmax( double a, double b )
    { return (a > b) ? a : b; }

    /* Compute sky patch radiance values (modified by GW) */
    void
    ComputeSky(float *parr)
    {
        int index;			/* Category index */
        double norm_diff_illum;		/* Normalized diffuse illuimnance */
        int i;
        
        /* Calculate atmospheric precipitable water content */
        apwc = CalcPrecipWater(dew_point);

        /* Calculate sun zenith angle (don't let it dip below horizon) */
        /* Also limit minimum angle to keep circumsolar off zenith */
        if (altitude <= 0.0)
            sun_zenith = DegToRad(90.0);
        else if (altitude >= DegToRad(87.0))
            sun_zenith = DegToRad(3.0);
        else
            sun_zenith = DegToRad(90.0) - altitude;

        /* Compute the inputs for the calculation of the sky distribution */
        
        if (input == 0)					/* XXX never used */
        {
            /* Calculate irradiance */
            diff_irrad = CalcDiffuseIrradiance();
            dir_irrad = CalcDirectIrradiance();
            
            /* Calculate illuminance */
            index = GetCategoryIndex();
            diff_illum = diff_irrad * CalcDiffuseIllumRatio(index);
            dir_illum = dir_irrad * CalcDirectIllumRatio(index);
        }
        else if (input == 1)
        {
            sky_brightness = CalcSkyBrightness();
            sky_clearness =  CalcSkyClearness();

            /* Limit sky clearness */
            if (sky_clearness > 11.9)
                sky_clearness = 11.9;

            /* Limit sky brightness */
            if (sky_brightness < 0.01)
                sky_brightness = 0.01;

            /* Calculate illuminance */
            index = GetCategoryIndex();
            diff_illum = diff_irrad * CalcDiffuseIllumRatio(index);
            dir_illum = dir_irrad * CalcDirectIllumRatio(index);
        }
        else if (input == 2)
        {
            /* Calculate sky brightness and clearness from illuminance values */
            index = CalcSkyParamFromIllum();
        }

        if (output == 1) {			/* hack for solar radiance */
            diff_illum = diff_irrad * WHTEFFICACY;
            dir_illum = dir_irrad * WHTEFFICACY;
        }

        if (bright(skycolor) <= 1e-4) {			/* 0 sky component? */
            memset(parr, 0, sizeof(float)*3*nskypatch);
            return;
        }
        /* Compute ground radiance (include solar contribution if any) */
        parr[0] = diff_illum;
        if (altitude > 0)
            parr[0] += dir_illum * sin(altitude);
        parr[2] = parr[1] = parr[0] *= (1./PI/WHTEFFICACY);
        multcolor(parr, grefl);

        /* Calculate Perez sky model parameters */
        CalcPerezParam(sun_zenith, sky_clearness, sky_brightness, index);

        /* Calculate sky patch luminance values */
        CalcSkyPatchLumin(parr);

        /* Calculate relative horizontal illuminance */
        norm_diff_illum = CalcRelHorzIllum(parr);

        /* Check for zero sky -- make uniform in that case */
        if (norm_diff_illum <= FTINY) {
            for (i = 1; i < nskypatch; i++)
                setcolor(parr+3*i, 1., 1., 1.);
            norm_diff_illum = PI;
        }
        /* Normalization coefficient */
        norm_diff_illum = diff_illum / norm_diff_illum;

        /* Apply to sky patches to get absolute radiance values */
        for (i = 1; i < nskypatch; i++) {
            scalecolor(parr+3*i, norm_diff_illum*(1./WHTEFFICACY));
            multcolor(parr+3*i, skycolor);
        }
    }

    /* Add in solar direct to nearest sky patches (GW) */
    void
    AddDirect(float *parr)
    {
        FVECT	svec;
        double	near_dprod[NSUNPATCH];
        int	near_patch[NSUNPATCH];
        double	wta[NSUNPATCH], wtot;
        int	i, j, p;

        if (dir_illum <= 1e-4 || bright(suncolor) <= 1e-4)
            return;
                        /* identify nsuns closest patches */
        if (nsuns > NSUNPATCH)
            nsuns = NSUNPATCH;
        else if (nsuns <= 0)
            nsuns = 1;
        for (i = nsuns; i--; )
            near_dprod[i] = -1.;
        vector(svec, altitude, azimuth);
        for (p = 1; p < nskypatch; p++) {
            FVECT	pvec;
            double	dprod;
            rh_vector(pvec, p);
            dprod = DOT(pvec, svec);
            for (i = 0; i < nsuns; i++)
                if (dprod > near_dprod[i]) {
                    for (j = nsuns; --j > i; ) {
                        near_dprod[j] = near_dprod[j-1];
                        near_patch[j] = near_patch[j-1];
                    }
                    near_dprod[i] = dprod;
                    near_patch[i] = p;
                    break;
                }
        }
        wtot = 0;			/* weight by proximity */
        for (i = nsuns; i--; )
            wtot += wta[i] = 1./(1.002 - near_dprod[i]);
                        /* add to nearest patch radiances */
        for (i = nsuns; i--; ) {
            float	*pdest = parr + 3*near_patch[i];
            float	val_add = wta[i] * dir_illum / (WHTEFFICACY * wtot);

            val_add /= (fixed_sun_sa > 0)	? fixed_sun_sa
                            : rh_dom[near_patch[i]] ;
            *pdest++ += val_add*suncolor[0];
            *pdest++ += val_add*suncolor[1];
            *pdest++ += val_add*suncolor[2];
        }
    }

    /* Initialize Reinhart sky patch positions (GW) */
    int
    rh_init(void)
    {
    #define	NROW	7
        static const int	tnaz[NROW] = {30, 30, 24, 24, 18, 12, 6};
        const double		alpha = (PI/2.)/(NROW*rhsubdiv + .5);
        int			p, i, j;
                        /* allocate patch angle arrays */
        nskypatch = 0;
        for (p = 0; p < NROW; p++)
            nskypatch += tnaz[p];
        nskypatch *= rhsubdiv*rhsubdiv;
        nskypatch += 2;
        rh_palt = (float *)malloc(sizeof(float)*nskypatch);
        rh_pazi = (float *)malloc(sizeof(float)*nskypatch);
        rh_dom = (float *)malloc(sizeof(float)*nskypatch);
        if ((rh_palt == NULL) | (rh_pazi == NULL) | (rh_dom == NULL)) {
            fprintf(stderr, "%s: out of memory in rh_init()\n", gendaymtxname);
            exit(1);
        }
        rh_palt[0] = -PI/2.;		/* ground & zenith patches */
        rh_pazi[0] = 0.;
        rh_dom[0] = 2.*PI;
        rh_palt[nskypatch-1] = PI/2.;
        rh_pazi[nskypatch-1] = 0.;
        rh_dom[nskypatch-1] = 2.*PI*(1. - cos(alpha*.5));
        p = 1;				/* "normal" patches */
        for (i = 0; i < NROW*rhsubdiv; i++) {
            const float	ralt = alpha*(i + .5);
            const int	ninrow = tnaz[i/rhsubdiv]*rhsubdiv;
            const float	dom = 2.*PI*(sin(alpha*(i+1)) - sin(alpha*i)) /
                            (double)ninrow;
            for (j = 0; j < ninrow; j++) {
                rh_palt[p] = ralt;
                rh_pazi[p] = 2.*PI * j / (double)ninrow;
                rh_dom[p++] = dom;
            }
        }
        return nskypatch;
    #undef NROW
    }

    /* Resize daylight matrix (GW) */
    float *
    resize_dmatrix(float *mtx_data, int nsteps, int npatch)
    {
        if (mtx_data == NULL)
            mtx_data = (float *)malloc(sizeof(float)*3*nsteps*npatch);
        else
            mtx_data = (float *)realloc(mtx_data,
                        sizeof(float)*3*nsteps*npatch);
        if (mtx_data == NULL) {
            fprintf(stderr, "%s: out of memory in resize_dmatrix(%d,%d)\n",
                    gendaymtxname, nsteps, npatch);
            exit(1);
        }
        return(mtx_data);
    }

    /* Determine category index */
    int GetCategoryIndex()
    {
        int index;	/* Loop index */

        for (index = 0; index < 8; index++)
            if ((sky_clearness >= SkyClearCat[index].lower) &&
                    (sky_clearness < SkyClearCat[index].upper))
                break;

        return index;
    }

    /* Calculate diffuse illuminance to diffuse irradiance ratio */

    /* Reference:	Perez, R., P. Ineichen, R. Seals, J. Michalsky, and R. */
    /*				Stewart. 1990. ìModeling Daylight Availability and */
    /*				Irradiance Components from Direct and Global */
    /*				Irradiance,î Solar Energy 44(5):271-289, Eqn. 7. */

    double CalcDiffuseIllumRatio( int index )
    {
        ModelCoeff const *pnle;	/* Category coefficient pointer */
        
        /* Get category coefficient pointer */
        pnle = &(DiffuseLumEff[index]);

        return pnle->a + pnle->b * apwc + pnle->c * cos(sun_zenith) +
                pnle->d * log(sky_brightness);
    }

    /* Calculate direct illuminance to direct irradiance ratio */

    /* Reference:	Perez, R., P. Ineichen, R. Seals, J. Michalsky, and R. */
    /*				Stewart. 1990. ìModeling Daylight Availability and */
    /*				Irradiance Components from Direct and Global */
    /*				Irradiance,î Solar Energy 44(5):271-289, Eqn. 8. */

    double CalcDirectIllumRatio( int index )
    {
        ModelCoeff const *pnle;	/* Category coefficient pointer */

        /* Get category coefficient pointer */
        pnle = &(DirectLumEff[index]);

        /* Calculate direct illuminance from direct irradiance */
        
        return dmax((pnle->a + pnle->b * apwc + pnle->c * exp(5.73 *
                sun_zenith - 5.0) + pnle->d * sky_brightness),
                0.0);
    }

    /* Calculate sky brightness */

    /* Reference:	Perez, R., P. Ineichen, R. Seals, J. Michalsky, and R. */
    /*				Stewart. 1990. ìModeling Daylight Availability and */
    /*				Irradiance Components from Direct and Global */
    /*				Irradiance,î Solar Energy 44(5):271-289, Eqn. 2. */

    double CalcSkyBrightness()
    {
        return diff_irrad * CalcAirMass() / (DC_SolarConstantE *
                CalcEccentricity());
    }

    /* Calculate sky clearness */

    /* Reference:	Perez, R., P. Ineichen, R. Seals, J. Michalsky, and R. */
    /*				Stewart. 1990. ìModeling Daylight Availability and */
    /*				Irradiance Components from Direct and Global */
    /*				Irradiance,î Solar Energy 44(5):271-289, Eqn. 1. */

    double CalcSkyClearness()
    {
        double sz_cubed;	/* Sun zenith angle cubed */

        /* Calculate sun zenith angle cubed */
        sz_cubed = sun_zenith*sun_zenith*sun_zenith;

        return ((diff_irrad + dir_irrad) / diff_irrad + 1.041 *
                sz_cubed) / (1.0 + 1.041 * sz_cubed);
    }

    /* Calculate diffuse horizontal irradiance from Perez sky brightness */

    /* Reference:	Perez, R., P. Ineichen, R. Seals, J. Michalsky, and R. */
    /*				Stewart. 1990. ìModeling Daylight Availability and */
    /*				Irradiance Components from Direct and Global */
    /*				Irradiance,î Solar Energy 44(5):271-289, Eqn. 2 */
    /*				(inverse). */

    double CalcDiffuseIrradiance()
    {
        return sky_brightness * DC_SolarConstantE * CalcEccentricity() /
                CalcAirMass();
    }

    /* Calculate direct normal irradiance from Perez sky clearness */

    /* Reference:	Perez, R., P. Ineichen, R. Seals, J. Michalsky, and R. */
    /*				Stewart. 1990. ìModeling Daylight Availability and */
    /*				Irradiance Components from Direct and Global */
    /*				Irradiance,î Solar Energy 44(5):271-289, Eqn. 1 */
    /*				(inverse). */

    double CalcDirectIrradiance()
    {
        return CalcDiffuseIrradiance() * ((sky_clearness - 1.0) * (1 + 1.041
                * sun_zenith*sun_zenith*sun_zenith));
    }

    /* Calculate sky brightness and clearness from illuminance values */
    int CalcSkyParamFromIllum()
    {
        double test1 = 0.1;
        double test2 = 0.1;
        int	counter = 0;
        int index = 0;			/* Category index */

        /* Convert illuminance to irradiance */
        diff_irrad = diff_illum * DC_SolarConstantE /
                (DC_SolarConstantL * 1000.0);
        dir_irrad = dir_illum * DC_SolarConstantE /
                (DC_SolarConstantL * 1000.0);

        /* Calculate sky brightness and clearness */
        sky_brightness = CalcSkyBrightness();
        sky_clearness =  CalcSkyClearness();

        /* Limit sky clearness */
        if (sky_clearness > 12.0)
            sky_clearness = 12.0;

        /* Limit sky brightness */
        if (sky_brightness < 0.01)
                sky_brightness = 0.01;

        while (((fabs(diff_irrad - test1) > 10.0) ||
                (fabs(dir_irrad - test2) > 10.0)) && !(counter == 5))
        {
            test1 = diff_irrad;
            test2 = dir_irrad;
            counter++;
        
            /* Convert illuminance to irradiance */
            index = GetCategoryIndex();
            diff_irrad = diff_illum / CalcDiffuseIllumRatio(index);
            dir_irrad = CalcDirectIllumRatio(index);
            if (dir_irrad > 0.1)
                dir_irrad = dir_illum / dir_irrad;
        
            /* Calculate sky brightness and clearness */
            sky_brightness = CalcSkyBrightness();
            sky_clearness =  CalcSkyClearness();

            /* Limit sky clearness */
            if (sky_clearness > 12.0)
                sky_clearness = 12.0;
        
            /* Limit sky brightness */
            if (sky_brightness < 0.01)
                sky_brightness = 0.01;
        }

        return GetCategoryIndex();
    }

    /* Calculate relative luminance */

    /* Reference:	Perez, R., R. Seals, and J. Michalsky. 1993. */
    /*				ìAll-Weather Model for Sky Luminance Distribution - */
    /*				Preliminary Configuration and Validation,î Solar Energy */
    /*				50(3):235-245, Eqn. 1. */

    double CalcRelLuminance( double gamma, double zeta )
    {
        return (1.0 + perez_param[0] * exp(perez_param[1] / cos(zeta))) *
                (1.0 + perez_param[2] * exp(perez_param[3] * gamma) +
                perez_param[4] * cos(gamma) * cos(gamma));
    }

    /* Calculate Perez sky model parameters */

    /* Reference:	Perez, R., R. Seals, and J. Michalsky. 1993. */
    /*				ìAll-Weather Model for Sky Luminance Distribution - */
    /*				Preliminary Configuration and Validation,î Solar Energy */
    /*				50(3):235-245, Eqns. 6 - 8. */

    void CalcPerezParam( double sz, double epsilon, double delta,
            int index )
    {
        double x[5][4];		/* Coefficents a, b, c, d, e */
        int i, j;			/* Loop indices */

        /* Limit sky brightness */
        if (epsilon > 1.065 && epsilon < 2.8)
        {
            if (delta < 0.2)
                delta = 0.2;
        }

        /* Get Perez coefficients */
        for (i = 0; i < 5; i++)
            for (j = 0; j < 4; j++)
                x[i][j] = PerezCoeff[index][4 * i + j];

        if (index != 0)
        {
            /* Calculate parameter a, b, c, d and e (Eqn. 6) */
            for (i = 0; i < 5; i++)
                perez_param[i] = x[i][0] + x[i][1] * sz + delta * (x[i][2] +
                        x[i][3] * sz);
        }
        else
        {
            /* Parameters a, b and e (Eqn. 6) */
            perez_param[0] = x[0][0] + x[0][1] * sz + delta * (x[0][2] +
                    x[0][3] * sz);
            perez_param[1] = x[1][0] + x[1][1] * sz + delta * (x[1][2] +
                    x[1][3] * sz);
            perez_param[4] = x[4][0] + x[4][1] * sz + delta * (x[4][2] +
                    x[4][3] * sz);

            /* Parameter c (Eqn. 7) */
            perez_param[2] = exp(pow(delta * (x[2][0] + x[2][1] * sz),
                    x[2][2])) - x[2][3];

            /* Parameter d (Eqn. 8) */
            perez_param[3] = -exp(delta * (x[3][0] + x[3][1] * sz)) +
                    x[3][2] + delta * x[3][3];
        }
    }

    /* Calculate relative horizontal illuminance (modified by GW) */

    /* Reference:	Perez, R., R. Seals, and J. Michalsky. 1993. */
    /*				ìAll-Weather Model for Sky Luminance Distribution - */
    /*				Preliminary Configuration and Validation,î Solar Energy */
    /*				50(3):235-245, Eqn. 3. */

    double CalcRelHorzIllum( float *parr )
    {
        int i;
        double rh_illum = 0.0;	/* Relative horizontal illuminance */

        for (i = 1; i < nskypatch; i++)
            rh_illum += parr[3*i+1] * rh_cos(i) * rh_dom[i];

        return rh_illum;
    }

    /* Calculate earth orbit eccentricity correction factor */

    /* Reference:	Sen, Z. 2008. Solar Energy Fundamental and Modeling  */
    /*				Techniques. Springer, p. 72. */

    double CalcEccentricity()
    {
        double day_angle;	/* Day angle (radians) */
        double E0;			/* Eccentricity */

        /* Calculate day angle */
        day_angle  = (julian_date - 1.0) * (2.0 * PI / 365.0);

        /* Calculate eccentricity */
        E0 = 1.00011 + 0.034221 * cos(day_angle) + 0.00128 * sin(day_angle)
                + 0.000719 * cos(2.0 * day_angle) + 0.000077 * sin(2.0 *
                day_angle);

        return E0;
    }

    /* Calculate atmospheric precipitable water content */

    /* Reference:	Perez, R., P. Ineichen, R. Seals, J. Michalsky, and R. */
    /*				Stewart. 1990. ìModeling Daylight Availability and */
    /*				Irradiance Components from Direct and Global */
    /*				Irradiance,î Solar Energy 44(5):271-289, Eqn. 3. */

    /* Note:	The default surface dew point temperature is 11 deg. C */
    /*			(52 deg. F). Typical values are: */

    /*			Celsius 	Fahrenheit	 	Human Perception */
    /*			> 24 		> 75 			Extremely uncomfortable */
    /*			21 - 24 	70 - 74 		Very humid */
    /*			18 - 21		65 - 69		 	Somewhat uncomfortable */
    /*			16 - 18 	60 - 64 		OK for most people */
    /*			13 - 16 	55 - 59		 	Comfortable */
    /*			10 - 12 	50 - 54		 	Very comfortable */
    /*			< 10 		< 49		 	A bit dry for some */

    double CalcPrecipWater( double dpt )
    { return exp(0.07 * dpt - 0.075); }

    /* Calculate relative air mass */

    /* Reference:	Kasten, F. 1966. "A New Table and Approximation Formula */
    /*				for the Relative Optical Air Mass," Arch. Meteorol. */
    /*				Geophys. Bioklimataol. Ser. B14, pp. 206-233. */

    /* Note:		More sophisticated relative air mass models are */
    /*				available, but they differ significantly only for */
    /*				sun zenith angles greater than 80 degrees. */

    double CalcAirMass()
    {
        return (1.0 / (cos(sun_zenith) + 0.15 * pow(93.885 -
                RadToDeg(sun_zenith), -1.253)));
    }

    /* Calculate Perez All-Weather sky patch luminances (modified by GW) */

    /* NOTE: The sky patches centers are determined in accordance with the */
    /*       BRE-IDMP sky luminance measurement procedures. (See for example */
    /*       Mardaljevic, J. 2001. "The BRE-IDMP Dataset: A New Benchmark */
    /*       for the Validation of Illuminance Prediction Techniques," */
    /*       Lighting Research & Technology 33(2):117-136.) */

    void CalcSkyPatchLumin( float *parr )
    {
        int i;
        double aas;				/* Sun-sky point azimuthal angle */
        double sspa;			/* Sun-sky point angle */
        double zsa;				/* Zenithal sun angle */

        for (i = 1; i < nskypatch; i++)
        {
            /* Calculate sun-sky point azimuthal angle */
            aas = fabs(rh_pazi[i] - azimuth);

            /* Calculate zenithal sun angle */
            zsa = PI * 0.5 - rh_palt[i];

            /* Calculate sun-sky point angle (Equation 8-20) */
            sspa = acos(cos(sun_zenith) * cos(zsa) + sin(sun_zenith) *
                    sin(zsa) * cos(aas));

            /* Calculate patch luminance */
            parr[3*i] = CalcRelLuminance(sspa, zsa);
            if (parr[3*i] < 0) parr[3*i] = 0;
            parr[3*i+2] = parr[3*i+1] = parr[3*i];
        }
    }

};

