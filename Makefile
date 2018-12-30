#C++
GCC=g++

VPATH=src

OBJ=obj

#All relevant -I options here
INCLUDES= -I src

#Add other options for ocamlc here
CFLAGS=-O5 $(INCLUDES) -g -lgsl -lblas

#There must be one line per program
PROGRAM_OBJS=types.o uniform.o graph_builder.o graph_builder_txt.o graph_node_names.o graph_node_colors.o log.o vertex.o graph_kcores.o graph_kcores_components.o graph_kdenses.o graph_kdenses_components.o graph_of_triangles.o graph_triangled_kcores.o graph.o clique.o connectedcomponent.o connectivity.o network.o kcores_component.o kdenses_component.o parameters.o povray_renderer.o svg_renderer.o povray.o svg.o graphics_kcores.o graphics_kdenses.o ff_routine.o distribute_components.o utils.o main.o

#move must be at the end
all: deldoc lanet

lanet: $(PROGRAM_OBJS)
	@echo Linking $@...
	@$(GCC) -o $@ $+ $(CFLAGS)

deldoc:
	@rm -r -f doc/

#Common rules
.SUFFIXES: .cpp .o
.cpp.o:
	@echo Compiling $<
	@$(GCC) $(CFLAGS) -c $<

#Clean up
clean:
	@rm -f src/*.o
	@rm -f *.o
	@rm -f src/*~
	@rm -f *~

