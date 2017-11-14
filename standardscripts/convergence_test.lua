--[[

@version 1.0.0
@author German Molina
@date November 2, 2017
@title Convergence test
@brief Tries to find the 'best' ambient parameters for solving your model

This scripts iterates trying to find the optimal parameters (i.e. those that 
take as little time as possible in solving) by testing different combinations 
of them

@input[required] File with sensor points to perform test
@input[optional] Number of randomly selected sensors to use for convergence test
@input[optional] Increase in Ambient Divisions (i.e. next AD will be arg3*AD)
@input[optional] Increase in Ambient Bounces (i.e. next AB will be AB + arg4)

]]

-- PARSE INPUTS
-- ============
sensor_file = arg1 or 0.2
n_sensors = arg2 or 28
increase_ad = arg3 or 1.6
increase_ab = arg4 or 1 

-- CHECK ARGUMENTS
-- ===============
if not file_exists(sensor_file) then
    raise("Sensor file '"..sensor_file.."' does not exist")
end

-- PROCESS
-- =======
-- Set parameters to something low
ray_trace_parameters{
    ab = 1;
    ad = current_ad;
}

-- Increase ambient divitions until converges
-- ===============================================

-- Define initial values
current_illuminance = lux_meter(0,0,1)

-- calculate initial error
error = 1e9

-- Calculate
while error > converge_ad do
    -- Increase ad
    current_ad = current_ad*increase_ad

    -- Modify params
    ray_trace_parameters{
        -- ab is already defined
        ad = current_ad;
    }

    -- calculate error
    error = math.abs(current_illuminance - lux_meter(0,0,1))
end

-- FIND AMBIENT BOUNCES
-- ====================

-- Define initial values
ray_trace_parameters {
    ab = current_ab;
}
current_illuminance = lux_meter(0,0,1)

-- calculate initial error
error = 1e9

-- Calculate
while error > converge_ad do
    -- Increase ab
    current_ab = current_ab + increase_ab

    -- Modify params
    ray_trace_parameters{
        -- ab is already defined
        ab = current_ab;
    }

    -- calculate error
    error = math.abs(current_illuminance - lux_meter(0,0,1))
end

-- REPORT
-- ======
print_ray_trace_options()