--[[

@version 1.0.0
@author German Molina
@date January 15, 2018
@title Export in Radiance format
@brief Exports the model in Radiance format

This scripts loads the model, creates the corresponding directories, and writes 
the Radiance files corresponding to such model, including workplanes

@input[optional] The name of the directory
]]

-- Retrieve the main directory
-- ===========================
out_dir = argv[1] or "RadianceModel"
-- auto_solve = false

-- Config
-- ======

scene_file = "scene.rad"
layers_directory = "Geometry"
views_directory = "Views"
skies_directory = "Skies"
materials_directory = "Materials"
materials_file = "materials.mat"
components_directory = "Components"
windows_directory = "Windows"
illums_directory = "Illums"
photosensors_directory = "Photosensors"
rif_file = "scene.rif"
info_file = "modelinfo.txt"
workplanes_directory = "Workplanes"
sky_name = "sky.rad"
weather_name = "weather.wea"

-- Create main directory
os.execute("mkdir -p "..out_dir)

-- Write scene file
task {
    class = "write_scene_file";
    filename = out_dir.."/"..scene_file;
    layers_directory = layers_directory;    
}

-- Write model info
task {
    class = "write_model_info";
    filename = out_dir.."/"..info_file -- change this to modify the directory
}

-- Write rif file
task {
    class = "write_rif_file";
    filename = out_dir.."/"..rif_file;
    layers_directory = layers_directory;    
    views_directory = views_directory;  
    skies_directory = skies_directory;  
    materials_directory = materials_directory;
    materials_file = materials_file;
    scene_file = scene_file;
    components_directory = components_directory;
    windows_directory = windows_directory;
    illums_directory = illums_directory;
}

-- Write components
task {
    class = "write_components";
    directory = out_dir.."/"..components_directory;
}

-- Write Views
task {
    class = "write_views";
    directory = out_dir.."/"..views_directory;
}

-- Write current sky
task {
    class = "write_current_sky";
    directory = out_dir.."/"..skies_directory;
    filename = sky_name;
}

-- Write weather
task {
    class = "write_current_weather";
    directory = out_dir.."/"..skies_directory;
    filename = weather_name;
}

-- Write materials
task {
    class = "write_materials";
    directory = out_dir.."/"..materials_directory;
    filename = out_dir.."/"..materials_file;
}


-- Write layers
task {
    class = "write_layers";
    directory = out_dir.."/"..layers_directory;
}


-- Write illums
--task {
--    class = "write_illums";
--    directory = out_dir.."/"..illums_directory;
--}

-- Write photosensors
task {
    class = "write_photosensors";
    directory = out_dir.."/"..photosensors_directory;
}

print_task_flow("flow1.txt")

-- Write workplanes
workplanes = get_workplanes_list()

if #workplanes > 0 then
    os.execute("mkdir -p "..out_dir.."/"..workplanes_directory)
end

for i=1,#workplanes do
    task {
        class = "write_workplane";
        directory = out_dir.."/"..workplanes_directory;
        name = workplanes[i].."_triangulation";
        workplane = workplanes[i];
        max_area = 0.25;
        max_aspect_ratio = 1.3;
    }
end



print_task_flow("flow2.txt")