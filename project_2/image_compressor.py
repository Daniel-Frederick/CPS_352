class image_compressor:
    def __init__(self, original_image, kmeans_images) -> None:
        self.original_image = original_image
        self.kmeans_images = kmeans_images

    def kmeans(self, num):
        pass

    def get_image(self):
        pass

# Attributes:
# ■ original_image: Stores the original image (using the PIL library to
# load and convert the image into a numpy array).
# ■ kmeans_images: A dictionary that maps each k-value (number of
# clusters) to its corresponding compressed image (as a numpy array).
# ○ Methods:
# ■ kmeans(num): A method that accepts a number num (representing the
# number of clusters k) and performs k-means clustering on the image. The
# result should be stored in the kmeans_images dictionary under the key
# num. Each time the method is called, the compressed image is generated
# and stored.
# ■ get_image(): A method that returns either the original image (if k=0) or
# the compressed k-means image (based on the current k value). This
# method is used to retrieve the image for display.
