# BootcampAmazon2022
Data management project for gas leak
detection
The system receives data from sensors and uses a dynamic library that
enables the following actions:
*Recording - creating a video consisting of a frame of a map of gas degrees
converted into colors.
The recording is carried out using the PIPELINE method which contains 5
steps
Each step is activated by a separate thread
The communication between the different threads is carried out in a
synchronized manner using a binary semaphore.
Every second 250 images are created that are part of the video.
The steps of the PIPELINE:
*Capture- creating a matrix of gas degrees by drawing the gas degrees
(getting the data from the sensors).
*Rgb_convert- receiving the degree matrix and converting the degrees to a
color matrix.
*Yuv_convert- compression of the color matrix to YUV format.

*stopping the recording - finishing creating frames - closing the video.
* Creating an image – catching a frame from the video / creating a frame
and saving it as an image.

The management and activation of the functions is carried out by a command
line adapted to the camera commands.
