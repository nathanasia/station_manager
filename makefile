all: comptroller s_manager bus mystation

comptroller: comptroller.c 
	gcc -o comptroller comptroller.c -pthread

s_manager: station_manager.c
	gcc -o s_manager station_manager.c -pthread

bus: bus.c
	gcc -o bus bus.c -pthread

mystation: myport.c common.c
	gcc -o mystation myport.c common.c -pthread

clean:
	rm -f comptroller s_manager bus mystation
