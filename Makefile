# Makefile para DuckHunt
# Variables configurables:
# - SFML_INCLUDE: ruta a los includes de SFML (ej: C:/SFML/include)
# - SFML_LIB: ruta a las librerías de SFML (ej: C:/SFML/lib)
# Si SFML está en el PATH del compilador, no es necesario definir estas variables.

CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -I./
SFML_INCLUDE ?=
SFML_LIB ?=

# librerías a enlazar (cambiar si tu instalación usa nombres con-s / -lsfml-graphics-s, etc.)
LDLIBS ?= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
LDFLAGS := $(if $(SFML_LIB),-L$(SFML_LIB),)
CPPFLAGS := $(if $(SFML_INCLUDE),-I$(SFML_INCLUDE),)

SRCS := duckhunt_demo.cpp Duck.cpp GameManager.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := DuckHunt

# Run command cross-platform
ifeq ($(OS),Windows_NT)
	RUN := .\\$(TARGET).exe
else
	RUN := ./$(TARGET)
endif

.PHONY: all clean run help

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

run: all
	@echo "Running $(TARGET)..."
	$(RUN)

help:
	@echo "Makefile DuckHunt"
	@echo "Usage:"
	@echo "  make            # compila el juego"
	@echo "  make run        # compila y ejecuta"
	@echo "  make clean      # limpia objetos y binarios"
	@echo "Variables opcionales (ejemplo):"
	@echo "  make SFML_INCLUDE=C:/SFML/include SFML_LIB=C:/SFML/lib"

# Nota para Windows: si usas SFML con DLLs, asegúrate de tener las DLLs en el mismo
# directorio del ejecutable o en el PATH antes de ejecutar `make run`.
