target = test_data_stream.o data_stream.o handle_data_package.o xml_parse.o relevant_struct_def.o\
	address_map.o route_map.o handle_event.o handle_event_func.o event.o

test_data_stream : $(target)
	cc -o test_data_stream $(target) -lmxml -lpthread
test_data_stream.o : test_data_stream.c ../../data_stream.h
	cc -c test_data_stream.c
data_stream.o : ../../data_stream.c ../../data_stream.h
	cc -c ../../data_stream.c
handle_data_package.o : ../../handle_data_package.c ../../handle_data_package.h
	cc -c ../../handle_data_package.c
xml_parse.o : ../../xml_parse.c ../../xml_parse.h
	cc -c ../../xml_parse.c
relevant_struct_def.o : ../../relevant_struct_def.c ../../relevant_struct_def.h
	cc -c ../../relevant_struct_def.c
address_map.o : ../../address_map.c ../../address_map.h
	cc -c ../../address_map.c
route_map.o : ../../route_map.c ../../route_map.h
	cc -c ../../route_map.c
handle_event.o : ../../handle_event.c ../../handle_event.h
	cc -c ../../handle_event.c
handle_event_func.o : ../../handle_event_func.c ../../handle_event_func.h
	cc -c ../../handle_event_func.c
event.o : ../../event.c ../../event.h
	cc -c ../../event.c
.PHONY : clean
clean:
	rm test_data_stream $(target)
