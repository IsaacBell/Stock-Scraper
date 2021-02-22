CFLAGS = -DCURL_STATICLIB

CURL_CFLAGS = -I/usr/local/Cellar/libcurl/include/

CURL_LIBS = -L/usr/local/Cellar/libcurl/lib -lcurl

OBJCXXFLAGS = -DCURL_STATICLIB

CXX      := -c++17
LDFLAGS  := -L/usr/lib -lstdc++ -lm
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := program
INCLUDE  := -Iinclude/
SRC      := ./quant/maineq1.cpp

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
   @mkdir -p $(@D)
   $(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(APP_DIR)/$(TARGET): $(OBJECTS)
   @mkdir -p $(@D)
   $(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

.PHONY: all build clean debug release

build:
   @mkdir -p $(APP_DIR)
   @mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
   -@rm -rvf $(OBJ_DIR)/*
   -@rm -rvf $(APP_DIR)/*
