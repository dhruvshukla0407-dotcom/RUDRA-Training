import cv2 as cv
import numpy as np

# Load the image
image = cv.imread("circles.jpeg")

# Check if image was loaded
if image is None:
    print("Error: Could not open circles.jpeg")
    exit()

# Convert to grayscale
gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)

# Reduce noise
gray = cv.medianBlur(gray, 5)

# Detect circles
circles = cv.HoughCircles(
    gray,
    cv.HOUGH_GRADIENT,
    dp=1,
    minDist=50,
    param1=60,
    param2=70,
    minRadius=35,
    maxRadius=100
)

# Check if circles were detected
if circles is not None:

    # Convert circle values to integers
    circles = np.uint16(np.around(circles))

    # Draw each detected circle
    for circle in circles[0, :]:

        x = circle[0]
        y = circle[1]
        radius = circle[2]

        # Draw center
        cv.circle(image, (x, y), 3, (0, 255, 0), -1)

        # Draw circle outline
        cv.circle(image, (x, y), radius, (255, 0, 255), 3)

        print("Circle detected:")
        print("Center:", (x, y))
        print("Radius:", radius)
        print()

else:
    print("No circles detected.")

# Display result
cv.imshow("Detected Circles", image)

# Wait for a key press
cv.waitKey(0)

# Close windows
cv.destroyAllWindows()
