# Perception Challenge Submission

Submitted by: Max Maeder (mmaeder@wisc.edu)

Draws boundary lines of the path defined by the cones, and saves the result as `answer.png`

## `Answer.png`
![Answer.png](/assets/answer.png)

## Methodolgy
The image processing pipeline I used was:
1) Extract red channel from image
2) Apply threshold to isolate really-red objects
3) Find contours in image
4) Generate a bounding box around each contour
    - Throw out contours with too small an area
    - Throw out contours that are too squat (can't be cones)
5) Find center of contour
6) Draw regression lines using contour centers as points on the lines

## What did you try and why do you think it did not work
I struggled to find a good way to bound the different cones since I have limited experience with OpenCV. I'm happy with the solution I ended up with.
## What libraries are used
1) OpenCV
2) STD