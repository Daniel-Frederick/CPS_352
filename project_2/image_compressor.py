import numpy as np
from PIL import Image

class ImageCompressor:
    """
    A class for compressing images using K-means clustering.
    """
    def __init__(self, image_path):
        """
        Initialize the ImageCompressor with an image.
        
        Args:
            image_path (str): Path to the image file
        """
        # Load and store the original image
        self.original_image = Image.open(image_path)
        self.original_array = np.array(self.original_image) / 255.0  # Normalize to [0, 1]
        
        # Dictionary to store k-means results for different k values
        self.kmeans_images = {}
        
        # Track the current k value
        self.current_k = 0
        
        # Store original image dimensions
        self.height, self.width, self.channels = self.original_array.shape
    
    def _calculate_distances(self, pixels, centers):
        """
        Calculate distances between pixels and cluster centers.
        
        Args:
            pixels (numpy.ndarray): Array of pixels
            centers (numpy.ndarray): Array of cluster centers
        
        Returns:
            numpy.ndarray: Array of cluster labels
        """
        # Initialize array to store labels
        labels = np.zeros(len(pixels), dtype=np.int32)
        
        # For each pixel, find the nearest center
        for i in range(len(pixels)):
            # Calculate distances to all centers
            distances = np.sum((centers - pixels[i]) ** 2, axis=1)
            # Assign pixel to the nearest center
            labels[i] = np.argmin(distances)
        
        return labels
    
    def _custom_kmeans(self, pixels, k, max_iterations=100):
        """
        Custom implementation of K-means clustering.
        
        Args:
            pixels (numpy.ndarray): Array of pixels
            k (int): Number of clusters
            max_iterations (int): Maximum number of iterations
        
        Returns:
            tuple: (labels, centers)
        """
        # Initialize random centers
        np.random.seed(42)  # For reproducibility
        idx = np.random.randint(0, len(pixels), k)
        centers = pixels[idx]
        
        # Previous centers for convergence check
        prev_centers = np.zeros_like(centers)
        
        # Repeat until convergence or max iterations
        for _ in range(max_iterations):
            # Assign pixels to nearest center
            labels = self._calculate_distances(pixels, centers)
            
            # Update centers
            prev_centers = centers.copy()
            for j in range(k):
                if np.sum(labels == j) > 0:  # Avoid empty clusters
                    centers[j] = np.mean(pixels[labels == j], axis=0)
            
            # Check for convergence
            if np.allclose(prev_centers, centers, rtol=1e-4):
                break
        
        return labels, centers
    
    def kmeans(self, num):
        """
        Perform k-means clustering on the image with the specified number of clusters.
        
        Args:
            num (int): Number of clusters (k) for K-means
        """
        # If k is 0 or already computed, return early
        if num == 0:
            return
        
        if num in self.kmeans_images:
            self.current_k = num
            return
        
        # Reshape the image for clustering
        pixels = self.original_array.reshape(-1, self.channels)
        
        # For large images, use a sample to improve performance
        sample_size = min(100000, len(pixels))
        np.random.seed(42)  # For reproducibility
        sample_indices = np.random.choice(len(pixels), sample_size, replace=False)
        sample_pixels = pixels[sample_indices]
        
        # Perform k-means clustering on the sample
        _, centers = self._custom_kmeans(sample_pixels, num)
        
        # Assign all pixels to their nearest center
        labels = self._calculate_distances(pixels, centers)
        
        # Replace each pixel with its cluster center
        compressed_pixels = centers[labels]
        
        # Reshape back to original image dimensions
        compressed_image = compressed_pixels.reshape(self.height, self.width, self.channels)
        
        # Ensure pixel values are in the valid range [0, 1]
        compressed_image = np.clip(compressed_image, 0, 1)
        
        # Store the result
        self.kmeans_images[num] = compressed_image
        self.current_k = num
    
    def get_image(self):
        """
        Get the current image based on the k value.
        
        Returns:
            numpy.ndarray: Original image if k=0, otherwise the compressed image
        """
        if self.current_k == 0:
            return self.original_array
        return self.kmeans_images[self.current_k]
