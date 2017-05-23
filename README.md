
# K-Image segmenation using network flow
# Both the C++11 and python dependencies are freely available. Installation is quite simple.
Code Dependency:
C++ 11
C++ opencv 2.4.13

Python dependency:
  python version 2.7
  scikit-learn
  numpy
  opencv

# Compilation instructions:
If you have pkg-config for opencv, the following command should compile the file

# 2 Segmentation:

#Compilation:
g++ -std=c++11 imagesegmentation.cpp -o imagesegmentation `pkg-config --cflags --libs opencv`

PS: If you don't have pkg-config you can expand the command `pkg-config --cflags --libs opencv`
In MAC OSX the command expands as following. Might have to change your path depending on your installation!

-I/usr/local/Cellar/opencv/2.4.13/include/opencv -I/usr/local/Cellar/opencv/2.4.13/include -L/usr/local/Cellar/opencv/2.4.13/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab

To Run 2 segmenation, you can use 500*500 which works within 3*4 minutes. Images under that size are pretty fast. The k-segmentation repeatedly runs network flow and hence is slower. Please run k-segmentation 200*200 (might take 7-8 mins )!

The downsample tool is avaialble to downsample the images
python downsample.py <imagename> <width> <height>

<imagename> Just provide the imagename. No path. All images should be in the image folder
<width> integer value width of the image
<height> integer value of height of the image.

# RUN:
Please provide the <imagename> directly. Please don't provide the path. The images should be added into the image folder. The program automatically looks for images in the image folder.

./imagesegmentation <imagename> <clustering>

 <imagename> - name of the imagefile
 <clustering> - an integer value 0 or 1, if you want to use gmm or k-means. It uses gmm by default.
 
# K Segmenation :
 
 g++ -std=c++11 kimagesegmentation.cpp -o kimagesegmentation `pkg-config --cflags --libs opencv`
 
 Please look into the pkg-config expansion above.
 
# RUN K-Segmentation
 
 ./kimagesegmentation <imagename> <clustering> <numberOfSegments>
 
 <imagename> - name of the imagefile
 <clustering> - an integer value 0 or 1, if you want to use gmm or k-means
 <numberOfSegments> - The total number of segments you want the image to be divided into.
 
 
 
  
