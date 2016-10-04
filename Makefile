all: webcamtest tracker capture contours

webcamtest: webcamtest.cc
	c++ -g -L /usr/lib -lopencv_features2d -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_core webcamtest.cc -o webcamtest

tracker: tracker.cc
	c++ -g -L /usr/lib -lopencv_features2d -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_core tracker.cc -o tracker

capture: capture.cc
	c++ -g -L /usr/lib -lopencv_features2d -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_core capture.cc -o capture

contours: contours.cc
	c++ -g -L /usr/lib -lopencv_features2d -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_core contours.cc -o contours

clean:
	rm -f webcamtest tracker capture contours
