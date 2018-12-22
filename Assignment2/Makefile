CPPFLAGS=-std=c++14 -g -O0 -Wall -Wextra

SRCS=main.cpp Point.cpp Ball.cpp Paddle.cpp Brick.cpp World.cpp
OBJS=$(SRCS:.cpp=.o)

all: arkanoid

arkanoid: $(OBJS)
	$(CXX) $^ -o $@

%.o: %.cpp %.h
	$(CXX) $(CPPFLAGS) -c $<

clean:
	rm -rf arkanoid
	rm -rf $(OBJS)