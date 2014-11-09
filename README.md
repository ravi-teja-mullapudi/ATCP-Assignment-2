Install OpenCV
Install Python, NumPy

icpc -openmp -O3 harris.cpp -L /usr/local/lib/ -lopencv_imgproc -lopencv_core -lopencv_highgui -o harris.out -DANALYZE -DSHOQ -DNRUNS=5
g++ -openmp -O3 harris.cpp -L /usr/local/lib/ -lopencv_imgproc -lopencv_core -lopencv_highgui -o harris.out -DANALYZE -DSHOW -DNRUNS=5

Compiling the cpp file as a shared library
==========================================
icpc -xhost -openmp -fPIC -shared -o <file>.so <file>.cpp

* Try queuing frames using one thread and processing using the rest
* Add a explanation on how the demo works
* Include the bilateral grid that works for all sizes
  currently the pipeline is built for a specific size.
