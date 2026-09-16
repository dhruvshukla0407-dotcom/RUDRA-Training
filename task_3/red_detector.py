import cv2
import numpy as np

# Read the image
image = cv2.imread("images.jpeg")

if image is None:
    print("Error: Could not load image.")
    exit()

# Convert BGR to HSV
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

# Red has two ranges in HSV
lower_red1 = np.array([0, 120, 70])
upper_red1 = np.array([10, 255, 255])

lower_red2 = np.array([170, 120, 70])
upper_red2 = np.array([180, 255, 255])

# Create masks
mask1 = cv2.inRange(hsv, lower_red1, upper_red1)
mask2 = cv2.inRange(hsv, lower_red2, upper_red2)

# Combine masks
mask = mask1 + mask2

# Find contours
contours, _ = cv2.findContours(
    mask,
    cv2.RETR_EXTERNAL,
    cv2.CHAIN_APPROX_SIMPLE
)

# Detect red objects
for contour in contours:

    area = cv2.contourArea(contour)

    # Ignore very small red regions
    if area < 500:
        continue

    # Get bounding box coordinates
    x, y, w, h = cv2.boundingRect(contour)

    # Calculate center
    center_x = x + w // 2
    center_y = y + h // 2

    # Draw bounding box
    cv2.rectangle(
        image,
        (x, y),
        (x + w, y + h),
        (0, 255, 0),
        3
    )

    # Write coordinates
    text = f"X:{center_x} Y:{center_y}"

    cv2.putText(
        image,
        text,
        (x, y - 10),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.7,
        (0, 255, 0),
        2
    )

    # Draw center point
    cv2.circle(
        image,
        (center_x, center_y),
        5,
        (255, 0, 0),
        -1
    )

    print("Red object detected")
    print(f"Top-left: ({x}, {y})")
    print(f"Width: {w}, Height: {h}")
    print(f"Center: ({center_x}, {center_y})")

# Save the result
cv2.imwrite("result.jpg", image)

# Display the result
cv2.imshow("Red Object Detection", image)

cv2.waitKey(0)
cv2.destroyAllWindows()
