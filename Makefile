CC = g++
LD = g++
CFLAGS = -c -g -std=c++11 -pedantic-errors -Wall
LFLAGS = -g

OBJS = p6_main.o Model.o View.o Controller.o 
OBJS += Grid_view.o List_view.o
OBJS += Map_view.o Local_view.o Agriculture_view.o
OBJS += Health_view.o Amounts_view.o
OBJS += Sim_object.o Structure.o Moving_object.o Agent.o
OBJS += Farm.o Town_Hall.o
OBJS += Peasant.o Warrior.o
OBJS += Soldier.o Archer.o Zealot.o
OBJS += Unit.o Group.o
OBJS += Agent_factory.o Structure_factory.o View_factory.o
OBJS += Geometry.o Utility.o
PROG = p6exe

default: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

p6_main.o: p6_main.cpp Model.h Controller.h
	$(CC) $(CFLAGS) p6_main.cpp

Model.o: Model.cpp Model.h View.h Sim_object.h Structure.h Agent.h Unit.h Group.h Agent_factory.h Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Model.cpp

View.o: View.cpp View.h Geometry.h Utility.h
	$(CC) $(CFLAGS) View.cpp

Grid_view.o: Grid_view.cpp Grid_view.h View.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Grid_view.cpp

Map_view.o: Map_view.cpp Map_view.h View.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Map_view.cpp

Local_view.o: Local_view.cpp Local_view.h View.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Local_view.cpp

Agriculture_view.o: Agriculture_view.cpp Agriculture_view.h View.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Agriculture_view.cpp

List_view.o: List_view.cpp List_view.h View.h Utility.h
	$(CC) $(CFLAGS) List_view.cpp

Health_view.o: Health_view.cpp Health_view.h View.h Utility.h
	$(CC) $(CFLAGS) Health_view.cpp

Amounts_view.o: Amounts_view.cpp View.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Amounts_view.cpp

Controller.o: Controller.cpp Controller.h Model.h View.h Sim_object.h Structure.h Agent.h Unit.h Group.h Agent_factory.h Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Controller.cpp

Sim_object.o: Sim_object.cpp Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Sim_object.cpp

Structure.o: Structure.cpp Structure.h Model.h Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Structure.cpp

Farm.o: Farm.cpp Farm.h Structure.h Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Farm.cpp

Town_Hall.o: Town_Hall.cpp Town_Hall.h Structure.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Town_Hall.cpp

Agent.o: Agent.cpp Agent.h Model.h Moving_object.h Sim_object.h Unit.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Agent.cpp

Peasant.o: Peasant.cpp Peasant.h Agent.h Moving_object.h Sim_object.h Unit.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Peasant.cpp

Warrior.o: Warrior.cpp Warrior.h Agent.h Moving_object.h Sim_object.h Unit.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Warrior.cpp

Archer.o: Archer.cpp Archer.h Warrior.h Agent.h Moving_object.h Sim_object.h Unit.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Archer.cpp

Soldier.o: Soldier.cpp Soldier.h Agent.h Warrior.h Moving_object.h Sim_object.h Unit.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Soldier.cpp

Zealot.o: Zealot.cpp Zealot.h Agent.h Warrior.h Moving_object.h Sim_object.h Unit.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Zealot.cpp

Moving_object.o: Moving_object.cpp Moving_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Moving_object.cpp

Unit.o: Unit.cpp Unit.h Utility.h
	$(CC) $(CFLAGS) Unit.cpp

Group.o: Group.cpp Group.h Unit.h Utility.h
	$(CC) $(CFLAGS) Group.cpp

Agent_factory.o: Agent_factory.cpp Agent_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Agent_factory.cpp

Structure_factory.o: Structure_factory.cpp Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Structure_factory.cpp

View_factory.o: View_factory.cpp View_factory.h Utility.h
	$(CC) $(CFLAGS) View_factory.cpp

Geometry.o: Geometry.cpp Geometry.h
	$(CC) $(CFLAGS) Geometry.cpp

Utility.o: Utility.cpp Utility.h
	$(CC) $(CFLAGS) Utility.cpp

clean:
	rm -f *.o
real_clean:
	rm -f $(PROG)
	rm -f *.o


play: Sim_object.h play.cpp
	g++ Structure.cpp play.cpp Geometry.cpp -o play
.PHONY: sample_test
sample_test:
	./p6exe < demo1_in.txt > demo1_out.tmp
	diff demo1_out.tmp demo1_out.txt -s
	./p6exe < demo2_in.txt > demo2_out.tmp
	diff demo2_out.tmp demo2_out.txt -s
	./p6exe < demo3_in.txt > demo3_out.tmp
	diff demo3_out.tmp demo3_out.txt -s