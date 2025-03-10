from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import Slider, Button
from image_compressor import image_compressor
import os

# List of image filenames
image_filenames = ["assets/test1.jpg", "assets/test2.jpg", "assets/test3.jpg", "assets/test4.jpg", "assets/test5.jpg"]

# Check if files exist
for filename in image_filenames:
    if not os.path.exists(filename):
        raise FileNotFoundError(f"File not found: {filename}")

# Load the initial image
current_image_index = 0
image = Image.open(image_filenames[current_image_index])
image_array = np.array(image) / 255.0  # Normalize to range [0, 1]

# Create the initial plot
fig, ax = plt.subplots(figsize=(8, 8))
plt.subplots_adjust(bottom=0.3)  # Adjust space for buttons and slider
ax.set_title("Brightness Adjustment")
ax.axis('off')

# Display the initial image
img_display = ax.imshow(image_array)

# Add a slider for brightness control
ax_slider = plt.axes([0.25, 0.1, 0.5, 0.03])  # x, y, width, height
slider = Slider(ax_slider, 'Brightness', 0.1, 2.0, valinit=1.0, valstep=0.1)

# Update function for slider
def update(val):
    scale = slider.val
    # Scale the brightness of the image
    adjusted_image = np.clip(image_array * scale, 0, 1)  # Keep values in range [0, 1]
    img_display.set_data(adjusted_image)  # Update the displayed image
    fig.canvas.draw_idle()  # Redraw the canvas

# Function to load and display the next/previous image
def change_image(direction):
    global current_image_index, image_array
    if direction == 'next':
        # Move to the next image, wrapping around if necessary
        current_image_index = (current_image_index + 1) % len(image_filenames)
    elif direction == 'prev':
        # Move to the previous image, wrapping around if necessary
        current_image_index = (current_image_index - 1) % len(image_filenames)
    
    # Load the new image and update the display
    image = Image.open(image_filenames[current_image_index])
    image_array = np.array(image) / 255.0  # Normalize to range [0, 1]
    img_display.set_data(image_array)  # Update the displayed image
    fig.canvas.draw_idle()  # Redraw the canvas

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
