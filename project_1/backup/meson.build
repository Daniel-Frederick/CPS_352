project('opencv_Mosaic_Effect', 'cpp')

opencv_dep = dependency('opencv4', required: true)

# Source files
src_files = files(
    './main.cpp'
)

# Build executable
executable('mosaic', src_files, dependencies: opencv_dep)
