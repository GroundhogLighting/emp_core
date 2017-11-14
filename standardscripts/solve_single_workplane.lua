--[[

@version 1.0.0
@author German Molina
@date November 2, 2017

@title Solves a single workplane
@brief This script solves all the objectives in the model that correspond to a single workplane

@input[required] The workplane name
@input[optional] A file with options

]]

-- PARSE INPUTS
-- ============

wp_name = arg1
optfile = arg2


-- CHECK ARGUMENTS
-- ===============

if wp_name == nil then
    warn("A Workplane name is required as first input")
    return
end

if not workplane_exist(wp_name) then
    warn("Workplane "..wp_name.." does not exist")
    return
end

if arg2 ~= nil then
   load_ray_trace_options(arg2) 
end


-- All good... do the calculation
-- ==============================

-- Gather all the tasks in a workplane
tasks = get_tasks_by_workplane(arg1)

for i=1,#tasks do
    task_manager.add(tasks[i])
end
