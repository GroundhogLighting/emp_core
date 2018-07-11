The files included in the skp_files folder are clasified by SketchUp version. They
also are supposed to enable testing the reading capabilities of emp_core

# Basic

## empty.skp
An empty file (i.e. Open Sketchup, save file) designed to test if a standard SketchUp model can be read properly

### Expected Behaviour
 1. No Throw on load
 2. Only one componet
 3. Have 32 faces in the only component of version 2017

## deleted.skp

A model on which a 1x1x1 cube with a corner in the origin has ben drawn, and the top of it has been deleted. No materials assigned.

This is designed to test if deleted faces are an issue or not (in Ruby they sometimes are)

### Expected Behaviour

 1. No throw on load
 2. Have 5 faces in the first layer
 
 # Faces
 
 ## small_square.skp

It is a simple model, with two layers (Layer0 and Layer1), and a 1x1 meter square in Z = 0, and with vertices at (0,0), (0,1), (1,1), (1,0)

Designed to test how faces are imported

### Expected Behaviour

1. Have two layers with the correct names (Layer0 and Layer1)
2. Have zero components
3. Have a face with four vertices, matching the correct ones

## face_with_holes.skp

It is a model with one face that have 7 holes (inner loops). This test is designed to check that holes are imported properly

### Expected Behaviour

1. The model should have a single face
2. The face should have 7 inner loops
3. The model should have a single layer, called Layer0


# Workplanes

## small_square_WP.skp
It is a simple model, with two layers (Layer0 and Layer1), and a 1x1 meter WorkPlane called 'WP' in Z = 0, and with vertices at (0,0), (0,1), (1,1), (1,0). The pixelSize in such workplane is 0.76

Designed to test how Workplanes are imported

### Expected Behaviour
1. Have two layers with the correct names (Layer0 and Layer1)
2. Have zero components
3. Have a workplane with four vertices, matching the correct ones

## workplane_group.skp

Contains one workplane with 3 polygons in it.

Designed to test if several polygons are put in the same workplane.

### Expected behavior

1. Have a single empty layer
2. Have no materials
3. Have no geometry
4. Have a single workplane called WP with three polygons
5. No components

## ugly_workplane.skp

Designed to test if the Workplanes with split edges (i.e. an edge that seems like one but are, indeed, two) are being cleaned correctly

### Expected Behaviour

1. Load properly
2. Have only one workplane called WP with one polygon, which has 6 vertices


# Illums

## small_square_illum.skp

It is a simple model, with two layers (Layer0 and Layer1), and a 1x1 meter Illum called in Z = 0, and with vertices at (0,0), (0,1), (1,1), (1,0)

Designed to test how illums are imported

### Expected Behaviour

1. Have two layers with the correct names (Layer0 and Layer1)
2. Have zero components
3. Have an illum with four vertices, matching the correct ones


## illums_group.skp

It is a simple model, with two four 1x1m illums called ILLUMS

Designed to test how illums_group are imported

### Expected Behaviour

1. Have a single illum group with four elements in it.
2. The window group should be called 'ILLUMS'


# Windows

## small_square_window.skp

It is a simple model, with two layers (Layer0 and Layer1), and a 1x1 meter window called in Z = 0, and with vertices at (0,0), (0,1), (1,1), (1,0)

Designed to test how squares are imported

### Expected Behaviour

1. Have two layers with the correct names (Layer0 and Layer1)
2. Have zero components
3. Have an window with four vertices, matching the correct ones

## window_group.skp

It is a simple model, with two four 1x1m windows called WINDOWS

Designed to test how window_groups are imported

### Expected Behaviour

1. Have a single window group with four windows in it.
2. The window group should be called 'WINDOWS'

# Location

## location.skp

An empty model that has been located in Country: New Zealand; City: Wellington; Latitude: 41.1S; Longitude: 174.1W; TimeZone: UTC+12

### Expected Behaviour

1. Loads correctly
2. All the inputs are well placed
3. Albedo is in default value (0.2)


## weather_file.skp

An empty model that has been assigned a weather file located in Country: CHL; City: SANTIAGO; Latitude: -33.38; Longitude: -70.78; TimeZone: UTC-12

### Expected Behaviour

1. Loads correctly
2. All the inputs are well placed
3. Albedo is set to 0.17


# Tasks

## df.skp

## da.skp

## task_udi.skp

A model that only has two simple 1x1 workplanes: "with task" and "without task". The former has an UDI task assigned. The task has the following data:

* name: "UDI"
* min_lux: 300
* max_lux: 3000
* early: 8
* late: 18
* min_month: 1
* max_month: 12
* min_time: 50

### Expected behavior

1. No throw on load
2. Has two workplanes
3. Has one task
4. The task is assigned only to the "with task" WP

# Materials

## default_materials.skp

A model that only has two surfaces: a Window and a surface without material

### Expected Behaviour

1. The window should have the Default Glass
2. The surface should have the Default Material
3. No components should be in the model
4. Only two materials should be in the model

## metal.skp

## glass.skp

