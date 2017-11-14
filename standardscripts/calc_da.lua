--[[

@title Calculate daylight autonomy
@brief Calcualtes the Daylight Autonomy for one or more workplanes
@author German Molina
@date November 4, 2017
@version 1.0.0

This script calculates the Daylight Autonomy. That is, percentage of 
the workplane that maintains a minimum illuminance

This is another paragraph

@input[required] Expected lux
@input[required] Expected time
@input[required] Beggining of the occupied hours
@input[required] End of the occupied hours
@input[required] Workplanes to calculate

]]

-- PARSE INPUTS
-- ============
min_lux = args[1]
min_time = args[2]
early = args[3]
late = args[4]

if not args[5] then
    raise("No workplanes were input")
end

-- ADD TASK
-- ========
nwps = #args-4
for i=1,nwps do
    if not worplane_exist(args[i+4]) then
        warn("Workplane '"..args[i+4].."' does not exist. It will be ignored")
    else

        wp_name = args[i+4]
        
        add_task {
            name = "Daylight Autonomy";
            workplane = wp_name;
            metric = "DA";
            min_illuminance = min_lux;
            min_time = min_time;
        }
    end
end

