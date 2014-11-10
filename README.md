Install OpenCV

Install Python, NumPy

icpc -openmp -O3 harris.cpp -L /usr/local/lib/ -lopencv_imgproc -lopencv_core -lopencv_highgui -o harris.out -DANALYZE -DSHOQ -DNRUNS=5

g++ -openmp -O3 harris.cpp -L /usr/local/lib/ -lopencv_imgproc -lopencv_core -lopencv_highgui -o harris.out -DANALYZE -DSHOW -DNRUNS=5

./harris.out path_to_image_file

Compiling the cpp file as a shared library
------------------------------------------
icpc -xhost -openmp -fPIC -shared -o <file>.so <file>.cpp
