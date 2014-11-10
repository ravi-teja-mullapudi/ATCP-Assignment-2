Compiling the reference implementation
--------------------------------------
Install OpenCV with QT or gtk

These two urls should help. 

http://karytech.blogspot.in/2012/05/opencv-24-on-ubuntu-1204.html
http://opencv-python-tutroals.readthedocs.org/en/latest/py_tutorials/py_setup/py_setup_in_fedora/py_setup_in_fedora.html

One can also install from source using the documentation on the OpenCV website

icpc -openmp -O3 harris.cpp -L /usr/local/lib/ -lopencv_imgproc -lopencv_core -lopencv_highgui -o harris -DANALYZE -DSHOQ -DNRUNS=5

g++ -openmp -O3 harris.cpp -L /usr/local/lib/ -lopencv_imgproc -lopencv_core -lopencv_highgui -o harris -DANALYZE -DSHOW -DNRUNS=5

./harris.out path_to_image_file

Running the python demo
-----------------------
Install Python, NumPy (the urls above cover this installation) 

Compile the reference implementation in the video folder as a shared library

icpc -xhost -openmp -fPIC -shared -o harris.so harris_ref.cpp

Run the python script as

python path_to_video_file 1/0
1 - for displaying the video
0 - for running 25 frames using OpenCV and 25 frames using reference implementation


