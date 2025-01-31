project('opencv_Mosaic_Effect', 'cpp')

opencv_dep = dependency('opencv4', required: true)

# Set include directory
inc = include_directories('include')

# Source files
src_files = files(
    'src/events.cpp',
    'src/main.cpp'
)

# Build executable
executable('mosaic', src_files, include_directories: inc, dependencies: opencv_dep)
