.SUFFIXES : .cpp .o

OBJECTS = main.o blobing.o CircularQueue.o ShapeDiscriptor.o
TARGET = cctv
INC = -I/usr/local/include/opencv2 -I/usr/local/include/opencv
DIRECTORY = -L/usr/local/lib/
DIRECTORY_UNDER = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching -lcvblob -lcurl


CXX = g++
CPPFLAGS = -g -c -std=c++11

cctv : $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) $(DIRECTORY) $(DIRECTORY_UNDER)
.cpp.o :
	$(CXX) $(INC) $(CPPFLAGS) $< $(DIRECTORY) $(DIRECTORY_UNDER) -fpermissive

main.o : starSkeleton.hpp main.hpp main.cpp
blobing.o : blobing.hpp blobing.cpp
CircularQueue.o : CircularQueue.hpp CircularQueue.cpp
starSkeleton.o : starSkeleton.hpp main.hpp
ShapeDiscriptor.o : ShapeDiscriptor.hpp ShapeDiscriptor.cpp
RegressionAnalysis.o : RegressionAnalysis.hpp main.hpp
clean :
	rm -rf $(OBJECTS) $(TARGET) temp.avi
