from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import Slider, Button
from image_compressor import ImageCompressor
import os

# List of image filenames
image_filenames = ["assets/test1.jpg", "assets/test2.jpg", "assets/test3.jpg", "assets/test4.jpg", "assets/test5.jpg"]

# Check if files exist
for filename in image_filenames:
    if not os.path.exists(filename):
        raise FileNotFoundError(f"File not found: {filename}")

# Create a list of ImageCompressor objects
image_compressor_list = [ImageCompressor(filename) for filename in image_filenames]

# Initialize with the first image
current_image_index = 0

# Create the initial plot
fig, ax = plt.subplots(figsize=(8, 8))
plt.subplots_adjust(bottom=0.3)  # Adjust space for buttons and slider
ax.set_title(f"Image Compression with K-Means Clustering")
ax.axis('off')

# Display the initial image
img_display = ax.imshow(image_compressor_list[current_image_index].get_image())

# Add a slider for k-means control
ax_slider = plt.axes([0.25, 0.1, 0.5, 0.03])  # x, y, width, height
slider = Slider(ax_slider, 'Clusters (k)', 0, 64, valinit=image_compressor_list[current_image_index].current_k, valstep=1)

# Update function for slider
def update(val):
    k = int(slider.val)
    # Update the current k value
    image_compressor_list[current_image_index].current_k = k
    
    # If k > 0, perform k-means clustering if not already computed
    if k > 0:
        image_compressor_list[current_image_index].kmeans(k)
    
    # Update the displayed image
    img_display.set_data(image_compressor_list[current_image_index].get_image())
    
    # Update the title to reflect the current k value
    ax.set_title(f"Image Compression with K-Means Clustering (k={k})")
    
    # Redraw the canvas
    fig.canvas.draw_idle()

# Function to load and display the next/previous image
def change_image(direction):
    global current_image_index
    if direction == 'next':
        # Move to the next image, wrapping around if necessary
        current_image_index = (current_image_index + 1) % len(image_compressor_list)
    elif direction == 'prev':
        # Move to the previous image, wrapping around if necessary
        current_image_index = (current_image_index - 1) % len(image_compressor_list)
    
    # Get the current k value for this image
    current_k = image_compressor_list[current_image_index].current_k
    
    # Update the slider to match the current k value
    slider.set_val(current_k)
    
    # Update the displayed image
    img_display.set_data(image_compressor_list[current_image_index].get_image())
    
    # Update the title
    ax.set_title(f"Image Compression with K-Means Clustering (k={current_k})")
    
    # Redraw the canvas
    fig.canvas.draw_idle()

# Add "Previous" and "Next" buttons
ax_prev = plt.axes([0.1, 0.01, 0.1, 0.075])  # x, y, width, height
ax_next = plt.axes([0.8, 0.01, 0.1, 0.075])
button_prev = Button(ax_prev, 'Previous')
button_next = Button(ax_next, 'Next')

# Define the button callbacks
button_prev.on_clicked(lambda event: change_image('prev'))
button_next.on_clicked(lambda event: change_image('next'))

# Connect the slider to the update function
slider.on_changed(update)

# Show the plot
plt.show()
