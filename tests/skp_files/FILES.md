The files included in the skp_files folder are clasified by SketchUp version. They
also are supposed to enable testing the reading capabilities of emp_core

# Basic

## empty.skp
An empty file (i.e. Open Sketchup, save file) designed to test if a standard SketchUp model can be read properly

### Expected behavior
 1. No Throw on load
 2. Have 32 faces in the only component of version 2017

## deleted.skp

A model on which a 1x1x1 cube with a corner in the origin has ben drawn, and the top of it has been deleted. No materials assigned.

This is designed to test if deleted faces are an issue or not (in Ruby they sometimes are)

### Expected behavior

 1. No throw on load
 2. Have 5 faces in the first layer
 
 # Faces
 
 ## small_square.skp

It is a simple model, with two layers (Layer0 and Layer1), and a 1x1 meter square in Z = 0, and with vertices at (0,0), (0,1), (1,1), (1,0)

Designed to test how faces are imported

### Expected Behavior

1. Have two layers with the correct names (Layer0 and Layer1)
2. Have zero components
3. Have a face with four vertices, matching the correct ones

# Workplanes

## small_square_WP.skp
It is a simple model, with two layers (Layer0 and Layer1), and a 1x1 meter WorkPlane called 'WP' in Z = 0, and with vertices at (0,0), (0,1), (1,1), (1,0)

Designed to test how Workplanes are imported

### Expected behavior
1. Have two layers with the correct names (Layer0 and Layer1)
2. Have zero components
3. Have a workplane with four vertices, matching the correct ones

# Illums

## small_square_illum.skp

It is a simple model, with two layers (Layer0 and Layer1), and a 1x1 meter Illum called in Z = 0, and with vertices at (0,0), (0,1), (1,1), (1,0)

Designed to test how illums are imported

### Expected behavior

1. Have two layers with the correct names (Layer0 and Layer1)
2. Have zero components
3. Have an illum with four vertices, matching the correct ones


# Windows

## small_square_window.skp

It is a simple model, with two layers (Layer0 and Layer1), and a 1x1 meter window called in Z = 0, and with vertices at (0,0), (0,1), (1,1), (1,0)

Designed to test how squares are imported

### Expected behavior

1. Have two layers with the correct names (Layer0 and Layer1)
2. Have zero components
3. Have an window with four vertices, matching the correct ones

## window_group.skp

It is a simple model, with two four 1x1m windows called WINDOWS

Designed to test how window_groups are imported

### Expected behavior

1. Have a single window group with four windows in it.
2. The window group should be called 'WINDOWS'

# Location

# Tasks

# Materials

